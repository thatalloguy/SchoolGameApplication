//
// Created by allos on 11/19/2024.
//

#pragma once

#include <string>
#include <fstream>
#include "spdlog/spdlog.h"

// This file is used to define simple types and or structs.
typedef unsigned int uint;
typedef const char* cstr;

//just a very simple struct to represent a GPU mesh.
struct GPUMesh {
    //VAO = vertex array buffer
    unsigned int VAO = 0;
    //VBO = vertex buffer object
    unsigned int VBO = 0;
};

namespace Utils {

    static float quadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    static std::string loadTxtFromFile(const char* path){
        std::ifstream in(path, std::ios::binary);
        if (in)
        {
            //Read the contents of the file
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());

            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.length());

            in.close();
            // Give
            return contents;
        }
        spdlog::error("Couldn't load: ", path);
        throw(errno);
    }
}