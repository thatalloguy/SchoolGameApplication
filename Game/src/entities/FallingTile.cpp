//
// Created by allos on 12/21/2024.
//

#include "FallingTile.h"

void Entities::FallingTile::initialize(const Vector2 &position, Game::Room *room, Entities::RoomObjectId id, char *tags) {
    RoomEntity::initialize(position, room, id, tags);

    _sprite = {3, 0};
    _scale = {25, 25};
}


void Entities::FallingTile::update() {
}

void Entities::FallingTile::destroy() {

}
