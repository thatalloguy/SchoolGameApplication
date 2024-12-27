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

    class RoomEntity: public Tyche::Entity {

    public:

        RoomEntity() = default;
        virtual ~RoomEntity() = default;

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
