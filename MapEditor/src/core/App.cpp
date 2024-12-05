//
// Created by allos on 12/5/2024.
//

#include "App.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


MapEditor::App::~App() {
    deinitializeImGui();
}

void MapEditor::App::init() {
    initializeImGui();
}

void MapEditor::App::run() {
    while (!_window.shouldWindowClose()) {

        _window.update();

        newImGuiFrame();

        ImGui::Begin("Hello Window");;

        ImGui::Button("Test");

        ImGui::End();

        renderImGuiFrame();
    }
}

void MapEditor::App::initializeImGui() {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) _window.getRawWindowPtr(), true);
    ImGui_ImplOpenGL3_Init("#version 420");
}

void MapEditor::App::deinitializeImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MapEditor::App::newImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void MapEditor::App::renderImGuiFrame() {

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
