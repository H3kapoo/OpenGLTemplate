#include "Application.hpp"

#include <GLFW/glfw3.h>

Application::Application(GLFWwindow* windowHandle)
    : gWindowHandle{ windowHandle }
    , gShInstance{ shaderHelpers::ShaderHelper::get() }
    , gRenderInstance{ renderHelpers::RenderHelper::get() }
{}

void Application::setup()
{
    printf("Setup called\n");
    int windowWidth, windowHeight;
    glfwGetWindowSize(gWindowHandle, &windowWidth, &windowHeight);

    /* Note: Highest Z renders in front of everything */
    glm::mat4 projMatrix = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight,
        0.0f, -100.0f, 0.0f);
    gRenderInstance.setProjectionMatrix(projMatrix);

    auto& mesh = gRootConcreteNode.gMesh;
    mesh.gColor.r = 1.0f;
    mesh.gColor.a = 1.0f;

    mesh.gBox.scale.x = 400;
    mesh.gBox.scale.y = 400;
    mesh.gBox.pos.x = 0;
    mesh.gBox.pos.y = 0;
    mesh.gBox.pos.z = 1;

    /* Everything parented to root rectangle will share this one unique window data source*/
    gRootConcreteNode.setStateSource(&gWindowState);

    /* Push Child to root node */
    gChildNode.gMesh.gBox.pos = gRootConcreteNode.gMesh.gBox.pos;
    gChildNode.gMesh.gBox.scale = gRootConcreteNode.gMesh.gBox.scale / 2.0f;
    gChildNode.gMesh.gColor.a = 1.0f;
    gChildNode.gMesh.gColor.g = 1.0f;
    gRootConcreteNode.gTreeStruct.append(&gChildNode);

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

void Application::onKeyPress(int key, int sc, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        gReloadShader = true;
    }
}

void Application::onWindowResize(int width, int height)
{
    printf("Window resized to %dx%d\n", width, height);
    glm::mat4 projMatrix = glm::ortho(0.0f, (float)width, (float)height,
        0.0f, 0.0f, -100.0f);
    gRenderInstance.setProjectionMatrix(projMatrix);

    /* glViewport is needed after changing the ortho matrix change or else
       the NDC coordinates will not be mapped correctly to screen coordinates. */
    glViewport(0, 0, width, height);

    auto& mesh = gRootConcreteNode.gMesh;
    mesh.gBox.pos.x = width / 2 - mesh.gBox.scale.x / 2;
    mesh.gBox.pos.y = height / 2 - mesh.gBox.scale.y / 2;
}

void Application::onButtonAction(int button, int action, int mods)
{
    gWindowState.mouseClicked = action == GLFW_PRESS ? true : false;
    gRootConcreteNode.emitEvent(inputHelpers::Event::MouseButton);
}

void Application::onMouseMoveAction(double xPos, double yPos)
{
    gWindowState.mouseX = xPos;
    gWindowState.mouseY = yPos;
    gRootConcreteNode.emitEvent(inputHelpers::Event::MouseMove);
}

