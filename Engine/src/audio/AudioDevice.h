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

    // A struct to specify the configuration when creating a SoundEffect object
    struct SoundEffectConfig {
        ma_node_config nodeConfig;
        ma_uint32 channelsIn;
        IPLAudioSettings  audioSettings;
        IPLContext context;
        IPLHRTF HRTF;
    };


    // Struct That holds everything needed to apply IplEffects to ma_sound's
    struct SoundEffect {
        ma_node_base baseNode;

        // General Ipl objects
        IPLAudioSettings  audioSettings;
        IPLContext context;
        IPLHRTF HRTF;

        //Ipl Effects
        IPLBinauralEffect binauralEffect;
        IPLDirectEffect  directEffect;

        // How much the the soudn should be "directionalized"
        float spatialBlend = 1.0f;

        //Direction gets calculated in the update function
        Math::Vector2 direction{1, 1};
        Math::Vector2 soundPosition{1, 0};
        Math::Vector2 playerPosition{1, 0};

        //The Raw audio
        float* inBuffer[2];
        float* outBuffer[2];
        void* _heap;

    };

    struct SoundCreationInfo {
        const char* filePath = nullptr;
        bool shouldLoop = false;
        float spatialBlend = 1.0f;
    };

    //Simple object to hold both ma_sound and soundEffect
    struct SoundObject {
        ma_sound g_sound;
        SoundEffect soundEffect;
    };


    //Class to Load, Manager, Play and Destroy sounds.
    class AudioEngine {


    public:

        //Initializes both SteamAudio and MiniAudio
        void Init();
        // Destroys any allocated sounds and deinitializes SteamAudio and Miniaudio
        void CleanUp();

        // Loads and setups up a Sound and returns it respective ID.
        SoundID registerSound(SoundCreationInfo info);

        //Plays the sound at the position given, also adds it to the queue to update it later.
        void playSound(SoundID id, const Math::Vector2& pos, float volume=1.0f);

        void updateSound(SoundID id, const Math::Vector2& pos);

        //Updates all sounds positions and directions
        void update(const Math::Vector2& listenerPos);

        //Returns wether or not the ma_sound is playing
        bool isSoundPlaying(SoundID id);
    private:

        // lifetime functions
        void initializeSteamAudioObjects();
        void initializeMiniAudioObjects();

        void destroySteamAudioObjects();
        void destroyMiniAudioObjects();



        // helper functions

        //Applies the direct and binural effect to the ma_sound
        static void soundEffectProccessPCMFrames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut);

        //Returns a generic SoundEffectConfig.
        SoundEffectConfig initSoundEffectConfig(ma_uint32 channelsIn, IPLAudioSettings audioSettings, IPLContext context, IPLHRTF hrtf);

        // Initializes the effect objects and raw audio buffers.
        ma_result initSoundEffect(ma_node_graph* nodeGraph, const SoundEffectConfig* config, const ma_allocation_callbacks* allocationCallbacks, SoundEffect& soundEffect);

        // Destroys and cleans up all allocated memory
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
        STL::vector<SoundObject*> _objects; // have a seperate vector, since the hashmap doesnt support iterating yet.

        Math::Vector2 _listener_pos{0, 0};

        STL::vector<SoundObject*> _queue;
        unsigned int idCounter = 1;

    };

}