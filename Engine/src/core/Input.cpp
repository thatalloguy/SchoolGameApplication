//
// Created by allos on 11/21/2024.
//

#include "Input.h"

#include "stl/hashmap.h"
#include "stl/vector.h"

namespace Tyche::Input {

    hashmap<const Action,vector<Key>*, StringHashFunc> _key_map;
    hashmap<const Action, bool, StringHashFunc> _key_state_map;
    GLFWwindow* _target_window;

}


void Tyche::Input::setTargetWindow(Window& window) {
    _target_window = (GLFWwindow*) window.getRawWindowPtr();
}


void Tyche::Input::addAction(const Action& action) {
    _key_map.put(action, new vector<Key>);
    _key_state_map.put(action, false);
}

void Tyche::Input::addKey(const Action& action, Key key) {
    if (_key_map.has(action)) {
        vector<Key>& keys = *_key_map.get(action);

        keys.push_back(key);
    }
}

void Tyche::Input::processInputs() {

    auto entry = _key_map.getStartHashNode();

    while (entry != nullptr) {
        vector<Key>& keys = *entry->getValue();
        _key_state_map.put(entry->getKey(), false);

        for (auto key : keys) {
            int input = glfwGetKey(_target_window, key);
            if (input == GLFW_PRESS) {
                _key_state_map.put(entry->getKey(), true);
            }
        }
        entry = entry->getNext();
    }
}

bool Tyche::Input::isActionPressed(const Action& action) {
    if (_key_map.has(action) && _key_state_map.has(action)) {
        bool state = _key_state_map.get(action);

        return state;
    }

    return false;
}

void Tyche::Input::cleanUp() {
    auto entry = _key_map.getStartHashNode();

    while (entry != nullptr) {
        auto keys = entry->getValue();

        delete keys;

        entry = entry->getNext();
    }
}
