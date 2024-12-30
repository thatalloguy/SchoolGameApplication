//
// Created by allos on 12/9/2024.
//

#pragma once
#include "math/vecs.h"
#include "physics/PhysicsHandler.h"
#include "renderer/DebugRenderer.h"
#include "renderer/TileRenderer.h"
#include "stl/vector.h"
#include "../entities/RoomEntity.h"
#include "stl/hashmap.h"
#include "renderer/EntityRenderer.h"
#include "audio/AudioDevice.h"
#include "stl/pair.h"
#include "../entities/Player.h"

namespace Game {

    struct EntityBlueprint {
        int type = 0;
        //tags are seperated by coma's
        char tags[50] = {" "};
        Vector2 position = {0,0};
    };

    //The struct which are saved in binary in the room files
    struct RawRoom {
        vector<Tyche::Tile*> tiles;
        vector<EntityBlueprint*> entities;
        vector<Vector4> colliders;
    };

    // Actual game ready Room structs
    struct Room {
        vector<Tyche::Tile*> tiles;
        vector<Entities::RoomEntity*> entities;
        vector<Vector4> colliders;

        Vector2 start{0, 0};

        // Parent is the RoomManager.
        void* parent = nullptr;
    };


    class RoomManager {

    public:
        RoomManager(Tyche::TileRenderer& tile_renderer, Tyche::EntityRenderer& entity_renderer,
                    Tyche::DebugRenderer& debug_renderer, Entities::Player& player);
        ~RoomManager();

        template<typename T>
        void registerEntity(int type_id) {
            _registry.put(type_id, RoomManager::createEntity<T>);
        }

        void loadRoom(const char* path);
        void loadRoomList(const char* directory_path, const char* seed);

        void goToNextRoom();
        void loadStartRoom();
        void resetCurrentRoom();

        void update(float delta_time);

        Tyche::World* getWorld();

        Tyche::DebugRenderer* getDebugRenderer() { return _debug_renderer; };
        Tyche::AudioEngine* getAudioEngine() { return _audio_engine; };
        void setAudioEngine(Tyche::AudioEngine* audio_engine) { _audio_engine = audio_engine; };

    private:

        //We need this function / way to create entities.
        // So that we use / instantiate the correct constructor.
        template<typename T>
        static Entities::RoomEntity* createEntity() {
            T* inst = new T{};

            Entities::RoomEntity* ref = (Entities::RoomEntity*) inst;

            return ref;
        }

        void loadTiles(RawRoom& room);
        void setupCollision(RawRoom& room);
        void loadEntities(RawRoom& in, Room& out);

        void destroyRoom(Room* room);

        Tyche::TileRenderer* _tile_renderer = nullptr;
        Tyche::EntityRenderer* _entity_renderer = nullptr;
        Tyche::DebugRenderer* _debug_renderer = nullptr;
        Tyche::AudioEngine*     _audio_engine = nullptr;

        Entities::Player* _player = nullptr;
        Tyche::World _world{true};

        //Float is the room weight amd the string is the path to the room.
        Pair<float, string*> _next_room{0.0f, nullptr};
        // Holds a list of paths with weights.
        vector<Pair<float, string*>> _room_weights;
        Room _current_room{};

        bool _is_outdated = false;
        float max_y = 0.0f;

        // a registry with function ptrs to creation funcs
        hashmap<int, Entities::RoomEntity*(*)()> _registry;
    };


}
