#include <Fwogisms/FwogismsApplication.hpp>

#include <imgui.h>

void FwogismsApplication::AfterCreatedUiContext()
{

}

void FwogismsApplication::BeforeDestroyUiContext()
{

}

bool FwogismsApplication::Load()
{
    if (!Application::Load())
    {
        return false;
    }

    return true;
}

void FwogismsApplication::RenderScene()
{

}

void FwogismsApplication::RenderUI(double deltaTime)
{
    ImGui::ShowDemoWindow();
}

void FwogismsApplication::Update(double deltaTime)
{

}