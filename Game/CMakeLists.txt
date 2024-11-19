
file(GLOB_RECURSE GAME_SOURCE
    src/core/*.h
    src/core/*.cpp
)

add_executable(tyche-game
        src/main.cpp
        ${GAME_SOURCE}
)


target_link_libraries(tyche-game PRIVATE tyche-engine-static)
target_link_libraries(tyche-game PRIVATE tyche-headers)