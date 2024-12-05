//
// Created by allos on 11/19/2024.
//

#include "Window.h"
#include <spdlog/spdlog.h>


#ifdef _WIN32
#include "window.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <dwmapi.h>
#endif

Vector2 Tyche::Window::getMonitorSize() {
    // Try to initialize glfw here because else we cant use the glfwGetPrimaryMonitor function.
    glfwInit();

    // get the primary monitor and return the size.

    auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    return Vector2{(float) mode->width,(float) mode->height};
}

Tyche::Window::Window(const Tyche::WindowCreationInfo &creation_info) {
    if (!glfwInit()) {
        spdlog::error("Could not initialize GLFW!");
        exit(-11); // For the game to run we would need glfw
    }

    // We do opengl 4.2 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




    GLFWmonitor* monitor = NULL;

    // If we've specified to do fullscreen, then pass along the monitor to the createWindow.
    if (creation_info.is_fullscreen) {
        monitor = glfwGetPrimaryMonitor();
        spdlog::info("Using Fullscreen");
    } else {
        spdlog::info("Using Windowed mode");
    }

    _raw_window = glfwCreateWindow((int) creation_info.size.getX(),(int) creation_info.size.getY(), creation_info.window_title.c_str(), monitor, NULL);

    if (!_raw_window) {
        spdlog::error("Could not create Window");
    } else {
        spdlog::info("Successfully created the window");
    }

    if (creation_info.enable_darktheme) {
        spdlog::info("Enabling darktheme");
        enableDarktheme();
    }

    //Make the Window focused.
    glfwMakeContextCurrent(_raw_window);


    //load Glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        spdlog::error("Could not load GLAD");
        exit(-12);
    } else {
        spdlog::info("Loaded GLAD");
    }
}

Tyche::Window::~Window() {
    spdlog::info("Destroying the window");

    glfwDestroyWindow(_raw_window);
    glfwTerminate();
}

Vector2 Tyche::Window::getWindowSize() {
    int w, h;

    glfwGetWindowSize(_raw_window, &w, &h);

    return Vector2((float) w, (float) h);
}

bool Tyche::Window::shouldWindowClose() {
    return glfwWindowShouldClose(_raw_window);
}

void Tyche::Window::update() {
    glfwPollEvents();
    glfwSwapBuffers(_raw_window);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void *Tyche::Window::getRawWindowPtr() {
    return (void*)_raw_window;
}

void Tyche::Window::enableDarktheme() {
#ifdef _WIN32
    // Get the Raw win32 window and tell windows to enable the darktheme titlebar for it.
    BOOL USE_DARK_MODE = true;

    BOOL status = SUCCEEDED(DwmSetWindowAttribute(
            glfwGetWin32Window(_raw_window), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
            &USE_DARK_MODE, sizeof(USE_DARK_MODE)
    ));
#else
    // if the user isnt on the windows platform we'll display this warning message instead.
    spdlog::warn("[Window] DarkTheme is not supported in this platform!");
#endif
}
