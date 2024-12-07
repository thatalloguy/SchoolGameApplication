//
// Created by allos on 12/7/2024.
//

#pragma once
#include <cstdio>
#include <stl/vector.h>


namespace Tyche::IO {

    template<typename T>
    void saveVectorToFile(const char * path, STL::vector<T>& vec) {

        FILE* f_out = fopen(path, "w");

        fwrite(&vec.length(), sizeof(int), 1, f_out);
        for (auto obj : vec) {
            fwrite(&obj, sizeof(T), 1, f_out);
        }

        fclose(f_out);
    }

    template<typename T>
    void loadVectorFromFile(const char* path, STL::vector<T>& out ) {
        FILE* f_in = fopen(path, "r");

        int size = 0;
        fread(&size, sizeof(int), 1, f_in);

        for (int i=0; i<size; i++) {
            T new_obj{};

            fread(&new_obj, sizeof(T), 1, f_in);
            out.push_back(new_obj);
        }

        fclose(f_in);
    }

}