#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <string>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

enum MenuCollapseState
{
    NoneOpen,
    CommandOpen,
    SequencesOpen,
    WorkspacesOpen
};

struct State
{
    int WindowWidth;
    int WindowHeight;
    ImVec4 ClearColor;
    MenuCollapseState MenuOpenState;
    ImVec2 GetWindowSize();
};

ImVec2 State::GetWindowSize()
{
    ImVec2 size{(float)WindowWidth, (float)WindowHeight};
    return size;
}

class ScriptCut
{
private:
public:
    ScriptCut();
    ~ScriptCut();

    static bool Init();
    static void Clean();
    static bool IsRunning();
    static int Render(State *state);
    static int Update(State *state);
    static GLFWwindow *GetWindow();
};

ScriptCut::ScriptCut(/* args */)
{
}

ScriptCut::~ScriptCut()
{
}

GLFWwindow *ScriptCut::GetWindow()
{
    static GLFWwindow *window = nullptr; // if not using C++11 use NULL or 0.
    if (nullptr == window)
        window = glfwCreateWindow(480, 720, "Window Title", NULL, NULL);
    return window;
}

bool ScriptCut::IsRunning()
{
    return !glfwWindowShouldClose(GetWindow());
}

void ScriptCut::Clean()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(GetWindow());
    glfwTerminate();
}

bool ScriptCut::Init()
{

    // Setup window
    glfwSetErrorCallback([](int error, const char *description)
                         { fprintf(stderr, "Glfw Error %d: %s\n", error, description); });

    if (!glfwInit())
        return false;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    if (!GetWindow())
        return false;
    glfwMakeContextCurrent(GetWindow());
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(GetWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}