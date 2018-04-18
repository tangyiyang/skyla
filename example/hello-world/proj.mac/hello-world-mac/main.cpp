/****************************************************************************
 * Copyright (c) Yiyang Tang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <time.h>

#include <GLFW/glfw3.h>

#include "s2d_context.h"
#include "entry.h"

static void frame_buffer_resize_callback(GLFWwindow* window, int width, int height)
{
    int win_width, win_height;
    s2d::context* ctx = (s2d::context*)glfwGetWindowUserPointer(window);

    glfwGetWindowSize(window, &win_width, &win_height);
    ctx->set_viewport_rect({0, 0, (float)width, (float)height});
    glViewport(0, 0, width, height);
    ctx->set_content_scale_factor(width/win_width);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            default:
                break;
        }
    }
}

static bool pressing = false;

static void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS && !pressing) {
        pressing = true;
        return;
    } else if (state == GLFW_RELEASE && pressing) {
        pressing = false;
        return;
    }

    s2d::context* C = s2d::context::C();
    if (pressing) {
        C->on_touch_moved(x, y);
    }

    static char title[256] = "";
    snprintf(title, 256, "[%s] [%d, %d]",
             C->_app->_app_name.c_str(), (int)x, (int)(C->_window_size.height - y));
    glfwSetWindowTitle(window, title);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int modifier)
{
    s2d::context* C = s2d::context::C();
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (action == GLFW_RELEASE) {
        pressing = false;
        C->on_touch_ended(x, y);
    } else if (action == GLFW_PRESS) {
        pressing = true;
        C->on_touch_begin(x, y);
    }
}


int main(int argc, char** argv)
{
    int width, height;
    int win_width, win_height;
    GLFWwindow* window;

    entry* game_entry = new entry();
    s2d::context* ctx = new s2d::context(game_entry);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    win_width  = 1136;
    win_height = 640;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(win_width, win_height, ctx->_app->_app_name.c_str(), NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowUserPointer(window, ctx);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, frame_buffer_resize_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Set initial aspect ratio
    glfwGetFramebufferSize(window, &width, &height);
    frame_buffer_resize_callback(window, width, height);

    glfwSetTime(0.0);

    ctx->init(3, win_width, win_height);
    ctx->set_content_scale_factor(width/win_width);

    double dt = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        if(!glfwGetWindowAttrib(window, GLFW_ICONIFIED)) { // prevent cpu 100% when in background.
            static double last = 0.0f;
            double now = glfwGetTime();
            dt = now - last;

            ctx->update(dt);
            ctx->draw();

            last = now;

            glfwSwapBuffers(window);
            glfwPollEvents();
        } else {
            glfwWaitEvents();
        }
    }

    ctx->shutdown();
    delete ctx;
    delete game_entry;

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

