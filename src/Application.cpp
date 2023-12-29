#include "Application.hpp"

#include <GLFW/glfw3.h>

Application::Application(GLFWwindow* windowHandle)
    : gWindowHandle{ windowHandle }
    , gShInstance{ shaderHelpers::ShaderHelper::get() }
    , gRenderInstance{ renderHelpers::RenderHelper::get() }
{}


void Application::keepRatio()
{
    /* Root note */
    auto& mesh = gRootConcreteNode.gMesh;
    mesh.gBox.pos.x = 0;
    mesh.gBox.pos.y = 0;
    mesh.gBox.scale.x = gWindowState.winWidth;
    mesh.gBox.scale.y = gWindowState.winHeight;

    /* 1st child of root */
    const int32_t spacingPx = 20;
    const float spaceFromBottomPct = 0.25f;

    auto& cMesh = gChildNode.gMesh;
    cMesh.gBox.pos.x = mesh.gBox.pos.x + spacingPx;
    cMesh.gBox.pos.y = (mesh.gBox.pos.y + mesh.gBox.scale.y) - mesh.gBox.scale.y * spaceFromBottomPct;
    cMesh.gBox.scale.x = mesh.gBox.scale.x - spacingPx * 2;
    cMesh.gBox.scale.y = mesh.gBox.scale.y * spaceFromBottomPct - spacingPx;
}

void Application::setTitle(const std::string& title)
{
    /*This shall be abstracted by the window helpers eventually */
    glfwSetWindowTitle(gWindowHandle, title.c_str());
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

    gRootConcreteNode.gMesh.gColor.r = 1.0f;
    gRootConcreteNode.gMesh.gColor.a = 1.0f;

    gChildNode.gMesh.gColor.a = 1.0f;
    gChildNode.gMesh.gColor.g = 1.0f;

    keepRatio();

    /* Everything parented to root rectangle will share this one unique window data source*/
    gRootConcreteNode.setStateSource(&gWindowState);

    gRootConcreteNode.gTreeStruct.enableFastTreeSearch();

    /* Push Child to root node */
    gRootConcreteNode.append(&gChildNode);

    gRootConcreteNode.registerOnClick([](int, int x, int y)
        {
            printf("Hello %d, %d !\n", x, y);
        });

    gRootConcreteNode.registerOnRelease([](int, int x, int y)
        {
            printf("Release %d, %d !\n", x, y);
        });


    printf("Root level is: %d and z: %f\n", gRootConcreteNode.gTreeStruct.getLevel(), gRootConcreteNode.gMesh.gBox.pos.z);
    printf("Child level is: %d and z: %f\n", gChildNode.gTreeStruct.getLevel(), gChildNode.gMesh.gBox.pos.z);
}

void Application::loop()
{
    if (gReloadShader)
    {
        printf("Should reload now..\n");
        gShInstance.reloadFromPath(gVertPath, gFragPath);
        gReloadShader = false;
    }

    gRenderInstance.clearScreen();
    gRenderInstance.renderRectMesh(gRootConcreteNode.gMesh);
    gRenderInstance.renderRectMesh(gChildNode.gMesh);
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
    // printf("Window resized to %dx%d\n", width, height);
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

