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

namespace Game {

    struct EntityBlueprint {
        int type = 0;
        //tags are seperated by coma's
        char tags[50] = {" "};
        Vector2 position = {0,0};
    };


    struct RawRoom {
        vector<Tyche::Tile*> tiles;
        vector<EntityBlueprint*> entities;
        vector<Vector4> colliders;
    };

    struct Room {
        vector<Tyche::Tile*> tiles;
        vector<Entities::RoomEntity*> entities;
        vector<Vector4> colliders;

        void* parent = nullptr;
    };

    class RoomManager {

    public:
        RoomManager(Tyche::TileRenderer& tile_renderer, Tyche::EntityRenderer& entity_renderer, Tyche::DebugRenderer& debug_renderer);
        ~RoomManager();

        template<typename T>
        void registerEntity(int type_id) {
            _registry.put(type_id, RoomManager::createEntity<T>);
        }

        void loadRoom(const char* path);

        void update(float delta_time);

        Tyche::World* getWorld();

        Tyche::DebugRenderer* getDebugRenderer() { return _debug_renderer; };

    private:

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

        vector<Room*> _rooms;
        hashmap<int, Entities::RoomEntity*(*)()> _registry;
        Tyche::World _world{true};
    };


}
