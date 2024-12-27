//
// Created by allos on 11/19/2024.
//

#include "core/App.h"



class Foo {
public:
    float a = 0.1f;
};


class Fool: public Foo {
public:
    float b = 0.1f;
};

int main(int argc, char** argv) {

    Game::App app{argc, argv};


    app.run();


    return 0;
}