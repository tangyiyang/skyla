#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "level_editor.h"

#include "lua_handler.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**)
{
        // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL3 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    gl3wInit();

    ImGui_ImplGlfwGL3_Init(window, true);


    ImVec4 clear_color = ImColor(114, 144, 154);

    game_tool::lua_handler* _l = new game_tool::lua_handler();
    _l->init();

    bool show_test_window = true;
    game_tool::level_editor* editor = new game_tool::level_editor();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        _l->update(0);
//        editor->draw();

//
//        ImGuiIO& io = ImGui::GetIO();
//        if (io.KeyCtrl) {
//            ImGui::Text("Ctrl");
//        }
//
//        if (io.KeyAlt) {
//            ImGui::Text("Alt");
//        }
//
//        if (io.KeySuper) {
//            ImGui::Text("Super");
//        }
//
//        ImGui::Text("Keys down:");
//        for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) {
//            if (io.KeysDownDuration[i] >= 0.0f) {
//                ImGui::SameLine();
//                ImGui::Text("%d (%.02f secs)", i, io.KeysDownDuration[i]);
//            }
//        }

//        if (show_test_window) {
//            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
//            ImGui::ShowTestWindow(&show_test_window);
//        }

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);
    }
        // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    delete editor;

    return 0;
}

