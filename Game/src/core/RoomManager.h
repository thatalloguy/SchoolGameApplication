//
// Created by allos on 12/9/2024.
//

#pragma once
#include "math/vecs.h"
#include "physics/PhysicsHandler.h"
#include "renderer/DebugRenderer.h"
#include "renderer/TileRenderer.h"
#include "stl/vector.h"

namespace Game {

    struct Room {
        vector<Tyche::Tile*> tiles;
        vector<Vector4> colliders;
    };


    class RoomManager {

    public:
        RoomManager(Tyche::TileRenderer& tile_renderer, Tyche::DebugRenderer& debug_renderer);
        ~RoomManager();

        void loadRoom(const char* path);

        void update();

        Tyche::World* getWorld();

    private:
        void loadTiles(Room& room);
        void setupCollision(Room& room);

        void destroyRoom(Room* room);

        Tyche::TileRenderer* _tile_renderer = nullptr;
        Tyche::DebugRenderer* _debug_renderer = nullptr;

        vector<Room*> _rooms;
        Tyche::World _world{true};
    };


}
