//
// Created by allos on 11/29/2024.
//
#pragma once


/// Inspired by the SteamAudio implementation from DuckWatchers remake (also made by myself)
/// https://github.com/thatalloguy/GameProject/blob/main/Engine/Audio/AudioEngine.h


#include <phonon.h>
#include <miniaudio.h>

#ifndef AUDIO_SETTINGS

#define FORMAT ma_format_32
#define CHANNELS 2
#define SAMPLE_RATE 48000

#define AUDIO_SETTINGS 1

#endif

namespace Tyche {

    namespace AudioDevice {

        void initializeDevice();
        void cleanUp();

    }
}