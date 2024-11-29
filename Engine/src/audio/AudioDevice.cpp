//
// Created by allos on 11/29/2024.
//

#include "AudioDevice.h"


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


    //Global Miniaudio objects
    ma_result result;
    ma_engine engine;
    ma_engine_config engine_config;


}


namespace SteamAudioUtils {
    void initializeSteamAudio() {
        MA_ZERO_OBJECT(&ipl_audio_settings);

        ipl_audio_settings.samplingRate = (int) ma_engine_get_sample_rate(&engine);
        ipl_audio_settings.frameSize = (int) engine_config.periodSizeInFrames;

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
}

void Tyche::AudioDevice::initializeDevice() {
    MiniAudioUtils::initializeMiniAudio();
    SteamAudioUtils::initializeSteamAudio();
}

void Tyche::AudioDevice::cleanUp() {
    SteamAudioUtils::destroySteamAudio();
    MiniAudioUtils::destroyMiniAudio();
}
