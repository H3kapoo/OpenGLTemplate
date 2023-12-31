#pragma once

#include <stdio.h>

#include "renderHelpers/RenderHelper.hpp"
#include "shaderHelpers/ShaderHelper.hpp"
#include "meshHelpers/RectMesh.hpp"
#include "treeHelpers/ConcreteNode.hpp"
#include "inputHelpers/Types.hpp"
#include "stateHelpers/WindowState.hpp"

class GLFWwindow; /* Fwd declare to avoid include errors */

class Application /* This shall later inherit from IApplication for better reusability */
{
public:
    Application(GLFWwindow* windowHandle);
    void setup();
    void loop();
    void onKeyPress(int key, int sc, int action, int mods);
    void onWindowResize(int width, int height);
    void onButtonAction(int button, int action, int mods);
    void onMouseMoveAction(double xPos, double yPos);

private:
    void keepRatio();
    void setTitle(const std::string& title);

    /* Basic shader */
    std::string gVertPath{ "src/assets/shaders/basicV.glsl" };
    std::string gFragPath{ "src/assets/shaders/basicF.glsl" };

    std::string gBorderedVertPath{ "src/assets/shaders/borderedV.glsl" };
    std::string gBorderedFragPath{ "src/assets/shaders/borderedF.glsl" };

    /* Basic mesh */
    treeHelpers::ConcreteNode gRootConcreteNode{ gBorderedVertPath, gBorderedFragPath };
    treeHelpers::ConcreteNode gFpNode{ gBorderedVertPath, gBorderedFragPath };
    treeHelpers::ConcreteNode gStatusNode{ gBorderedVertPath, gBorderedFragPath };
    treeHelpers::ConcreteNode gExtractNode{ gBorderedVertPath, gBorderedFragPath };

    stateHelpers::WindowState gWindowState;

    bool gReloadShader{ false };
    GLFWwindow* gWindowHandle{ nullptr };

    shaderHelpers::ShaderHelper& gShInstance;
    renderHelpers::RenderHelper& gRenderInstance;
};