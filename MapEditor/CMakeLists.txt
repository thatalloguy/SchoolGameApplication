
file(GLOB_RECURSE EDITOR_SOURCE
        src/core/*.h
        src/core/*.cpp

        src/entities/*.h
        src/entities/*.cpp
)

add_executable(tyche-editor
        src/main.cpp
        ${GAME_SOURCE}
)


target_link_libraries(tyche-editor PRIVATE tyche-engine-static)
target_link_libraries(tyche-editor PRIVATE tyche-headers)