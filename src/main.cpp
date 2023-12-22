#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shaderHelpers/ShaderHelper.hpp"
#include "meshHelpers/MeshBuilder.hpp"
#include "meshHelpers/RectMesh.hpp"

bool shouldReload = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        shouldReload = true;
    }
}

int main()
{
    constexpr int32_t windowWidth = 1280;
    constexpr int32_t windowHeight = 720;

    /* Init glfw */
    if (GLFW_FALSE == glfwInit())
    {
        perror("Something happened while trying to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwSwapInterval(0); // zero to disable Vsync

    /* Create mock window just to succeed initializing glew*/
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "MyWindow", NULL, NULL);
    if (window == NULL)
    {
        perror("Failed to create glew initializing window\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    /* Init glew after glfw (we need to have a valid context bound first) */
    if (glewInit() != GLEW_OK)
    {
        perror("GLEW failed to initialize\n");
        return false;
    }

    /* Set key callback */
    glfwSetKeyCallback(window, keyCallback);

    std::string vertPath{ "src/assets/shaders/basicV.glsl" };
    std::string fragPath{ "src/assets/shaders/basicF.glsl" };

    shaderHelpers::ShaderHelper shaderHelper_;
    int32_t shaderId = shaderHelper_.loadFromPath(vertPath, fragPath);

    meshHelpers::MeshBuilder meshBuilders_;

    std::vector<float> vertices = {
        // POS 3F         TEX 2F
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f

        // 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        // 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    //    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    std::vector<uint32_t> indices = {
        0, 1, 3,
        1, 2, 3
    };

    std::vector<uint32_t> layout = { 3,2 };

    uint32_t vaoId = meshBuilders_.generateWith(vertices, indices, layout);

    meshHelpers::RectMesh rectMesh_{ shaderId, vaoId };
    rectMesh_.box_.scale.x = 600;
    rectMesh_.box_.scale.y = 600;
    rectMesh_.box_.pos.x = 0; windowWidth / 2 - rectMesh_.box_.scale.x / 2;
    rectMesh_.box_.pos.y = 0; windowHeight / 2 - rectMesh_.box_.scale.y / 2;

    shaderHelper_.setActiveShaderId(rectMesh_.shaderId_);

    glBindVertexArray(rectMesh_.vaoId_);

    glm::mat4 projMatrix = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight,
        0.0f, 0.0f, 100.0f);

    shaderHelper_.setMatrix4("projMatrix", projMatrix);
    shaderHelper_.setMatrix4("modelMatrix", rectMesh_.getTransform());
    shaderHelper_.setVec3f("res", rectMesh_.box_.scale);

    while (!glfwWindowShouldClose(window))
    {
        if (shouldReload)
        {
            printf("Should reload now..\n");
            shaderId = shaderHelper_.loadFromPath(vertPath, fragPath);
            rectMesh_.shaderId_ = shaderId;
            shaderHelper_.setActiveShaderId(shaderId);
            shaderHelper_.setMatrix4("projMatrix", projMatrix);
            shaderHelper_.setVec3f("res", rectMesh_.box_.scale);

            shouldReload = false;
        }

        glClearColor(.3f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const double ampl = 10.0f;
        const double freq = 1.0f;
        const double sinTime = ampl * std::sin(glfwGetTime() * freq);

        shaderHelper_.setMatrix4("modelMatrix", rectMesh_.getTransform());
        shaderHelper_.setVec1f("uTime", glfwGetTime());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* Free no longer needed init window. User must make sure now there's a context bound
       by him before using any glew/glfw API calls
    */
    glfwDestroyWindow(window);
    glfwTerminate();
}