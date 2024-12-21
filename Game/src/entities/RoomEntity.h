//
// Created by allos on 12/21/2024.
//
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

    typedef unsigned int RoomObjectId;

    class RoomEntity: public Tyche::Entity {

    public:
        virtual void initialize(const Vector2& position, Game::Room* room, RoomObjectId id, char tags[50]);

        bool hasTag(const string& tag);

    protected:
        vector<string> _tags;
        Game::Room* _parent;
        RoomObjectId _id = 0;

    private:
        void loadTagsFromSingleString(char tags[50]);
    };


}
