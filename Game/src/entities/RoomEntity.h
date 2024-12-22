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
        ~RoomEntity() = default;

        virtual void initialize(const Vector2& position, Game::Room* room, char tags[50]);

        virtual void update();
        virtual void destroy();

        bool hasTag(const string& tag);

    protected:
        vector<string> _tags;
        Game::Room* _parent{};

    private:
        void loadTagsFromSingleString(char tags[50]);
    };


}
