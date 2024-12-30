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
    destroyRoom(&_current_room);


    //We have to delete each string since we allocate it.
    for (auto room_weight : _room_weights) {
        delete room_weight.second;
    }

}

void Game::RoomManager::loadRoom(const char* path) {

    // The directory iterator doesnt give the full path so we need to correct it.
    string full_path = "../../../Rooms/";
    full_path = full_path + path;

    RawRoom new_raw_room{};

    // read the file
    FILE* file = fopen(full_path.c_str(), "r");

    //Load everything into the raw room.
    Tyche::IO::loadVectorFromFile<Tyche::Tile*, Tyche::Tile>(full_path.c_str(), new_raw_room.tiles, file, false);
    Tyche::IO::loadVectorFromFile<EntityBlueprint*, EntityBlueprint>(full_path.c_str(), new_raw_room.entities, file, false);
    Tyche::IO::loadVectorFromFile<Vector4>(full_path.c_str(), new_raw_room.colliders, file, false);

    loadTiles(new_raw_room);
    setupCollision(new_raw_room);

    spdlog::info("Loaded Room from disk: {}", full_path.c_str());

    fclose(file);

    //Update the current room.
    _current_room.tiles = new_raw_room.tiles;
    _current_room.colliders = new_raw_room.colliders;
    _current_room.parent = this;

    loadEntities(new_raw_room, _current_room);

}

void Game::RoomManager::update(float delta_time) {

    //If the player falls out of the map we need to reset the map.
    if (_player->getPosition().getY() >= max_y) {
        resetCurrentRoom();
        spdlog::info("Fell out of the map");
    }


    for (Entities::RoomEntity* entity: _current_room.entities) {

        // If the room has loaded via the last entity
        // it means that the room is outdated, so we just return out of the function.
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

        if (tile->position.getY() > max_y) {
            max_y = tile->position.getY();
        }
    }

    //Update the tile renderer
    _tile_renderer->prepareRendering();

}

void Game::RoomManager::setupCollision(RawRoom& room) {

    for (auto collider: room.colliders) {
        // we can allocate a new collider since the world will handle the memory.
        _world.addStaticBody(new Tyche::PhysicsObject{collider, Tyche::ObjectType::Static});
    }
}

void Game::RoomManager::destroyRoom(Room* room) {

    //Delete all allocated memory and clear all the vectors.

    for (Tyche::Tile* tile: room->tiles) {
        delete tile;
    }

    for (auto entity : room->entities) {
        delete entity;
    }

    _world.clearAllStaticBodies();
    _tile_renderer->clearTiles();

    // set the max_y to 100 as a failsafe.
    max_y = 100.0f;

    _current_room.tiles.clear();
    _current_room.entities.clear();
    _current_room.colliders.clear();

    spdlog::info("Destroyed room");
}

void Game::RoomManager::loadEntities(Game::RawRoom &in, Room& out) {

    for (auto blueprint : in.entities) {

        if (_registry.has(blueprint->type)) {
            // the creation function
            auto create_entity = _registry.get(blueprint->type);

            // Create a new entity but with the correct constructor.
            auto new_entity = create_entity();

            new_entity->initialize(blueprint->position, &out, blueprint->tags);

            out.entities.push_back(new_entity);
            _entity_renderer->addEntity(new_entity);

            // If the entity is the start of the level.
            if (new_entity->hasTag("start")) {
                out.start = new_entity->getPosition();
            }

        } else {
            spdlog::error("No Entity with TypeId {} is registered!", blueprint->type);
        }
    }

}

void Game::RoomManager::loadRoomList(const char *directory_path, const char* seed) {
    int i = 0;
    for (const auto & entry : std::filesystem::directory_iterator(directory_path)) {
        if (!std::filesystem::is_directory(entry.path())) {

            // if we are outside the seed reset the counter.
            if (i > strlen(seed)) {
                i = 0;
            }

            // convert the char to a weight.
            // which we do by converting it to a int and subtract 64 from it.
            // This gives us a value between 1-26
            // then we divide by 26 to get a value from 0.0f to 1.0f.
            char a = seed[i];
            int letter_pos = a - 64;

            float weight = 1.0f + ((float) letter_pos / 26);

            _room_weights.push_back({weight, new string{entry.path().filename().string().data()}});

            auto room = _room_weights.back();

            // Always prefer the highest weight
            if (_next_room.first <= room.first) {
                _next_room = room;
            }

            spdlog::info("Room {} {}", room.first, room.second->c_str());

            i++;
        }
    }
}

void Game::RoomManager::goToNextRoom() {

    // ignore the call if we already loaded a room last frame.
    if (_is_outdated) {
        return;
    }

    // make the current room to the lowest weight so that we dont have the same room twice in a row.
    _next_room.first = -0.1f;

    for (auto room_weight: _room_weights) {
        // increase the weight
        room_weight.first += 0.1f;

        if (room_weight.first >= _next_room.first && room_weight.second != _next_room.second) {
            _next_room = room_weight;
        }
    }

    // destroy the room
    destroyRoom(&_current_room);

    // Add the player's rigidbody to it again.
    _world.addRigidBody(&_player->getBody());

    // then load the next room
    loadRoom(_next_room.second->c_str());


    // teleport player to the start portal
    _player->setPosition(_current_room.start);

    _is_outdated = true;
}

void Game::RoomManager::loadStartRoom() {
    if (_next_room.second != nullptr) {
        loadRoom(_next_room.second->c_str());
    } else {
        spdlog::error("No Next room found!");
    }
}

void Game::RoomManager::resetCurrentRoom() {

    spdlog::info("resetting map");

    for (auto entity : _current_room.entities) {
        entity->reset();
    }

    _player->setPosition( _current_room.start);
    _player->getBody().setVelocity({0, 0});
}
