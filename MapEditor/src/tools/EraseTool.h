//
// Created by allos on 12/5/2024.
//
#pragma once

#include "Tool.h"

namespace Tools {


    class EraseTool: public Tools::Tool {

    public:
        void initialize() override;
        void update() override;
        void destroy() override;
    };

}
