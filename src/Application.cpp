#include "Application.hpp"

#include <filesystem>

#include <GLFW/glfw3.h>
#include <GL/glew.h>

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

    /* 3rd child of root */
    auto& extractMesh = gExtractNode.gMesh;
    extractMesh.gBox.pos.x = statusMesh.gBox.pos.x;
    extractMesh.gBox.pos.y = statusMesh.gBox.pos.y + statusMesh.gBox.scale.y + spacingPx;
    extractMesh.gBox.scale.x = statusMesh.gBox.scale.x;
    extractMesh.gBox.scale.y = rootMesh.gBox.scale.y * extractScale - spacingPx * 2;

    /* 1st child of top */
    auto& textMesh = gPathTextNode.gMesh;
    textMesh.gBox.pos.x = fpMesh.gBox.pos.x + spacingPx;
    textMesh.gBox.pos.y = fpMesh.gBox.pos.y + spacingPx;
    textMesh.gBox.scale.x = fpMesh.gBox.scale.x - spacingPx * 2;
    textMesh.gBox.scale.y = fpMesh.gBox.scale.y - spacingPx * 2;

    /* 1st child of status */
    auto& statusTextMesh = gStatusTextNode.gMesh;
    statusTextMesh.gBox.pos.x = statusMesh.gBox.pos.x + spacingPx;
    statusTextMesh.gBox.pos.y = statusMesh.gBox.pos.y + spacingPx;
    statusTextMesh.gBox.scale.x = statusMesh.gBox.scale.x - spacingPx * 2;
    statusTextMesh.gBox.scale.y = statusMesh.gBox.scale.y - spacingPx * 2;

    /* 1st child of status */
    auto& extractTextMesh = gExtractTextNode.gMesh;
    extractTextMesh.gBox.pos.x = extractMesh.gBox.pos.x + spacingPx;
    extractTextMesh.gBox.pos.y = extractMesh.gBox.pos.y + spacingPx;
    extractTextMesh.gBox.scale.x = extractMesh.gBox.scale.x - spacingPx * 2;
    extractTextMesh.gBox.scale.y = extractMesh.gBox.scale.y - spacingPx * 2;
}

void Application::setup()
{
    printf("Setup called\n");
    setTitle("UnSnapshot C++ 1.0");

    glfwGetWindowSize(gWindowHandle, &gWindowState.winWidth, &gWindowState.winHeight);

    /* Note: Highest Z renders in front of everything */
    glm::mat4 projMatrix = glm::ortho(0.0f, (float)gWindowState.winWidth, (float)gWindowState.winHeight,
        0.0f, renderHelpers::RenderHelper::MAX_LAYERS, 0.0f);
    gRenderInstance.setProjectionMatrix(projMatrix);

    /* Basic color and style */
    gRootConcreteNode.gMesh.gColor = utils::hexToVec4("#6ebcb4");
    gRootConcreteNode.gStyle.gBorderSize = glm::vec4(4, 4, 4, 4);
    gRootConcreteNode.gStyle.gBorderColor = utils::hexToVec4("#349798");

    gFpNode.gMesh.gColor = utils::hexToVec4("#16796F");
    gFpNode.gStyle.gBorderSize = glm::vec4(4, 4, 4, 4);
    gFpNode.gStyle.gBorderColor = utils::hexToVec4("#349798");

    gStatusNode.gMesh.gColor = utils::hexToVec4("#16796F");
    gStatusNode.gStyle.gBorderSize = glm::vec4(4, 4, 4, 4);
    gStatusNode.gStyle.gBorderColor = utils::hexToVec4("#349798");

    gExtractNode.gMesh.gColor = utils::hexToVec4("#16796F");
    gExtractNode.gStyle.gBorderSize = glm::vec4(4, 4, 4, 4);
    gExtractNode.gStyle.gBorderColor = utils::hexToVec4("#349798");

    // gStatusTextNode.gMesh.gColor = utils::hexToVec4("#aa1a1a");
    // gStatusTextNode.gMesh.gColor.a = 1.0f;

    /* Uniform Watchers */
    gRootConcreteNode.gMesh.gUniKeeper.watch("uInnerColor", &gRootConcreteNode.gMesh.gColor);
    gRootConcreteNode.gMesh.gUniKeeper.watch("uResolution", &gRootConcreteNode.gMesh.gBox.scale);
    gRootConcreteNode.gMesh.gUniKeeper.defaultVec4("uBorderColor");
    gRootConcreteNode.gMesh.gUniKeeper.defaultVec4("uBorderSize");

    gExtractNode.gMesh.gUniKeeper.watch("uInnerColor", &gExtractNode.gMesh.gColor);
    gExtractNode.gMesh.gUniKeeper.watch("uBorderColor", &gExtractNode.gStyle.gBorderColor);
    gExtractNode.gMesh.gUniKeeper.watch("uBorderSize", &gExtractNode.gStyle.gBorderSize);
    gExtractNode.gMesh.gUniKeeper.watch("uResolution", &gExtractNode.gMesh.gBox.scale);

    gStatusNode.gMesh.gUniKeeper.watch("uInnerColor", &gStatusNode.gMesh.gColor);
    gStatusNode.gMesh.gUniKeeper.watch("uResolution", &gStatusNode.gMesh.gBox.scale);
    gStatusNode.gMesh.gUniKeeper.defaultVec4("uBorderColor");
    gStatusNode.gMesh.gUniKeeper.defaultVec4("uBorderSize");

    gFpNode.gMesh.gUniKeeper.watch("uInnerColor", &gFpNode.gMesh.gColor);
    gFpNode.gMesh.gUniKeeper.watch("uResolution", &gFpNode.gMesh.gBox.scale);
    gFpNode.gMesh.gUniKeeper.defaultVec4("uBorderColor");
    gFpNode.gMesh.gUniKeeper.defaultVec4("uBorderSize");

    //TODO: not really hot?
    // gFpNode.gStyle.gTextureId = gTexHelperInstance.loadTexture("src/assets/textures/imeg.jpeg")->gId;
    // gFpNode.gMesh.gUniKeeper.watch("uTextureId", &gFpNode.gStyle.gTextureId);

    keepRatio();

    /* Everything parented to root rectangle will share this one unique window data source*/
    gRootConcreteNode.setStateSource(&gWindowState);

    /* Push Child to root node */
    gRootConcreteNode.append(&gFpNode);
    gRootConcreteNode.append(&gExtractNode);
    gRootConcreteNode.append(&gStatusNode);

    gFpNode.append(&gPathTextNode);
    gStatusNode.append(&gStatusTextNode); //TODO: Implement append such that we CANNOT APPEND OURSELVES
    gExtractNode.append(&gExtractTextNode);

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

    const int32_t btnPushAmt = 3;
    gExtractNode.registerOnClick([this, btnPushAmt](int, int, int)
        {
            gExtractNode.gStyle.gBorderColor = utils::hexToVec4("#8dd9f0");
            gExtractNode.gStyle.gBorderSize.x += btnPushAmt;
            gExtractNode.gStyle.gBorderSize.z += btnPushAmt;
        });

    gExtractNode.registerOnRelease([this, btnPushAmt](int, int, int)
        {
            gExtractNode.gStyle.gBorderColor = utils::hexToVec4("#349798");
            gExtractNode.gStyle.gBorderSize.x -= btnPushAmt;
            gExtractNode.gStyle.gBorderSize.z -= btnPushAmt;
        });

    gExtractNode.registerOnMouseEnter([this](int, int)
        {
            gExtractNode.gMesh.gColor = utils::hexToVec4("#136b63");
        });

    gExtractNode.registerOnMouseExit([this](int, int)
        {
            gExtractNode.gMesh.gColor = utils::hexToVec4("#16796F");
        });

    gPathTextNode.registerOnItemsDrop([this](int32_t count, const char** paths)
        {
            std::string x;
            for (int i = 0; i < count; i++)
            {
                x.append(paths[i]);
                x.append(" - ");
                printf("Path dropped: %s\n", paths[i]);
            }
            gPathTextNode.setText(std::move(x));
        });

    gPathTextNode.alignTextToCenter(true);
    gStatusTextNode.alignTextToCenter(true);
    gExtractTextNode.alignTextToCenter(true);
    gExtractTextNode.setEventTransparent(true);

    gPathTextNode.setText("Drag snapshot zip here");
    gStatusTextNode.setText("Status: Ready");
    gExtractTextNode.setText("Extract");
}

void Application::loop()
{
    if (gReloadShader)
    {
        printf("Should reload now..\n");
        // gShInstance.reloadFromPath(gBorderedVertPath, gBorderedFragPath);
        gShInstance.reloadFromPath(
            "src/assets/shaders/textV.glsl",
            "src/assets/shaders/textF.glsl");
        gReloadShader = false;
        gPathTextNode.setText("Reloaded..");
    }

    /* Render stuff, order independent (depends only on Z) */
    gRenderInstance.clearScreen();
    gRenderInstance.renderRectNode(gRootConcreteNode);
    gRenderInstance.renderRectNode(gFpNode);
    gRenderInstance.renderRectNode(gStatusNode);
    gRenderInstance.renderRectNode(gExtractNode);
    gRenderInstance.renderRectNode(gPathTextNode);
    gRenderInstance.renderRectNode(gStatusTextNode); //TODO: Avoid the possibility to render same node twice
    gRenderInstance.renderRectNode(gExtractTextNode);
}

void Application::setTitle(const std::string& title)
{
    /*This shall be abstracted by the window helpers eventually */
    glfwSetWindowTitle(gWindowHandle, title.c_str());
}

/* -------------- Window event propagation zone --------------- */
void Application::onKeyPress(int key, int, int action, int)
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
    gRootConcreteNode.emitEvent(inputHelpers::Event::WindowResize);
}

void Application::onButtonAction(int button, int action, int)
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

void Application::onMouseDrop(int dropCount, const char** paths)
{
    gWindowState.droppedPaths = paths;
    gWindowState.dropCount = dropCount;
    gRootConcreteNode.emitEvent(inputHelpers::Event::ItemsDrop);
    gWindowState.droppedPaths = nullptr;
    gWindowState.dropCount = 0;
}


