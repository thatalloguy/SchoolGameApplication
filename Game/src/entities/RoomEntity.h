//
// Created by allos on 12/21/2024.
//

#pragma once

#include <core/Entity.h>
#include <math/vecs.h>
#include <stl/string.h>
#include <stl/vector.h>

using namespace Tyche::Math;
using namespace Tyche::STL;

namespace Game {
    struct Room;
}

namespace Entities {

    /*
     * The room entity is a very basic entity, meant for entities that are inside of rooms.
     * (which are basically every entity except the player)
     * It also handles tags, which the base entity class does not have.
     */

    class RoomEntity: public Tyche::Entity {

    public:

        RoomEntity() = default;
        virtual ~RoomEntity() = default;

        //Note Tags are a char[50] since that's alot easier to write to binary instead of a string.
        virtual void initialize(const Vector2& position, Game::Room* room, char tags[50]);

        virtual void update(float delta_time);
        virtual void destroy();

        virtual void reset();

        bool hasTag(const string& tag);

    protected:
        string _tag;
        Game::Room* _parent{};

    };


}
