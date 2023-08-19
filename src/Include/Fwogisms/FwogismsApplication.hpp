#pragma once

#include <Fwogisms/Application.hpp>

#include <Fwog/Context.h>
#include <Fwog/Pipeline.h>
#include <Fwog/Rendering.h>
#include <Fwog/Shader.h>
#include <Fwog/Texture.h>

#include <optional>

class FwogismsApplication final : public Application
{
public:
protected:
    void AfterCreatedUiContext() override;
    void BeforeDestroyUiContext() override;
    bool Load() override;
    void RenderScene() override;
    void RenderUI(double deltaTime) override;
    void Update(double deltaTime) override;

private:
    std::optional<Fwog::GraphicsPipeline> pipelineTextured;
    std::optional<Fwog::Texture> cubeTexture;
};