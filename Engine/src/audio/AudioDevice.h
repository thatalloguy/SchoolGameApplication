//
// Created by allos on 11/29/2024.
//
#pragma once


/// Inspired by the SteamAudio implementation from DuckWatchers remake (also made by myself)
/// https://github.com/thatalloguy/GameProject/blob/main/Engine/Audio/AudioEngine.h



#include "math/vecs.h"
#include "stl/string.h"




#ifndef AUDIO_SETTINGS

#define FORMAT ma_format_f32
#define CHANNELS 2
#define SAMPLE_RATE 48000

#define AUDIO_SETTINGS 1

#endif

namespace Tyche {

    typedef unsigned int SoundID;

    struct AudioBuffer {
        float* in_buffer[2];
        float* out_buffer[2];
        void* _heap;
    };

    enum class AudioResult: unsigned int {
        SUCCESS = 0,
        FAILURE = 1,
    };

    namespace AudioDevice {

        // need_effects determines if the program should exit if we fail to load any effects.
        // made it false by default since we rather have sound with no effects and the game works, then the game  just straight up doesn't work
        void initializeDevice(bool need_effects=false);

        void cleanUp();


        SoundID loadSound(const STL::string& path);

        void playSound(SoundID id, const Math::Vector2& position);

        void updateListener(const Math::Vector2& pos);



    }
}