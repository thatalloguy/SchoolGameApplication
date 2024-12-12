//
// Created by allos on 12/7/2024.
//

#pragma once
#include <cstdio>
#include <stl/vector.h>


namespace Tyche::IO {

    template<typename T, typename N>
    void saveVectorToFile(const char * path, STL::vector<T>& vec, FILE* file = nullptr, bool auto_close_file = true) {

        if (file == nullptr)
            file = fopen(path, "w");



        int size = vec.length();

        fwrite(&size, sizeof(int), 1, file);
        for (T obj_ptr : vec) {
            N obj = *obj_ptr;
            fwrite(&obj, sizeof(N), 1, file);
        }

        if (auto_close_file)
            fclose(file);

    }

    template<typename T>
    void saveVectorToFile(const char * path, STL::vector<T>& vec, FILE* file = nullptr, bool auto_close_file = true) {

        if (file == nullptr)
            file = fopen(path, "w");



        int size = vec.length();

        fwrite(&size, sizeof(int), 1, file);
        for (T obj : vec) {
            fwrite(&obj, sizeof(T), 1, file);
        }

        if (auto_close_file)
            fclose(file);

    }

    template<typename T, typename N>
    void loadVectorFromFile(const char* path, STL::vector<T>& out, FILE* file = nullptr, bool auto_close_file = true) {

        if (file == nullptr)
            file = fopen(path, "r");

        int size = 0;
        fread(&size, sizeof(int), 1, file);

        for (int i=0; i<size; i++) {
            N* new_obj = new N{};

            fread(new_obj, sizeof(N), 1, file);

            out.push_back(new_obj);
        }


        if (auto_close_file)
            fclose(file);
    }

    template<typename T>
    void loadVectorFromFile(const char* path, STL::vector<T>& out, FILE* file = nullptr, bool auto_close_file = true) {

        if (file == nullptr)
            file = fopen(path, "r");

        int size = 0;
        fread(&size, sizeof(int), 1, file);

        for (int i=0; i<size; i++) {
            T new_obj{};

            fread(&new_obj, sizeof(T), 1, file);

            out.push_back(new_obj);
        }


        if (auto_close_file)
            fclose(file);
    }

}