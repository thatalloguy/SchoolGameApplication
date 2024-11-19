add_library(tyche-engine INTERFACE)
target_include_directories(tyche-game INTERFACE include)


file(GLOB_RECURSE TYCHE_ENGINE_SRC

        src/core/*.h
        src/core/*.cpp

)

add_library(tyche-engine-static STATIC ${TYCHE_ENGINE_SRC} ${IMGUI_SOURCE} ${GLAD_SOURCE} ${STB_IMAGE_SOURCE})
target_link_libraries(tyche-engine-static PUBLIC tyche-engine)
target_include_directories(tyche-engine-static PUBLIC src)


target_include_directories(tyche-engine-static PUBLIC "Thirdparty/imgui")
target_include_directories(tyche-engine-static PUBLIC "Thirdparty/stb")
target_include_directories(tyche-engine-static PUBLIC "Thirdparty/glfw/include")
target_include_directories(tyche-engine-static PUBLIC "Thirdparty/glad/include/")