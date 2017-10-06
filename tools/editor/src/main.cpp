#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "lua_handler.h"
#include "s2d.h"
#include "entry.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

static bool is_touch_down = false;
static float cursor_x;
static float cursor_y;
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);

    switch (action) {
        case GLFW_PRESS: {
            context::C()->on_touch_begin(cursor_x, cursor_y);
            is_touch_down = true;
            }
            break;

        case GLFW_RELEASE: {
            context::C()->on_touch_ended(cursor_x, cursor_y);
            is_touch_down = false;
        }
            break;

        default:
            break;
    }
}

void MousePosCallback(GLFWwindow* window, double x, double y)
{
    cursor_x = x;
    cursor_y = y;
    if (is_touch_down) {
        context::C()->on_touch_moved(x, y);
    }
    static char title_text[256] = "";
    snprintf(title_text, 256, "%s:(%d, %d)",
             "seal2d",
             (int)(x), (int)(context::C()->_window_size.height - y));
    glfwSetWindowTitle(window, title_text);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
}

void CharCallback(GLFWwindow* window, unsigned int c)
{
    ImGui_ImplGlfwGL3_CharCallback(window, c);
}


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int width = 1280;
    int height = 720;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL3 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    gl3wInit();

    int fb_w, fb_h;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    float content_scale_factor = fb_w / width;

    ImGui_ImplGlfwGL3_Init(window, false);

    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCharCallback(window, CharCallback);

    glfwSetCursorPosCallback(window, MousePosCallback);

    ImVec4 clear_color = ImColor(114, 144, 154);

    game_tool::lua_handler* _l = new game_tool::lua_handler();
    _l->init();


    entry* game_entry = new entry();
    s2d::context* ctx = new s2d::context(game_entry);
    ctx->init(3, width, height);
    ctx->set_content_scale_factor(content_scale_factor);

    bool show_test_window = true;


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        _l->update(0);
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

        ctx->loop(0);

        glfwSwapBuffers(window);
    }

    ctx->shutdown();
    delete game_entry;
    delete ctx;
        // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
