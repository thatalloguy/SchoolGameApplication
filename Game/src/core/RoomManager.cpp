//
// Created by allos on 12/9/2024.
//

#include "RoomManager.h"

#include "core/IO.h"

Game::RoomManager::RoomManager(Tyche::TileRenderer& tile_renderer, Tyche::EntityRenderer& entity_renderer, Tyche::DebugRenderer& debug_renderer) {
    _rooms.reserve(3);
    _tile_renderer = &tile_renderer;
    _debug_renderer = &debug_renderer;
    _entity_renderer = &entity_renderer;
}

Game::RoomManager::~RoomManager() {
    for (auto room: _rooms) {
        destroyRoom(room);
    }
}

void Game::RoomManager::loadRoom(const char* path) {

    RawRoom* new_raw_room = new RawRoom{};
    Room* new_room = new Room{};

    FILE* file = fopen(path, "r");

    Tyche::IO::loadVectorFromFile<Tyche::Tile*, Tyche::Tile>(path, new_raw_room->tiles, file, false);
    Tyche::IO::loadVectorFromFile<EntityBlueprint*, EntityBlueprint>(path, new_raw_room->entities, file, false);
    Tyche::IO::loadVectorFromFile<Vector4>(path, new_raw_room->colliders, file, false);

    loadTiles(*new_raw_room);
    setupCollision(*new_raw_room);

    spdlog::info("Loaded Room from disk: {}", path);

    fclose(file);

    new_room->tiles = new_raw_room->tiles;
    new_room->colliders = new_raw_room->colliders;
    new_room->parent = this;

    loadEntities(*new_raw_room, *new_room);

    delete new_raw_room;

    _rooms.push_back(new_room);
}

void Game::RoomManager::update(float delta_time) {
    for (auto room: _rooms) {
        for (Entities::RoomEntity* entity: room->entities) {
            entity->update(delta_time);
        }
    }
}

Tyche::World* Game::RoomManager::getWorld() {
    return &_world;
}

void Game::RoomManager::loadTiles(RawRoom& room) {

    for (auto tile : room.tiles) {
        _tile_renderer->addTile(*tile);
    }



}

void Game::RoomManager::setupCollision(RawRoom& room) {

    for (auto collider: room.colliders) {
        _world.addStaticBody(new Tyche::PhysicsObject{collider, Tyche::ObjectType::Static});
    }
}

void Game::RoomManager::destroyRoom(Room* room) {

    for (Tyche::Tile* tile: room->tiles) {
        delete tile;
    }

    for (auto entity : room->entities) {
        entity->destroy();

        delete entity;
    }

    delete room;
}

void Game::RoomManager::loadEntities(Game::RawRoom &in, Room& out) {

    for (auto blueprint : in.entities) {

        if (_registry.has(blueprint->type)) {
            auto create_entity = _registry.get(blueprint->type);

            auto new_entity = create_entity();

            new_entity->initialize(blueprint->position, &out, blueprint->tags);

            out.entities.push_back(new_entity);
            _entity_renderer->addEntity(new_entity);

        } else {
            spdlog::error("No Entity with TypeId {} is registered!", blueprint->type);
        }

        delete blueprint;
    }

}
