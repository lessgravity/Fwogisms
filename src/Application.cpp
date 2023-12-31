#include <Fwogisms/Application.hpp>

#include <debugbreak.h>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Fwog/Context.h>

#include <iostream>
#include <string>

void Application::Run()
{
    FrameMarkStart("App Run");
    if (!Initialize())
    {
        return;
    }

    spdlog::info("App: Initialized");

    if (!Load())
    {
        return;
    }

    spdlog::info("App: Loaded");

    double previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(_windowHandle))
    {
        glfwPollEvents();

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;

        Update(deltaTime);
        Render(deltaTime);

        previousTime = currentTime;
    }

    spdlog::info("App: Unloading");

    Unload();

    spdlog::info("App: Unloaded");
    FrameMarkEnd("App Run");
}

void Application::Close()
{
    glfwSetWindowShouldClose(_windowHandle, 1);
}

bool Application::IsKeyPressed(int32_t key)
{
    return glfwGetKey(_windowHandle, key) == GLFW_PRESS;
}

bool Application::Initialize()
{
    if (!glfwInit())
    {
        spdlog::error("Glfw: Unable to initialize");
        return false;
    }

    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

    const auto primaryMonitor = glfwGetPrimaryMonitor();
    const auto primaryMonitorVideoMode = glfwGetVideoMode(primaryMonitor);

    const auto screenWidth = primaryMonitorVideoMode->width;
    const auto screenHeight = primaryMonitorVideoMode->height;

    const auto windowWidth = static_cast<int32_t>(screenWidth * 0.9f);
    const auto windowHeight = static_cast<int32_t>(screenHeight * 0.9f);

    _windowHandle = glfwCreateWindow(windowWidth, windowHeight, "Fwogisms", nullptr, nullptr);
    if (_windowHandle == nullptr)
    {
        spdlog::error("Glfw: Unable to create window");
        glfwTerminate();
        return false;
    }


    int monitorLeft = {};
    int monitorTop = {};
    glfwGetMonitorPos(primaryMonitor, &monitorLeft, &monitorTop);
    glfwSetWindowPos(
        _windowHandle,
        screenWidth / 2 - windowWidth / 2 + monitorLeft, 
        screenHeight / 2 - windowHeight / 2 + monitorTop);

    int32_t iconWidth = {};
    int32_t iconHeight = {};

    auto* iconData = stbi_load("data/app.png", &iconWidth, &iconHeight, nullptr, 4);
    if (iconData != nullptr)
    {
        GLFWimage iconImage
        {
            .width = iconWidth,
            .height = iconHeight,
            .pixels = iconData
        };
        glfwSetWindowIcon(_windowHandle, 1, &iconImage);
        stbi_image_free(iconData);
    }

    glfwMakeContextCurrent(_windowHandle);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Fwog::Initialize();

    ImGui::CreateContext();
    AfterCreatedUiContext();
    ImGui_ImplGlfw_InitForOpenGL(_windowHandle, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    return true;
}

bool Application::Load()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([](GLenum source,
                              GLenum type,
                              GLuint,
                              GLenum severity,
                              GLsizei,
                              const GLchar* message,
                              const void*)
    {
        if (type == GL_DEBUG_TYPE_ERROR)
        {
            spdlog::error("GL CALLBACK: type = {}, severity = error, message = {}\n", type, message);
            debug_break();
        }
    }, nullptr);

    glfwSwapInterval(1);

    return true;
}

void Application::Unload()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    BeforeDestroyUiContext();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Application::Render(double deltaTime)
{
    ZoneScopedC(tracy::Color::Red2);

    glEnable(GL_FRAMEBUFFER_SRGB);
    RenderScene();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        RenderUI(deltaTime);
        ImGui::Render();
        glDisable(GL_FRAMEBUFFER_SRGB);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();
    }

    glfwSwapBuffers(_windowHandle);
}

void Application::RenderScene()
{
}

void Application::RenderUI(double deltaTime)
{
}

void Application::Update(double deltaTime)
{
}

void Application::AfterCreatedUiContext()
{
}

void Application::BeforeDestroyUiContext()
{
}