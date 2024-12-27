//
// Created by allos on 12/9/2024.
//

#include <filesystem>
#include "RoomManager.h"

#include "core/IO.h"

Game::RoomManager::RoomManager(Tyche::TileRenderer& tile_renderer, Tyche::EntityRenderer& entity_renderer,
                               Tyche::DebugRenderer& debug_renderer, Entities::Player& player) {

    _tile_renderer = &tile_renderer;
    _debug_renderer = &debug_renderer;
    _entity_renderer = &entity_renderer;
    _player = &player;
}

Game::RoomManager::~RoomManager() {
    destroyRoom(_current_room);


    for (auto room_weight : _room_weights) {
        delete room_weight.second;
    }

}

void Game::RoomManager::loadRoom(const char* path) {

    string full_path = "../../../Rooms/";
    full_path = full_path + path;

    RawRoom* new_raw_room = new RawRoom{};
    Room* new_room = new Room{};

    FILE* file = fopen(full_path.c_str(), "r");

    Tyche::IO::loadVectorFromFile<Tyche::Tile*, Tyche::Tile>(full_path.c_str(), new_raw_room->tiles, file, false);
    Tyche::IO::loadVectorFromFile<EntityBlueprint*, EntityBlueprint>(full_path.c_str(), new_raw_room->entities, file, false);
    Tyche::IO::loadVectorFromFile<Vector4>(full_path.c_str(), new_raw_room->colliders, file, false);

    loadTiles(*new_raw_room);
    setupCollision(*new_raw_room);

    spdlog::info("Loaded Room from disk: {}", full_path.c_str());

    fclose(file);

    new_room->tiles = new_raw_room->tiles;
    new_room->colliders = new_raw_room->colliders;
    new_room->parent = this;

    loadEntities(*new_raw_room, *new_room);

    delete new_raw_room;

    _current_room = new_room;
}

void Game::RoomManager::update(float delta_time) {

    for (Entities::RoomEntity* entity: _current_room->entities) {
        if (_is_outdated) {
            _is_outdated = false;
            return;
        }
        entity->update(delta_time);
    }

}

Tyche::World* Game::RoomManager::getWorld() {
    return &_world;
}

void Game::RoomManager::loadTiles(RawRoom& room) {

    for (auto tile : room.tiles) {
        _tile_renderer->addTile(*tile);
    }

    _tile_renderer->prepareRendering();

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
    _tile_renderer->clearTiles();

    for (auto entity : room->entities) {
        entity->destroy();

        delete entity;
    }

    _world.clearAllStaticBodies();

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

void Game::RoomManager::loadRoomList(const char *directory_path, const char* seed) {
    int i = 0;
    for (const auto & entry : std::filesystem::directory_iterator(directory_path)) {

        if (i>strlen(seed)) {
            i=0;
        }

        char a = seed[i];
        int letter_pos = a - 64;

        float weight = 1.0f + ((float) letter_pos / 26);

        _room_weights.push_back({weight, new string{entry.path().filename().string().data()}});

        auto room = _room_weights.back();

        if (_next_room.first <= room.first) {
            _next_room = room;
        }

        spdlog::info("Room {} {}", room.first, room.second->c_str());

        i++;
    }
}

void Game::RoomManager::goToNextRoom() {

    if (_is_outdated) {
        return;
    }

    _next_room.first = -0.1f;

    for (auto room_weight: _room_weights) {
        room_weight.first += 0.1f;

        if (room_weight.first >= _next_room.first && room_weight.second != _next_room.second) {
            _next_room = room_weight;
        }
    }

    // go to next room :D

    destroyRoom(_current_room);

    _world.addRigidBody(&_player->getBody());

    loadRoom(_next_room.second->c_str());


    // teleport player to the start portal
    for (auto entity : _current_room->entities) {
        if (entity->hasTag("start")) {
            _player->setPosition(entity->getPosition());
        }
    }

    _is_outdated = true;
}

void Game::RoomManager::loadStartRoom() {
    loadRoom(_next_room.second->c_str());
}
