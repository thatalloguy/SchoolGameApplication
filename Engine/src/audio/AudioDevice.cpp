//
// Created by allos on 11/29/2024.
//

#include "AudioDevice.h"
#include "stl/hashmap.h"

#include <phonon.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <spdlog/spdlog.h>

namespace {

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
    Tyche::STL::hashmap<unsigned int, ma_sound> _sounds;


    //Global Structs
    struct NodeConfig {
        ma_node_config node_config;
        ma_uint32 channels_in;
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
}

void Tyche::AudioDevice::initializeDevice(bool needEffects) {

    need_effects = needEffects;

    MiniAudioUtils::initializeMiniAudio();
    SteamAudioUtils::initializeSteamAudio();

    SteamAudioUtils::initializeSoundEffects();
}

void Tyche::AudioDevice::cleanUp() {
    SteamAudioUtils::destroySteamAudio();
    MiniAudioUtils::destroyMiniAudio();
}

Tyche::SoundID Tyche::AudioDevice::loadSound(const Tyche::STL::string &path) {
    id_counter++;

    ma_sound new_sound{};

    ma_sound_config sound_config = ma_sound_config_init();

    sound_config.pFilePath = path.c_str();
    sound_config.flags = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT;

    result = ma_sound_init_ex(&engine, &sound_config, &new_sound);

    if (result != MA_SUCCESS) {
        spdlog::error("Could not load Sound: {}", ma_result_description(result));
        return -1;
    }

    ma_sound_set_directional_attenuation_factor(&new_sound, 0);


    _sounds.put(id_counter, new_sound);
}



void Tyche::AudioDevice::playSound(Tyche::SoundID id, const Tyche::Math::Vector2 &position) {

    auto sound = _sounds.get(id);

    ma_sound_start(&sound);


}

void Tyche::AudioDevice::updateListener(const Tyche::Math::Vector2 &pos) {

}
