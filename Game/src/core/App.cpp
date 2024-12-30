//
// Created by allos on 11/19/2024.
//

#include "App.h"

#include "core/Input.h"


#include "../entities/Player.h"
#include "../entities/FallingTile.h"
#include "../entities/Portal.h"
#include "../entities/Spikes.h"
#include "../entities/Spring.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

//Global constants
namespace {
    Entities::Player player;
}

Game::App::App(int argc, char **argv) {



    Tyche::WindowCreationInfo defaultWindowInfo{
        .window_title = "Game"
    };

    _window = new Tyche::Window{defaultWindowInfo};
    _camera.setViewportSize(_window->getWindowSize());

    Tyche::TileRendererCreationInfo tile_renderer_info {};
    _tile_renderer.initialize(tile_renderer_info);

    Tyche::EntityRendererCreationInfo entity_renderer_info {};
    _entity_renderer.initialize(entity_renderer_info);

    _debug_renderer = new Tyche::DebugRenderer{};


    Tyche::Tile floor{
        .position{100, 700},
        .scale{1200, 100},
        .texture_pos{0, 0}
    };

    _tile_renderer.addTile(floor);

    Tyche::Input::setTargetWindow(*_window);

    Tyche::Input::addAction("jump");
    Tyche::Input::addAction("left");
    Tyche::Input::addAction("right");
    Tyche::Input::addKey("jump", Tyche::Input::Key::W);
    Tyche::Input::addKey("left", Tyche::Input::Key::D);
    Tyche::Input::addKey("right", Tyche::Input::Key::A);

    player.initialize();


    _room_manager = new RoomManager(_tile_renderer, _entity_renderer, *_debug_renderer, player);

    _room_manager->registerEntity<Entities::FallingTile>(0);
    _room_manager->registerEntity<Entities::Portal>(1);
    _room_manager->registerEntity<Entities::Spikes>(2);
    _room_manager->registerEntity<Entities::Spring>(3);

    _world = _room_manager->getWorld();

    _world->addRigidBody(&player.getBody());
    _audio_engine.Init();

    _room_manager->setAudioEngine(&_audio_engine);


    _room_manager->loadRoomList("../../../Rooms", "TEST");
    _room_manager->loadStartRoom();

    processArgs(argc, argv);

    initializeImgui();
}

Game::App::~App() {
    _audio_engine.CleanUp();
    cleanupImgui();


    delete _room_manager;
    delete _debug_renderer;
    delete _window;
}

void Game::App::run() {
    auto currentTime = std::chrono::high_resolution_clock ::now();

    _tile_renderer.prepareRendering();

    while (!_window->shouldWindowClose()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = (float) std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime).count() / 100000;
        currentTime = newTime;

        // If we dont cap it, then the physics engine will explode since instead of correcting the velocities we are enlarging them.
        if (frameTime >= 0.166)
            frameTime = 0.166;


        _window->update();


        switch (_current_state) {
            case GameState::GAMELOOP:
                mainloopTick(frameTime);
                break;
            case GameState::START_MENU:
                menuTick(frameTime);
                break;
            case GameState::PAUSED:
                break;
        }

    }

}

void Game::App::processArgs(int argc, char** argv) {
    for (int i=0; i< argc; i++) {
        if (strcmp("-d", argv[i]) == 0) {
            debugRoom(argv[i + 1]);
        }
    }
}

void Game::App::debugRoom(const char* path) {
    spdlog::info("Path {}", path);
}

void Game::App::mainloopTick(float frameTime) {

    _world->step(frameTime);
    _world->renderBodies(*_debug_renderer);

    _room_manager->update(frameTime);



    player.update(frameTime);
    _audio_engine.update(player.getPosition());

    _camera.setViewportSize(_window->getWindowSize());

    // 640 = 1280 / 2 (which is always the width of the camera view)
    _camera.setPosition({-player.getPosition().getX() + 640, _camera.getPosition().getY()});
    _camera.update();

    _tile_renderer.renderTiles(_camera);
    _entity_renderer.renderEntities(_camera);
    _entity_renderer.renderEntity( _camera, &player);

    if (Tyche::Input::isKeyPressed(Tyche::Input::F1)) {
        _debug_rendering = true;
    } else if (Tyche::Input::isKeyPressed(Tyche::Input::F2)) {
        _debug_rendering = false;
    }

    if (_debug_rendering) {
        _debug_renderer->render(_camera);
    }

    _debug_renderer->clearQueue();



}

void Game::App::menuTick(float delta_time) {
    newImguiFrame();

    // A very ugly Main menu, but better something then nothing :shrug:

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({_window->getWindowSize()[0], _window->getWindowSize()[1]});
    ImGui::Begin("Main menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                                                            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


    float window_width = _window->getWindowSize()[0];
    float window_height = _window->getWindowSize()[1];

    float scale = sqrt((window_width * window_width) + (window_height * window_height)) / 200.0f;


    ImGui::SetWindowFontScale(scale);
    ImGui::Text("Tomb Crawler");

    ImGui::SetCursorPos({window_width * 0.45f, window_height * 0.45f});
    if (ImGui::Button("START")) {
        _current_state = GameState::GAMELOOP;
    }


    ImGui::End();


    renderImguiFrame();
}

void Game::App::initializeImgui() {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) _window->getRawWindowPtr(), true);
    ImGui_ImplOpenGL3_Init("#version 420");
}

void Game::App::cleanupImgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Game::App::newImguiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Game::App::renderImguiFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
