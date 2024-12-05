//
// Created by allos on 11/19/2024.
//

/// Credits: Catos/Runtime/src/core/Window.h - on github by thatalloguy (me)

#pragma once

#include <glad/glad.h>
#include <math/vecs.h>
#include <stl/string.h>
#include <GLFW/glfw3.h>

using namespace Tyche::Math;
using namespace Tyche::STL;

namespace Tyche {

    //This is a struct to specify how our window should be created.
    struct WindowCreationInfo {
        Vector2 size{1280, 720};
        string window_title = "Tyche Application";
        bool is_fullscreen = false;
        bool borderless = false;
        //Note: only supported on the windows platform
        bool enable_darktheme = true;
    };

    class Window {

    public:
        //Retuns the Size of the PRIMARY monitor.
        static Vector2 getMonitorSize();

        //Creates and initializes a window with glfw and glad (our gl loader)
        Window(const WindowCreationInfo& creation_info);

        //Destroys all allocated objects by this class.
        ~Window();

        //Returns the size of the window.
        Vector2 getWindowSize();

        Vector2 getWindowPosition();

        //Returns true or false if the user has requested to close the window.
        bool shouldWindowClose();

        //Checks for events and swaps the buffers of the window.
        void update();

        //Returns a raw pointer to the underlaying glfw window.
        void* getRawWindowPtr();

    private:

        GLFWwindow* _raw_window = nullptr;

        void enableDarktheme();
    };


}
