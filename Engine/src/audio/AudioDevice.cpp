//
// Created by allos on 11/29/2024.
//

#include "AudioDevice.h"
#include "stl/hashmap.h"
#include "stl/vector.h"



#include <spdlog/spdlog.h>


#include <phonon.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>


namespace {

    //Global Structs
    struct NodeConfig {
        ma_node_config node_config;
        ma_uint32 channels_in;
    };

    struct SoundNode {
        ma_node_base base_node;
        Tyche::AudioBuffer buffer;
    };

    struct SoundObject {
        ma_sound sound;
        SoundNode node;
    };


    //Global SteamAudio Objects
    IPLAudioSettings ipl_audio_settings;
    IPLContextSettings ipl_context_settings;
    IPLContext ipl_context = nullptr;
    IPLHRTFSettings ipl_hrtf_settings;
    IPLHRTF ipl_hrtf;

    //Global Effect objects
    IPLBinauralEffect binaural_effect = nullptr;
    IPLDirectEffect direct_effect = nullptr;


    //Global Miniaudio objects
    ma_result result;
    ma_engine engine;
    ma_engine_config engine_config;

    //Other Misc Globals
    unsigned int id_counter = 0;
    bool need_effects = false; // See initializeDevice comment in the header for variable explanation
    Tyche::STL::hashmap<unsigned int, SoundObject*> _sounds;
    // Using a separate vector to easily iterate through the sounds, since the current hashmap class doesnt support it
    Tyche::STL::vector<SoundObject*> _sound_ptrs;


    //our frame processing function

    void effectProccessFrames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut) {
        SoundNode* sound_node = (SoundNode*) pNode;

        IPLBinauralEffectParams  binauralEffectParams;
        IPLAudioBuffer inputBufferDesc;
        IPLAudioBuffer outputBufferDesc;
        ma_uint32 totalFramesToProcess = *pFrameCountOut;
        ma_uint32 totalFramesProcessed = 0;

        binauralEffectParams.direction.x = 1;
        binauralEffectParams.direction.y = 1;
        binauralEffectParams.direction.z = 1;
        binauralEffectParams.interpolation = IPL_HRTFINTERPOLATION_NEAREST;
        binauralEffectParams.spatialBlend = 1.0f;
        binauralEffectParams.hrtf = ipl_hrtf;
        binauralEffectParams.peakDelays = nullptr;

        inputBufferDesc.numChannels = (IPLint32) ma_node_get_input_channels(pNode, 0);

        outputBufferDesc.numSamples = ipl_audio_settings.frameSize;
        outputBufferDesc.numChannels = 2;
        outputBufferDesc.data = sound_node->buffer.out_buffer;

        while (totalFramesProcessed < totalFramesToProcess) {
            ma_uint32  framesToProcessThisIteration = totalFramesToProcess - totalFramesProcessed;
            if  (framesToProcessThisIteration > (ma_uint32) ipl_audio_settings.frameSize) {
                framesToProcessThisIteration = (ma_uint32) ipl_audio_settings.frameSize;
            }

            if (inputBufferDesc.numChannels == 1) {
                // no need for deinterleaving since its a mono stream.
                sound_node->buffer.in_buffer[0] = (float*) ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed, 1);
            } else {
                // womp womp we need to deinterleave the input stream.
                ma_deinterleave_pcm_frames(FORMAT, inputBufferDesc.numChannels,
                                           framesToProcessThisIteration,
                                           ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed,inputBufferDesc.numChannels),
                                           reinterpret_cast<void **>(&sound_node->buffer.in_buffer)); // note the example doenst cast to (void **)
            }

            inputBufferDesc.data = sound_node->buffer.in_buffer;
            inputBufferDesc.numSamples = (IPLint32) framesToProcessThisIteration;
            // spannend



            iplBinauralEffectApply(binaural_effect, &binauralEffectParams, &inputBufferDesc, &outputBufferDesc);


            ////iplDirectEffectApply(soundEffect->directEffect, &directEffectParams, &outputBufferDesc, &outputBufferDesc);


            ma_interleave_pcm_frames(
                    ma_format_f32,
                    2,
                    framesToProcessThisIteration,
                    reinterpret_cast<const void **>(&sound_node->buffer.out_buffer), // note the example also doenst cast anything here.
                    ma_offset_pcm_frames_ptr_f32(ppFramesOut[0], totalFramesProcessed, 2)
            );


            totalFramesProcessed += framesToProcessThisIteration;
        }

        (void)pFrameCountIn; // unused?
    }



    ma_node_vtable sound_node_VTable {
            effectProccessFrames,
            NULL,
            1,
            1,
            0
    };

}


namespace SteamAudioUtils {

    void initializeSteamAudio() {
        MA_ZERO_OBJECT(&ipl_audio_settings);

        ipl_audio_settings.samplingRate = ma_engine_get_sample_rate(&engine);
        ipl_audio_settings.frameSize = engine_config.periodSizeInFrames;

        ipl_context_settings.version = STEAMAUDIO_VERSION;

        IPLerror success = iplContextCreate(&ipl_context_settings, &ipl_context);
        if (success != IPL_STATUS_SUCCESS) {
            spdlog::error("SteamAudio failed to initialize");
        }


        // Setup the HRTF
        MA_ZERO_OBJECT(&ipl_hrtf_settings);
        ipl_hrtf_settings.type = IPL_HRTFTYPE_DEFAULT;
        ipl_hrtf_settings.volume = 1.0f;

        success = iplHRTFCreate(ipl_context, &ipl_audio_settings, &ipl_hrtf_settings, &ipl_hrtf);
        if (success != IPL_STATUS_SUCCESS) {
            spdlog::error("SteamAudio failed to create HRTF");
        }

        spdlog::info("initialized SteamAudio");
    }

    void destroySteamAudio() {
        iplContextRelease(&ipl_context);
        iplHRTFRelease(&ipl_hrtf);
    }

    Tyche::AudioResult initializeSoundEffects() {

        IPLBinauralEffectSettings  binaural_settings{};
        IPLDirectEffectSettings    direct_settings{};


        binaural_settings.hrtf = ipl_hrtf;
        direct_settings.numChannels = 2;

        auto success = iplBinauralEffectCreate(ipl_context, &ipl_audio_settings, &binaural_settings, &binaural_effect);

        if (success != IPL_STATUS_SUCCESS) {
            spdlog::error("Failed to setup binaural effect");

            if (need_effects) {
                exit(-401);
            }
            return Tyche::AudioResult::FAILURE;
        }

        success = iplDirectEffectCreate(ipl_context, &ipl_audio_settings, &direct_settings, &direct_effect);

        if (success != IPL_STATUS_SUCCESS) {
            spdlog::error("Failed to setup direct effect");

            if (need_effects) {
                exit(-401);
            }
            return Tyche::AudioResult::FAILURE;
        }

        spdlog::info("Successfully initialized sound effects");
        return Tyche::AudioResult::SUCCESS;
    }

    void destroySoundEffects() {
        iplBinauralEffectRelease(&binaural_effect);
        iplDirectEffectRelease(&direct_effect);
    }
}

namespace MiniAudioUtils {

    void initializeMiniAudio() {
        engine_config = ma_engine_config_init();

        engine_config.channels = CHANNELS;
        engine_config.sampleRate = SAMPLE_RATE;
        engine_config.periodSizeInFrames = 256;

        result = ma_engine_init(&engine_config, &engine);
        if (result != MA_SUCCESS) {
            spdlog::error("Failed to initialize Miniaudio: {}", ma_result_description(result));
        }

        spdlog::info("Initialized Miniaudio engine");
    }

    void destroyMiniAudio() {
        ma_engine_uninit(&engine);
    }

    NodeConfig initNodeConfig(ma_uint32 channels) {
        NodeConfig config;

        MA_ZERO_OBJECT(&config);
        config.node_config = ma_node_config_init();
        config.channels_in = channels;

        return config;
    }

    void initializeSoundNode(SoundNode* sound_node, ma_node_graph* node_graph) {
        ma_node_config base_config;
        ma_uint32 channels_in = CHANNELS;
        ma_uint32 channels_out = 2;
        size_t heap_size;

        base_config = ma_node_config_init();
        base_config.vtable = &sound_node_VTable;
        base_config.pInputChannels = &channels_in;
        base_config.pOutputChannels = &channels_out;

        result = ma_node_init(node_graph, &base_config, nullptr, &sound_node->base_node);

        if (result != MA_SUCCESS) {
            spdlog::error("Could not initialize the Sound Node!");
            exit(-402);
        }

        heap_size = 0;

        heap_size += sizeof(float) * channels_out * ipl_audio_settings.frameSize;
        heap_size += sizeof(float) * channels_in * ipl_audio_settings.frameSize;

        sound_node->buffer._heap = ma_malloc(heap_size, nullptr);
        if (sound_node->buffer._heap == NULL) {
            ma_node_uninit(&sound_node->base_node, nullptr);

            spdlog::error("Failed to allocate Heap buffer for a Sound node");
            exit(-403);
        }

        sound_node->buffer.out_buffer[0] = (float*)sound_node->buffer._heap;
        sound_node->buffer.out_buffer[1] = (float*)ma_offset_ptr(sound_node->buffer._heap, sizeof(float) * ipl_audio_settings.frameSize);

        ma_uint32 ichannels_in;
        for (ichannels_in = 0; ichannels_in < channels_in; ichannels_in++) {
            sound_node->buffer.in_buffer[ichannels_in] = (float*) ma_offset_ptr(sound_node->buffer._heap, sizeof(float) * ipl_audio_settings.frameSize * (channels_out + ichannels_in));
        }

        spdlog::info("Successfully load sound");
    }
}

void Tyche::AudioDevice::initializeDevice(bool needEffects) {

    need_effects = needEffects;

    MiniAudioUtils::initializeMiniAudio();
    SteamAudioUtils::initializeSteamAudio();

    SteamAudioUtils::initializeSoundEffects();
}

void Tyche::AudioDevice::cleanUp() {

    for (auto ptr : _sound_ptrs) {

        ma_node_uninit(&ptr->node.base_node, nullptr);
        ma_free(ptr->node.buffer._heap, nullptr);

        delete ptr;

    }

    SteamAudioUtils::destroySoundEffects();
    SteamAudioUtils::destroySteamAudio();

    MiniAudioUtils::destroyMiniAudio();
}

Tyche::SoundID Tyche::AudioDevice::loadSound(const Tyche::STL::string &path) {
    id_counter++;

    SoundObject* new_sound = new SoundObject{};

    ma_sound_config sound_config = ma_sound_config_init();

    sound_config.pFilePath = path.c_str();
    sound_config.flags = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT;

    result = ma_sound_init_ex(&engine, &sound_config, &new_sound->sound);

    if (result != MA_SUCCESS) {
        spdlog::error("Could not load Sound: {}", ma_result_description(result));
        return -1;
    }

    ma_sound_set_directional_attenuation_factor(&new_sound->sound, 0);

    MiniAudioUtils::initializeSoundNode(&new_sound->node, ma_engine_get_node_graph(&engine));

    auto r = ma_node_attach_output_bus(&new_sound->node, 0, ma_engine_get_endpoint(&engine), 0);

    spdlog::info("Message {}", ma_result_description(r));

    r = ma_node_attach_output_bus(&new_sound->sound, 0, &new_sound->node, 0);

    spdlog::info("Message {}", ma_result_description(r));

    _sounds.put(id_counter, new_sound);
    _sound_ptrs.push_back(new_sound);

    return id_counter;
}



void Tyche::AudioDevice::playSound(Tyche::SoundID id, const Tyche::Math::Vector2 &position) {

    auto sound = _sounds.get(id)->sound;

    //ma_sound_set_volume(&sound, 2.0f);
    auto r = ma_sound_start(&sound);

    spdlog::info("Message {}", ma_result_description(r));

}

void Tyche::AudioDevice::updateListener(const Tyche::Math::Vector2 &pos) {

}
