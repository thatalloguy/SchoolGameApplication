//
// Created by allos on 12/9/2024.
//

#include "RoomManager.h"

#include "core/IO.h"

Game::RoomManager::RoomManager(Tyche::TileRenderer& tile_renderer, Tyche::DebugRenderer& debug_renderer) {
    _rooms.reserve(3);
    _tile_renderer = &tile_renderer;
    _debug_renderer = &debug_renderer;
}

Game::RoomManager::~RoomManager() {
    for (auto room: _rooms) {
        destroyRoom(room);
    }
}

void Game::RoomManager::loadRoom(const char* path) {

    Room* new_room = new Room{};

    FILE* file = fopen(path, "r");

    Tyche::IO::loadVectorFromFile<Tyche::Tile*, Tyche::Tile>(path, new_room->tiles, file, false);
    Tyche::IO::loadVectorFromFile<Vector4>(path, new_room->colliders, file, false);

    loadTiles(*new_room);
    setupCollision(*new_room);

    spdlog::info("Loaded Room from disk: {}", path);

    fclose(file);
}

void Game::RoomManager::update() {

}

Tyche::World* Game::RoomManager::getWorld() {
    return &_world;
}

void Game::RoomManager::loadTiles(Room& room) {

    for (auto tile : room.tiles) {
        _tile_renderer->addTile(*tile);
    }

}

void Game::RoomManager::setupCollision(Room& room) {

    for (auto collider: room.colliders) {
        _world.addStaticBody(new Tyche::PhysicsObject{collider, Tyche::ObjectType::Static});
    }
}

void Game::RoomManager::destroyRoom(Room* room) {

    for (Tyche::Tile* tile: room->tiles) {
        delete tile;
    }

    delete room;
}

