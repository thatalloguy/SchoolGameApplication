#pragma once
#include "AudioDevice.h"
#include <spdlog/spdlog.h>


#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

void Tyche::AudioEngine::soundEffectProccessPCMFrames(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut, ma_uint32 *pFrameCountOut)  {
    SoundEffect* soundEffect = (SoundEffect*)pNode;
    IPLBinauralEffectParams  binauralEffectParams;
    IPLAudioBuffer inputBufferDesc;
    IPLAudioBuffer outputBufferDesc;
    ma_uint32 totalFramesToProcess = *pFrameCountOut;
    ma_uint32 totalFramesProcessed = 0;

    binauralEffectParams.direction.x = soundEffect->direction[0];
    binauralEffectParams.direction.y = soundEffect->direction[1];
    binauralEffectParams.direction.z = 1;
    binauralEffectParams.interpolation = IPL_HRTFINTERPOLATION_NEAREST;
    binauralEffectParams.spatialBlend = 1.0f;
    binauralEffectParams.hrtf = soundEffect->HRTF;
    binauralEffectParams.peakDelays = nullptr;


    IPLDirectEffectParams directEffectParams{};
    IPLDistanceAttenuationModel model{};

    model.type = IPL_DISTANCEATTENUATIONTYPE_DEFAULT;
    directEffectParams.flags = IPL_DIRECTEFFECTFLAGS_APPLYDISTANCEATTENUATION;
    directEffectParams.distanceAttenuation = iplDistanceAttenuationCalculate(soundEffect->context, IPLVector3{soundEffect->soundPosition[0], soundEffect->soundPosition[1], 1}, IPLVector3{soundEffect->playerPosition[0], soundEffect->playerPosition[1], 1}, &model);

    inputBufferDesc.numChannels = (IPLint32) ma_node_get_input_channels(pNode, 0);

    //use a loop in case the deinterleaved buffers are too small
    outputBufferDesc.numSamples = soundEffect->audioSettings.frameSize;
    outputBufferDesc.numChannels = 2;
    outputBufferDesc.data = soundEffect->outBuffer;

    while (totalFramesProcessed < totalFramesToProcess) {

        ma_uint32  framesToProcessThisIteration = totalFramesToProcess - totalFramesProcessed;
        if  (framesToProcessThisIteration > (ma_uint32)soundEffect->audioSettings.frameSize) {
            framesToProcessThisIteration = (ma_uint32)soundEffect->audioSettings.frameSize;
        }

        if (inputBufferDesc.numChannels == 1) {
            // no need for deinterleaving since its a mono stream.
            soundEffect->inBuffer[0] = (float*) ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed, 1);
        } else {
            // womp womp we need to deinterleave the input stream.
            ma_deinterleave_pcm_frames(ma_format_f32, inputBufferDesc.numChannels, framesToProcessThisIteration,
                                       ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed, inputBufferDesc.numChannels),
                                       (void **) soundEffect->inBuffer); // note the example doenst cast to (void **)
        }

        inputBufferDesc.data = soundEffect->inBuffer;
        inputBufferDesc.numSamples = (IPLint32) framesToProcessThisIteration;
        // spannend



        iplBinauralEffectApply(soundEffect->binauralEffect, &binauralEffectParams, &inputBufferDesc, &outputBufferDesc);

        iplDirectEffectApply(soundEffect->directEffect, &directEffectParams, &outputBufferDesc, &outputBufferDesc);
        // then apply the direct efffect?

        ma_interleave_pcm_frames(
                ma_format_f32,
                2,
                framesToProcessThisIteration,
                (const void **) soundEffect->outBuffer, // note the example also doenst cast anything here.
                ma_offset_pcm_frames_ptr_f32(ppFramesOut[0], totalFramesProcessed, 2)
        );


        totalFramesProcessed += framesToProcessThisIteration;
    }

    (void)pFrameCountIn; // unused?

}

Tyche::SoundEffectConfig Tyche::AudioEngine::initSoundEffectConfig(ma_uint32 channelsIn, IPLAudioSettings audioSettings, IPLContext context, IPLHRTF hrtf)  {
    SoundEffectConfig config;

    MA_ZERO_OBJECT(&config);
    config.nodeConfig = ma_node_config_init();
    config.channelsIn = channelsIn;
    config.audioSettings = audioSettings;
    config.context = context;
    config.HRTF = hrtf;

    return config;
}

ma_result Tyche::AudioEngine::initSoundEffect(ma_node_graph* nodeGraph, const SoundEffectConfig* config, const ma_allocation_callbacks* allocationCallbacks, SoundEffect& soundEffect) {
    ma_result result;
    ma_node_config baseConfig;
    ma_uint32 channelsIn;
    ma_uint32 channelsOut;
    size_t heapSizeInBytes;

    IPLBinauralEffectSettings  effectSettings{};
    IPLDirectEffectSettings  directEffectSettings{};


    MA_ZERO_OBJECT(&soundEffect);

    if (config == NULL || !(config->channelsIn == 2 || config->channelsIn == 1)) {
        return MA_INVALID_ARGS;
    }

    channelsIn = config->channelsIn;
    channelsOut = 2; // must be stereo

    baseConfig = ma_node_config_init();
    baseConfig.vtable = &this->soundNodeVTable;
    baseConfig.pInputChannels = &channelsIn;
    baseConfig.pOutputChannels = &channelsOut;


    soundEffect.audioSettings = config->audioSettings;
    soundEffect.context       = config->context;
    soundEffect.HRTF          = config->HRTF;



    //Initialize Binaural effect
    effectSettings.hrtf = soundEffect.HRTF;


    directEffectSettings.numChannels = 2;

    result = ma_node_init(nodeGraph, &baseConfig, allocationCallbacks, &soundEffect.baseNode);
    if (result != MA_SUCCESS) {
        return result;
    }



    auto success = iplBinauralEffectCreate(soundEffect.context, &soundEffect.audioSettings, &effectSettings, &soundEffect.binauralEffect);
    if (success != IPL_STATUS_SUCCESS) {
        return MA_INVALID_DATA;
    }

    success = iplDirectEffectCreate(soundEffect.context, &soundEffect.audioSettings, &directEffectSettings, &soundEffect.directEffect);
    if (success != IPL_STATUS_SUCCESS) {
        return MA_INVALID_DATA;
    }

    heapSizeInBytes = 0;

    heapSizeInBytes += sizeof(float) * channelsOut * soundEffect.audioSettings.frameSize; // out buffer
    heapSizeInBytes += sizeof(float) * channelsIn * soundEffect.audioSettings.frameSize; // in buffer

    soundEffect._heap = ma_malloc(heapSizeInBytes, allocationCallbacks);
    if (soundEffect._heap == NULL) {
        iplBinauralEffectRelease(&soundEffect.binauralEffect);
        iplDirectEffectRelease(&soundEffect.directEffect);
        ma_node_uninit(&soundEffect.baseNode, allocationCallbacks);
        return MA_OUT_OF_MEMORY;
    }

    soundEffect.outBuffer[0] = (float*)soundEffect._heap;
    soundEffect.outBuffer[1] = (float*)ma_offset_ptr(soundEffect._heap, sizeof(float) * soundEffect.audioSettings.frameSize);

    ma_uint32 iChannelIn;
    for (iChannelIn = 0; iChannelIn < channelsIn; iChannelIn += 1) {
        soundEffect.inBuffer[iChannelIn] = (float*)ma_offset_ptr(soundEffect._heap, sizeof(float) * soundEffect.audioSettings.frameSize * (channelsOut + iChannelIn));
    }

    return MA_SUCCESS;
}

void Tyche::AudioEngine::destroySoundEffect(SoundEffect *binauralEffect,
                                            const ma_allocation_callbacks *allocationCallbacks) {
    if (binauralEffect == NULL) {
        spdlog::error("CPP IS TWEAKING IS SWEAR TO GOD");
        return;
    }

    ma_node_uninit(&binauralEffect->baseNode, allocationCallbacks);

    iplBinauralEffectRelease(&binauralEffect->binauralEffect);
    iplDirectEffectRelease(&binauralEffect->directEffect);

    ma_free(binauralEffect->_heap, allocationCallbacks);

}

void Tyche::AudioEngine::Init() {
    initializeMiniAudioObjects();
    initializeSteamAudioObjects();
}

void Tyche::AudioEngine::CleanUp() {

    destroySteamAudioObjects();
    destroyMiniAudioObjects();


    for (auto soundObject : _objects) {
        delete soundObject;
    }

}

void Tyche::AudioEngine::initializeMiniAudioObjects() {
    // load engine config
    engineConfig = ma_engine_config_init();
    engineConfig.channels = CHANNELS;
    engineConfig.sampleRate = SAMPLE_RATE;
    engineConfig.periodSizeInFrames = 256;

    result = ma_engine_init(&engineConfig, &engine);
    if (result != MA_SUCCESS) {
        spdlog::info("Miniaudio initialization error: {}", ma_result_description(result));
        exit(-105);
    }


}

void Tyche::AudioEngine::initializeSteamAudioObjects() {
    // Steam audio initialization.
    MA_ZERO_OBJECT(&iplAudioSettings);
    iplAudioSettings.samplingRate = ma_engine_get_sample_rate(&engine);

    iplAudioSettings.frameSize = engineConfig.periodSizeInFrames;



    iplContextSettings.version = STEAMAUDIO_VERSION;
    auto success = iplContextCreate(&iplContextSettings, &iplContext);
    if (success != IPL_STATUS_SUCCESS) {
        spdlog::info("steamaudio initialization error: {}", (int) success);
        exit(-106);
    }


    //HRTF
    MA_ZERO_OBJECT(&iplhrtfSettings);
    iplhrtfSettings.type = IPL_HRTFTYPE_DEFAULT;
    iplhrtfSettings.volume = 1.0f;

    success = iplHRTFCreate(iplContext, &iplAudioSettings, &iplhrtfSettings, &iplHRTF);

    if (success != IPL_STATUS_SUCCESS) {
        spdlog::info("steamaudio initialization error: {}",  (int) success);
        exit(-107);
    }
}

void Tyche::AudioEngine::destroySteamAudioObjects() {



    iplContextRelease(&iplContext);
    iplHRTFRelease(&iplHRTF);
}

void Tyche::AudioEngine::destroyMiniAudioObjects() {


    for (auto soundObject : _objects) {
        ma_sound_uninit(&soundObject->g_sound);
        destroySoundEffect(&soundObject->soundEffect, nullptr);
    }


    ma_engine_uninit(&engine);

}

Tyche::SoundID Tyche::AudioEngine::registerSound(SoundCreationInfo info) {
    idCounter++;
    SoundObject* newSoundObject = new SoundObject{};

    ma_sound_config soundConfig;

    soundConfig = ma_sound_config_init();
    soundConfig.pFilePath = info.filePath;
    soundConfig.flags = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT;
    result = ma_sound_init_ex(&engine, &soundConfig, &newSoundObject->g_sound);
    if (result != MA_SUCCESS) {
        return -108;
    }

    // No need for miniaudio to do this, since steam audio does this already
    ma_sound_set_directional_attenuation_factor(&newSoundObject->g_sound, 0);

    ma_sound_set_looping(&newSoundObject->g_sound, info.shouldLoop);


    newSoundObject->soundEffect.spatialBlend = info.spatialBlend;

    // add binauralEffect
    SoundEffectConfig binauralEffectConfig;

    binauralEffectConfig = initSoundEffectConfig(CHANNELS, iplAudioSettings, iplContext, iplHRTF);

    result = initSoundEffect(ma_engine_get_node_graph(&engine), &binauralEffectConfig, NULL, newSoundObject->soundEffect);

    if (result != MA_SUCCESS) {
        spdlog::error("Could not initialize binaural Effect: {}", ma_result_description(result));
        return -1;
    }

    ma_node_attach_output_bus(&newSoundObject->soundEffect, 0, ma_engine_get_endpoint(&engine), 0);

    ma_node_attach_output_bus(&newSoundObject->g_sound, 0, &newSoundObject->soundEffect, 0);
    registry.put(idCounter, newSoundObject);
    _objects.push_back(newSoundObject);

    return idCounter;
}



void Tyche::AudioEngine::playSound(SoundID id, const Math::Vector2& pos, float volume) {

    SoundObject* obj = registry.get(id);

    auto sound = &obj->g_sound;
    obj->soundEffect.soundPosition = pos;

    ma_sound_set_volume(sound, volume);
    ma_sound_start(sound);

    _queue.push_back(obj);
}

bool Tyche::AudioEngine::isSoundPlaying(Tyche::SoundID id) {
    return (bool) ma_sound_is_playing(&registry.get(id)->g_sound);
}

void Tyche::AudioEngine::update(const Tyche::Math::Vector2 &listenerPos) {
    for (int i=0; i<_queue.length(); i++) {

        auto sound = _queue[i];

        if (ma_sound_is_playing(&sound->g_sound)) {

            //Update the direction
            sound->soundEffect.direction = sound->soundEffect.soundPosition - listenerPos;
            sound->soundEffect.direction.normalize();

            sound->soundEffect.playerPosition = listenerPos;
        } else {
            _queue.remove(i);
        }
    }
}