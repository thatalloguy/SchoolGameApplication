//
// Created by allos on 11/29/2024.
//
#pragma once


/// A modified version of the audio engine from Duck Watchers remake (also made by myself)
/// https://github.com/thatalloguy/GameProject/blob/main/Engine/Audio/AudioEngine.h



#include "math/vecs.h"
#include "stl/string.h"
#include "stl/hashmap.h"
#include "stl/vector.h"


#ifndef AUDIO_SETTINGS

#define FORMAT ma_format_f32
#define CHANNELS 2
#define SAMPLE_RATE 48000

#define AUDIO_SETTINGS 1

#endif


#include <miniaudio.h>


#include <phonon.h>

#define FORMAT ma_format_32
#define CHANNELS 2
#define SAMPLE_RATE 48000

namespace Tyche {

    typedef unsigned int SoundID;

    struct SoundEffectConfig {
        ma_node_config nodeConfig;
        ma_uint32 channelsIn;
        IPLAudioSettings  audioSettings;
        IPLContext context;
        IPLHRTF HRTF;
    };



    struct SoundEffect {
        ma_node_base baseNode;
        IPLAudioSettings  audioSettings;
        IPLContext context;
        IPLHRTF HRTF;
        IPLBinauralEffect binauralEffect;
        IPLDirectEffect  directEffect;
        float spatialBlend = 1.0f;
        Math::Vector2 direction{1, 1};
        Math::Vector2 soundPosition{1, 0};
        Math::Vector2 playerPosition{1, 0};
        float* inBuffer[2];
        float* outBuffer[2];
        void* _heap;

    };

    struct SoundCreationInfo {
        const char* filePath = nullptr;
        bool shouldLoop = false;
        float spatialBlend = 1.0f;
        //todo add  steamaudio options here?
    };

    struct SoundObject {
        ma_sound g_sound;
        SoundEffect soundEffect;
    };


    class AudioEngine {


    public:

        void Init();
        void CleanUp();

        SoundID registerSound(SoundCreationInfo info);

        void playSound(SoundID id, const Math::Vector2& pos, float volume=1.0f);

        void update(const Math::Vector2& listenerPos);
        bool isSoundPlaying(SoundID id);
    private:

        // lifetime functions
        void initializeSteamAudioObjects();
        void initializeMiniAudioObjects();

        void destroySteamAudioObjects();
        void destroyMiniAudioObjects();



        // helper functions
        static void soundEffectProccessPCMFrames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut);

        SoundEffectConfig initSoundEffectConfig(ma_uint32 channelsIn, IPLAudioSettings audioSettings, IPLContext context, IPLHRTF hrtf);
        ma_result initSoundEffect(ma_node_graph* nodeGraph, const SoundEffectConfig* config, const ma_allocation_callbacks* allocationCallbacks, SoundEffect& soundEffect);

        void destroySoundEffect(SoundEffect* binauralEffect, const ma_allocation_callbacks * allocationCallbacks);

        ma_node_vtable soundNodeVTable {
                AudioEngine::soundEffectProccessPCMFrames,
                NULL,
                1,
                1,
                0
        };

        //MiniAudio Objects
        ma_result result;
        ma_engine engine;
        ma_engine_config    engineConfig;

        // SteamAudio objects
        IPLAudioSettings    iplAudioSettings;
        IPLContextSettings  iplContextSettings{};
        IPLContext          iplContext = nullptr;
        IPLHRTFSettings     iplhrtfSettings;
        IPLHRTF             iplHRTF;


        STL::hashmap<SoundID, SoundObject*> registry;
        STL::vector<SoundObject*> _objects;
        STL::vector<SoundObject*> _queue;
        unsigned int idCounter = 1;

    };

}