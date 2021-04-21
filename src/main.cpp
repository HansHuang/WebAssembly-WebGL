

#include <stdio.h>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>

#include "blotter.cpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
using namespace ImGui;

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

GLFWwindow* window;
bool showDemoWidgets = false;
bool showDemoBlotter = false;
ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool quitApp = false;

#ifdef __EMSCRIPTEN__
EM_JS(int, getCanvasWidth, (), {
    return Module.canvas.width;
});

EM_JS(int, getCanvasHeight, (), {
    return Module.canvas.height;
});

EM_JS(void, resizeCanvas, (), {
    Module.resizeCanvas();
});
#endif

void render() {
    glfwPollEvents();

#ifdef __EMSCRIPTEN__
    int width = getCanvasWidth();
    int height = getCanvasHeight();
    glfwSetWindowSize(window, width, height);
#endif

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    NewFrame();

    {
#ifndef __EMSCRIPTEN__
        const ImGuiViewport* main_viewport = GetMainViewport();
        SetNextWindowViewport(rand());
        SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 120, main_viewport->WorkPos.y + 120), ImGuiCond_FirstUseEver);
#endif
        SetNextWindowSize(ImVec2(400, 180), ImGuiCond_FirstUseEver);

        static int counter = 0;
        static bool show = true;

        Begin("Hello, world!", &show, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar);

        NewLine();
        Checkbox("Demo Widgets", &showDemoWidgets);
        SameLine(0, 20);
        Checkbox("Demo Blotter", &showDemoBlotter);

        NewLine();
        ColorEdit3("clear color", (float*)&clearColor, ImGuiColorEditFlags_NoInputs);
        SameLine(0, 30);
        if (Button("Button")) counter++;
        SameLine();
        Text("counter = %d", counter);
#ifndef __EMSCRIPTEN__
        SameLine(0, 10);
        if (Button("Quit")) quitApp = true;
#endif

        BeginChild("blank", ImVec2(0, -GetTextLineHeightWithSpacing()));
        EndChild();
        Text("Application average %.2f ms/frame (%.1f FPS)", 1000.0f / GetIO().Framerate, GetIO().Framerate);

        End();
    }

    if (showDemoWidgets) {
        ShowDemoWindow(&showDemoWidgets);
    }

    if (showDemoBlotter) {
        App::ShowDemoBlotter(&showDemoBlotter);
    }

    // Rendering
    Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

#ifndef __EMSCRIPTEN__
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    UpdatePlatformWindows();
    RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
    glfwHideWindow(window);
#endif

    glfwSwapBuffers(window);
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int init(int width, int height, const char* title) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(2);  // limit to max 30 FPS

    IMGUI_CHECKVERSION();
    CreateContext();
    ImGuiIO& io = GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

#ifndef __EMSCRIPTEN__
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.ConfigViewportsNoAutoMerge = true;
#endif

    //io.ConfigViewportsNoTaskBarIcon = true;

    StyleColorsDark();
    // StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    io.Fonts->AddFontFromFileTTF("../misc/fonts/SourceCodePro-Regular.ttf", 16.0f);
    io.Fonts->AddFontFromFileTTF("../misc/fonts/SourceCodePro-Regular.ttf", 15.0f);
    io.Fonts->AddFontFromFileTTF("../misc/fonts/SourceCodePro-Regular.ttf", 26.0f);
    io.Fonts->AddFontFromFileTTF("../misc/fonts/SourceCodePro-Regular.ttf", 32.0f);
    io.Fonts->AddFontDefault();

#ifdef __EMSCRIPTEN__
    resizeCanvas();
#endif

    return 0;
}

void dispose() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

int main(int, char**) {
    if (init(100, 100, "Demo OpenGL3 GLFW App")) return 1;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(render, 0, 1);
    // glfwTerminate();
#else
    while (!glfwWindowShouldClose(window) && !quitApp) {
        render();
    }
    dispose();
#endif

    return 0;
}
