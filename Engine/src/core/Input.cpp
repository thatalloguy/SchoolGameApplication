//
// Created by allos on 11/21/2024.
//

#include "Input.h"

#include "stl/hashmap.h"
#include "stl/vector.h"

namespace Tyche::Input {

    hashmap<const Action,vector<Key>*, StringHashFunc> _key_map;
    GLFWwindow* _target_window;

}


void Tyche::Input::setTargetWindow(Window& window) {
    _target_window = (GLFWwindow*) window.getRawWindowPtr();
}


void Tyche::Input::addAction(const Action& action) {
    _key_map.put(action, new vector<Key>);
}

void Tyche::Input::addKey(const Action& action, Key key) {
    if (_key_map.has(action)) {
        vector<Key>& keys = *_key_map.get(action);

        keys.push_back(key);
    }
}



bool Tyche::Input::isActionPressed(const Action& action) {
    vector<Key>* keys = _key_map.get(action);

    for (auto key : *keys) {
        int input = glfwGetKey(_target_window, key);
        if (input == GLFW_PRESS) {
            return true;
        }
    }

    return false;
}

