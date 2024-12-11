//
// Created by allos on 12/11/2024.
//
#pragma once
#include "core/Entity.h"

namespace MapEntities {


    class DebugBox: public Tyche::Entity {

    public:
        void initialize() override;
        void update() override;
        void prepareRendering() override;
        void destroy() override;

        void setBeginPos(const Tyche::Math::Vector2& pos);
        void setEndPos(const Tyche::Math::Vector2& pos);

    private:
        Tyche::Math::Vector4 _AABB{0, 0, 0, 0};

    };

}