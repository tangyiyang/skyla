#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "lua_editor.h"
#include "skyla.h"
#include "entry.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#define WINDOW_WIDTH (1280)
#define WINDOW_HEIGHT (720)

#define CANAS_HEIGHT (WINDOW_HEIGHT/2)
static skyla::vec2 translate_to_canvas(float mouse_pos_x,
                                       float mouse_pos_y,
                                       float cursor_x,
                                       float cursor_y,
                                       float window_pos_x,
                                       float window_pos_y)
{
    float xt = mouse_pos_x - window_pos_x - cursor_x;
    float yt = mouse_pos_y - window_pos_y - cursor_y;

    float x = xt * 2;
    float y = yt * 2;

    return skyla::vec2::make(x, y);
}

/*
 * p is the position in canvas coordinate
 */
static bool pressing = false;
static bool moving = false;
static skyla::vec2 begin_p;
static void dispatch_mouse_event_to_canvas(const skyla::vec2& p)
{
    ImGuiIO& io = ImGui::GetIO();
    skyla::context* C = skyla::context::C();

    // TODO: this is an ugly simulation, written in quick and dirty way.
    // consider a better way in the furture. :)
    if (!io.MouseDown[0] && pressing && moving) {
        C->on_touch_ended(p.x, p.y);
        pressing = false;
        moving = false;
        return;
    }

    if (io.MouseDown[0] && !pressing) {
        pressing = true;
        C->on_touch_begin(p.x, p.y);
        begin_p = p;
    } else if ((io.MouseDown[0] && pressing) || moving) {
        if ((p.x - begin_p.x)*(p.x - begin_p.x) + (p.y - begin_p.y)* (p.y - begin_p.y) >= 10) {
            moving = true;
        }
        if (moving) {
            C->on_touch_moved(p.x, p.y);
        }
    } else {
        // pass, we do not touch, just moving around.
    }
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
}

void MousePosCallback(GLFWwindow* window, double x, double y)
{
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

// tmp code, remove later on

node* game_scene(skyla::context* ctx)
{
    lua_State* L = ctx->_lua_context->_lua_state;
    bool ok = ctx->_lua_context->load_lua_file("res/scripts/game_scene_renderer.lua");
    lua_context::call_lua(L, 0, LUA_MULTRET);
    lua_getglobal(L, "global_game_scene");
    lua_context::call_lua(L, 0, 1);
    if (ok) {
        node* n = (node*)(lua_touserdata(L, -1));
        lua_pop(L, -1);
        return n;
    } else {
        LOGE("error load the scene file for editor, check game_scene_renderer.lua");
        exit(-1);
        return nullptr;
    }
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

    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    GLFWwindow* window = glfwCreateWindow(width, height, "ImGui OpenGL3 example", NULL, NULL);
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

    entry* game_entry = new entry();
    skyla::context* ctx = new skyla::context(game_entry);
    ctx->init(3, width, height);
    ctx->set_content_scale_factor(content_scale_factor);

    bool show_test_window = true;

    int window_width = width/2;
    int window_height = height/2;
    node* rendered_scene = game_scene(ctx);

    render_texture* rt = new render_texture();
    rt->init(width, height);

    bool _window_open = false;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        ImGui::SetNextWindowSize(ImVec2(window_width, window_height),
                                 ImGuiSetCond_FirstUseEver);
        ImGui::Begin("game-scene", &_window_open, ImGuiWindowFlags_NoMove);
        {
            ImGuiIO& io = ImGui::GetIO();
            vec2 pos = translate_to_canvas(io.MousePos.x,
                                           io.MousePos.y,
                                           ImGui::GetCursorPosX(),
                                           ImGui::GetCursorPosY(),
                                           ImGui::GetWindowPos().x,
                                           ImGui::GetWindowPos().y);
            dispatch_mouse_event_to_canvas(pos);

            rt->draw(rendered_scene);

            ImGui::Image((ImTextureID)rt->_name, ImVec2(window_width, window_height), ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();

        if (show_test_window) {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        ctx->update(0);
        ctx->draw();

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();

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
