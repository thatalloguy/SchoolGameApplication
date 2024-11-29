add_library(tyche-engine INTERFACE)
target_include_directories(tyche-game INTERFACE include)


file(GLOB_RECURSE TYCHE_ENGINE_SRC

        src/core/*.h
        src/core/*.cpp

        src/math/*.h
        src/math/*.cpp

        src/stl/*.h
        src/stl/*.cpp

        src/renderer/*.h
        src/renderer/*.cpp

        src/physics/*.h
        src/physics/*.cpp

        src/audio/*.h
        src/audio/*.cpp
)

add_library(tyche-engine-static STATIC ${TYCHE_ENGINE_SRC} ${IMGUI_SOURCE} ${GLAD_SOURCE} ${STB_IMAGE_SOURCE})
target_link_libraries(tyche-engine-static PUBLIC tyche-engine)
target_include_directories(tyche-engine-static PUBLIC src)


target_include_directories(tyche-engine-static PUBLIC "Thirdparty/imgui")
target_include_directories(tyche-engine-static PUBLIC "Thirdparty/stb")
target_include_directories(tyche-engine-static PUBLIC "Thirdparty/glfw/include")
target_include_directories(tyche-engine-static PUBLIC "Thirdparty/glad/include/")

target_link_libraries(tyche-engine-static PUBLIC spdlog)
target_link_libraries(tyche-engine-static PUBLIC glfw)

target_link_libraries(tyche-engine-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/steamaudio/lib/windows-x64/phonon.lib)

# If we are on windows also link dwmapi so that we can support darktheme
IF (WIN32)
    target_link_libraries(tyche-engine-static PUBLIC dwmapi)
ENDIF()