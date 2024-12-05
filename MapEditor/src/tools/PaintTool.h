//
// Created by allos on 12/5/2024.
//
#pragma once

#include "Tool.h"

namespace Tools {


    class PaintTool: public Tools::Tool {

    public:
        void initialize(MapEditor::App& editor_instance) override;
        void update() override;
        void destroy() override;
    };

}
