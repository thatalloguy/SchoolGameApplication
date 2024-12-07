//
// Created by allos on 12/7/2024.
//

#pragma once
#include <cstdio>
#include <stl/vector.h>


namespace Tyche::IO {

    template<typename T, typename N>
    void saveVectorToFile(const char * path, STL::vector<T>& vec) {

        FILE* f_out = fopen(path, "w");

        int size = vec.length();

        fwrite(&size, sizeof(int), 1, f_out);
        for (T obj_ptr : vec) {
            N obj = *obj_ptr;
            printf("POS %f \n", obj.position[0]);
            fwrite(&obj, sizeof(N), 1, f_out);
        }

        fclose(f_out);
    }

    template<typename T, typename N>
    void loadVectorFromFile(const char* path, STL::vector<T>& out ) {
        FILE* f_in = fopen(path, "r");

        int size = 0;
        fread(&size, sizeof(int), 1, f_in);

        for (int i=0; i<size; i++) {
            N* new_obj = new N{};

            fread(new_obj, sizeof(N), 1, f_in);

            out.push_back(new_obj);
        }

        fclose(f_in);
    }

}