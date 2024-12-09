//
// Created by allos on 12/5/2024.
//
#pragma once
#include "core/Entity.h"

namespace MapEntities {

// Just a simple Entity to draw a sprite at the mouse position
class Cursor: public Tyche::Entity {

public:
    void initialize() override;
    void update() override;
    void prepareRendering() override;
    void destroy() override;

    void updateSprite(const Tyche::Math::Vector2& sprite);
};


}
