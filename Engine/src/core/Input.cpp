//
// Created by allos on 11/21/2024.
//

#include "Input.h"

#include "stl/hashmap.h"
#include "stl/vector.h"

namespace Tyche::Input {
    //Global constants


    //We specify we need the StringHash function since the action is a String.
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

    //Iterator through all of the keys bound to the action.
    // if any returns true then we do as well.
    for (auto key : *keys) {
        int input = glfwGetKey(_target_window, key);
        if (input == GLFW_PRESS) {
            return true;
        }
    }

    return false;
}

bool Tyche::Input::isKeyPressed(Key key) {
    return (glfwGetKey(_target_window, key) == GLFW_PRESS);
}

void Tyche::Input::cleanUp() {

    // Deleting all of the allocated vectors when a new action gets created.

    for (int i=0; i<_key_map.maxLength(); i++) {

        hashnode<const Action,vector<Key>*>* prev = nullptr;
        auto entry = _key_map.getNode(i);

        // Iterator through each non null entry, and delete their value.
        while(entry != nullptr) {
            prev = entry;
            entry = entry->getNext();

            delete prev->getValue();
        }


    }
}
