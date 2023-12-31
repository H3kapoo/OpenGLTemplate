#include "Application.hpp"

#include <GLFW/glfw3.h>

#include "utils/CommonUtils.hpp"

Application::Application(GLFWwindow* windowHandle)
    : gWindowHandle{ windowHandle }
    , gShInstance{ shaderHelpers::ShaderHelper::get() }
    , gRenderInstance{ renderHelpers::RenderHelper::get() }
{}

void Application::keepRatio()
{
    /* Root note */
    auto& rootMesh = gRootConcreteNode.gMesh;
    rootMesh.gBox.pos.x = 0;
    rootMesh.gBox.pos.y = 0;
    rootMesh.gBox.scale.x = gWindowState.winWidth;
    rootMesh.gBox.scale.y = gWindowState.winHeight;

    const int32_t spacingPx = 5;
    const float fpScale = 0.65f;
    const float statusScale = 0.15f;
    const float extractScale = 0.20f;

    /* 1st child of root */
    auto& fpMesh = gFpNode.gMesh;
    fpMesh.gBox.pos.x = rootMesh.gBox.pos.x + spacingPx;
    fpMesh.gBox.pos.y = rootMesh.gBox.pos.y + spacingPx;
    fpMesh.gBox.scale.x = rootMesh.gBox.scale.x - spacingPx * 2;
    fpMesh.gBox.scale.y = rootMesh.gBox.scale.y * fpScale - spacingPx;

    /* 2nd child of root */
    auto& statusMesh = gStatusNode.gMesh;
    statusMesh.gBox.pos.x = fpMesh.gBox.pos.x;
    statusMesh.gBox.pos.y = fpMesh.gBox.pos.y + fpMesh.gBox.scale.y + spacingPx;
    statusMesh.gBox.scale.x = fpMesh.gBox.scale.x;
    statusMesh.gBox.scale.y = rootMesh.gBox.scale.y * statusScale - spacingPx;

    // /* 3rd child of root */
    auto& extractMesh = gExtractNode.gMesh;
    extractMesh.gBox.pos.x = statusMesh.gBox.pos.x;
    extractMesh.gBox.pos.y = statusMesh.gBox.pos.y + statusMesh.gBox.scale.y + spacingPx;
    extractMesh.gBox.scale.x = statusMesh.gBox.scale.x;
    extractMesh.gBox.scale.y = rootMesh.gBox.scale.y * extractScale - spacingPx * 2;
}

void Application::setup()
{
    setTitle("UnSnapshot C++ 1.0");

    printf("Setup called\n");
    glfwGetWindowSize(gWindowHandle, &gWindowState.winWidth, &gWindowState.winHeight);

    /* Note: Highest Z renders in front of everything */
    glm::mat4 projMatrix = glm::ortho(0.0f, (float)gWindowState.winWidth, (float)gWindowState.winHeight,
        0.0f, renderHelpers::RenderHelper::MAX_LAYERS, 0.0f);
    gRenderInstance.setProjectionMatrix(projMatrix);

    gRootConcreteNode.gMesh.gColor = utils::hexToVec4("#6ebcb4");

    gFpNode.gMesh.gColor = utils::hexToVec4("#16796F");
    gFpNode.gStyle.gBorderSize = glm::vec4(4, 4, 4, 4);
    gFpNode.gStyle.gBorderColor = utils::hexToVec4("#349798");

    gStatusNode.gMesh.gColor = utils::hexToVec4("#16796F");
    gStatusNode.gStyle.gBorderSize = glm::vec4(4, 4, 4, 4);
    gStatusNode.gStyle.gBorderColor = utils::hexToVec4("#349798");

    gExtractNode.gMesh.gColor = utils::hexToVec4("#16796F");
    gExtractNode.gStyle.gBorderSize = glm::vec4(4, 4, 4, 4);
    gExtractNode.gStyle.gBorderColor = utils::hexToVec4("#349798");

    keepRatio();

    /* Everything parented to root rectangle will share this one unique window data source*/
    gRootConcreteNode.setStateSource(&gWindowState);

    /* Push Child to root node */
    gRootConcreteNode.append(&gFpNode);
    gRootConcreteNode.append(&gExtractNode);
    gRootConcreteNode.append(&gStatusNode);

    /* Enable FTS for quicker click/mouse movement searches in the internal tree struct */
    gRootConcreteNode.enableFastTreeSort();
    gRootConcreteNode.updateFastTree();

    gRootConcreteNode.registerOnClick([](int, int x, int y)
        {
            printf("Hello %d, %d !\n", x, y);
        });

    gRootConcreteNode.registerOnRelease([](int, int x, int y)
        {
            printf("Release %d, %d !\n", x, y);
        });

    gExtractNode.registerOnClick([this](int, int x, int y)
        {
            gExtractNode.gStyle.gBorderColor = utils::hexToVec4("#8dd9f0");
            gExtractNode.gStyle.gBorderSize.x += 3;
            gExtractNode.gStyle.gBorderSize.z += 3;
        });

    gExtractNode.registerOnRelease([this](int, int x, int y)
        {
            gExtractNode.gStyle.gBorderColor = utils::hexToVec4("#349798");
            gExtractNode.gStyle.gBorderSize.x -= 3;
            gExtractNode.gStyle.gBorderSize.z -= 3;
        });

    printf("Root level is: %d and z: %f\n", gRootConcreteNode.gTreeStruct.getLevel(), gRootConcreteNode.gMesh.gBox.pos.z);
    printf("Top child level is: %d and z: %f\n", gFpNode.gTreeStruct.getLevel(), gFpNode.gMesh.gBox.pos.z);
    printf("Bot border level is: %d and z: %f\n", gExtractNode.gTreeStruct.getLevel(), gExtractNode.gMesh.gBox.pos.z);
}

void Application::loop()
{
    if (gReloadShader)
    {
        printf("Should reload now..\n");
        gShInstance.reloadFromPath(gBorderedVertPath, gBorderedFragPath);
        // gShInstance.reloadFromPath(gVertPath, gFragPath);
        gReloadShader = false;
    }

    /* Render stuff, order independent (depends only on Z) */
    gRenderInstance.clearScreen();
    gRenderInstance.renderRectMesh(gRootConcreteNode.gMesh, gRootConcreteNode.gStyle);
    gRenderInstance.renderRectMesh(gFpNode.gMesh, gFpNode.gStyle);
    gRenderInstance.renderRectMesh(gStatusNode.gMesh, gStatusNode.gStyle);
    gRenderInstance.renderRectMesh(gExtractNode.gMesh, gExtractNode.gStyle);
}

void Application::setTitle(const std::string& title)
{
    /*This shall be abstracted by the window helpers eventually */
    glfwSetWindowTitle(gWindowHandle, title.c_str());
}

/* -------------- Window event propagation zone --------------- */
void Application::onKeyPress(int key, int sc, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        gReloadShader = true;
    }
}

void Application::onWindowResize(int width, int height)
{
    gWindowState.winWidth = width;
    gWindowState.winHeight = height;

    glm::mat4 projMatrix = glm::ortho(0.0f, (float)width, (float)height,
        0.0f, renderHelpers::RenderHelper::MAX_LAYERS, 0.0f);
    gRenderInstance.setProjectionMatrix(projMatrix);

    /* glViewport is needed after changing the ortho matrix change or else
       the NDC coordinates will not be mapped correctly to screen coordinates. */
    glViewport(0, 0, width, height);

    keepRatio();
}

void Application::onButtonAction(int button, int action, int mods)
{
    gWindowState.mouseClicked = action == GLFW_PRESS ? true : false;
    gWindowState.button = button; /*Left, Right, Middle, etc */
    gRootConcreteNode.emitEvent(inputHelpers::Event::MouseButton);
}

void Application::onMouseMoveAction(double xPos, double yPos)
{
    gWindowState.mouseX = xPos;
    gWindowState.mouseY = yPos;
    gRootConcreteNode.emitEvent(inputHelpers::Event::MouseMove);
}

