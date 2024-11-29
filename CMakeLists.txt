cmake_minimum_required(VERSION 3.28)
project(TombCrawler)

# We require a minimum of CPP 20
set(CMAKE_CXX_STANDARD 20)

# List of dependencies (that we can automatically do with cmake)
add_subdirectory("Thirdparty/glfw")
add_subdirectory("Thirdparty/spdlog")

# Imgui, stb  and GLAD we have to do manually
file(GLOB_RECURSE IMGUI_SOURCE
        Thirdparty/imgui/imgui.cpp
        Thirdparty/imgui/imgui.h
        Thirdparty/imgui/imgui_internal.h
        Thirdparty/imgui/imgui_internal.cpp
        Thirdparty/imgui/imconfig.h
        Thirdparty/imgui/imstb_rectpack.h
        Thirdparty/imgui/imgui_draw.cpp
        Thirdparty/imgui/imgui_tables.cpp
        Thirdparty/imgui/imgui_widgets.cpp
        Thirdparty/imgui/imgui_demo.cpp

        Thirdparty/imgui/backends/imgui_impl_glfw.h
        Thirdparty/imgui/backends/imgui_impl_glfw.cpp

        Thirdparty/imgui/extensions/imgui_dock.h
        Thirdparty/imgui/extensions/imgui_dock.cpp
)

file(GLOB_RECURSE GLAD_SOURCE
        Thirdparty/GLAD/include/glad/glad.h
        Thirdparty/GLAD/src/glad.c
)

file(GLOB_RECURSE STB_IMAGE_SOURCE
        Thirdparty/stb/stb_image.h
)

set(STEAMAUDIO_DLL_LOC "ThirdParty/steamaudio/lib/windows-x64/phonon.dll")


include_directories("Thirdparty/imgui")
include_directories("Thirdparty/stb")
include_directories("Thirdparty/glfw/include")
include_directories("Thirdparty/GLAD/include")
include_directories("Thirdparty/miniaudio/")
include_directories("Thirdparty/steamaudio/include")

# Tyche is the name i use for the project :)
add_library(tyche-headers INTERFACE)


# Splitting the mapEditor, Main Game and Engine into different sub folders
# for easier building / source code management.

add_subdirectory(Game)
add_subdirectory(Engine)

# Building SteamAudio is a bit weird, so we have to manually copy the phonon.dll file to where our executables are.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(COPY ${STEAMAUDIO_DLL_LOC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Engine/Debug/)
    file(COPY ${STEAMAUDIO_DLL_LOC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Game/Debug/)
else()
    file(COPY ${STEAMAUDIO_DLL_LOC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Engine/Release/)
    file(COPY ${STEAMAUDIO_DLL_LOC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Game/Release/)
endif()

##add_subdirectory(MapEditor)