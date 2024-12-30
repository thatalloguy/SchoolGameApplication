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

    //Setup our window, renderer, audio engine, room manager


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

    Tyche::Input::setTargetWindow(*_window);

    // Add our actions we need for the player.
    Tyche::Input::addAction("jump");
    Tyche::Input::addAction("left");
    Tyche::Input::addAction("right");
    Tyche::Input::addKey("jump", Tyche::Input::Key::W);
    Tyche::Input::addKey("left", Tyche::Input::Key::D);
    Tyche::Input::addKey("right", Tyche::Input::Key::A);

    player.initialize();

    _room_manager = new RoomManager(_tile_renderer, _entity_renderer, *_debug_renderer, player);

    //Register all the game entities.
    _room_manager->registerEntity<Entities::FallingTile>(0);
    _room_manager->registerEntity<Entities::Portal>(1);
    _room_manager->registerEntity<Entities::Spikes>(2);
    _room_manager->registerEntity<Entities::Spring>(3);

    _world = _room_manager->getWorld();

    // For some reason the physics engine doenst like it when the player rigidbody
    // is somewhere else except the last body world.
    _world->addRigidBody(&player.getBody());

    _audio_engine.Init();
    // We give the audio_engine to the room manager so that entities can easily acess it.
    _room_manager->setAudioEngine(&_audio_engine);

    initializeImgui();
}

Game::App::~App() {

    _audio_engine.CleanUp();
    Tyche::Input::cleanUp();
    cleanupImgui();


    delete _room_manager;
    delete _debug_renderer;
    delete _window;
}

void Game::App::run() {
    auto currentTime = std::chrono::high_resolution_clock ::now();

    _tile_renderer.prepareRendering();

    while (!_window->shouldWindowClose()) {

        //Calculate the time between frames.
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


void Game::App::mainloopTick(float frameTime) {

    // First update our bodies
    _world->step(frameTime);
    _world->renderBodies(*_debug_renderer);

    //update the entities inside of the current room.
    _room_manager->update(frameTime);

    // Update our player and the audio engine
    player.update(frameTime);

    // update the audio engine so that the sounds are coming from the correct direction.
    _audio_engine.update(player.getPosition());

    _camera.setViewportSize(_window->getWindowSize());

    // 640 = 1280 / 2 (which is always the width of the camera view)
    _camera.setPosition({-player.getPosition().getX() + 640, _camera.getPosition().getY()});
    _camera.update();

    //render tiles first so entities are in front.
    _tile_renderer.renderTiles(_camera);
    _entity_renderer.renderEntities(_camera);

    // Render the player separate so they appear in front of any entity.
    _entity_renderer.renderEntity( _camera, &player);

    // Debug rendering.
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

    // A very ugly Main menu, but better something then nothing

    //Create a imgui window that covers the entire screen.
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({_window->getWindowSize()[0], _window->getWindowSize()[1]});
    ImGui::Begin("Main menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                                                            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


    float window_width = _window->getWindowSize()[0];
    float window_height = _window->getWindowSize()[1];

    // calc a font scale based on the diagonal size of the window
    float scale = sqrt((window_width * window_width) + (window_height * window_height)) / 800.0f;

    // Draw the title
    ImGui::SetWindowFontScale(scale);
    ImGui::Text("Tomb Crawler");
    ImGui::Separator();

    // Move to the next pos
    ImGui::SetWindowFontScale(scale * 0.2f);
    ImGui::SetCursorPos({window_width * 0.05f, window_height * 0.45f});

    // Draw the seed input
    ImGui::SetNextItemWidth(window_width * 0.2f);
    ImGui::InputText(" <- Seed ", seed_to_use, 10);
    ImGui::NewLine();

    // Draw the start button
    ImGui::SetWindowFontScale(scale * 0.4f);
    if (ImGui::Button("START")) {
        // Switch to the game loop
        _current_state = GameState::GAMELOOP;

        // Setups the room list and chooses the starter room based on the seed.
        _room_manager->loadRoomList("../../../Rooms", seed_to_use);

        // load the starter room.
        _room_manager->loadStartRoom();
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


    //Load custom font :D
    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->AddFontFromFileTTF("../../../Resources/Fonts/KarmaFuture.ttf", 128);
    io.Fonts->Build();
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
