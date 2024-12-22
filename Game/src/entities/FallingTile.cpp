//
// Created by allos on 12/21/2024.
//

#include "FallingTile.h"
#include "spdlog/spdlog.h"

void Entities::FallingTile::initialize(const Vector2 &position, Game::Room *room, char *tags) {
    RoomEntity::initialize(position, room, tags);

    _sprite = {3, 0};
    _scale = {25, 25};
}


void Entities::FallingTile::update() {
    spdlog::info("UPDATE");
}

void Entities::FallingTile::destroy() {

}
