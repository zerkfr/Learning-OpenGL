#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../OpenGL_Camera/Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 1000;

float mixScale = 0.0f;
float lastX = 600, lastY = 500;
bool firstMouse = true;

glm::vec3 lightPosCube(20.0f, 0.0f, -20.0f);

Camera camera;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,

   -8.0f, -0.0f, -8.0f,  0.0f,  1.0f, 0.0f,
    8.0f, -0.0f, -8.0f,  0.0f,  1.0f, 0.0f,
    8.0f, -0.0f,  8.0f,  0.0f,  1.0f, 0.0f,
    8.0f, -0.0f,  8.0f,  0.0f,  1.0f, 0.0f,
    -8.0f, -0.0f,  8.0f,  0.0f,  1.0f, 0.0f,
    -8.0f, -0.0f, -8.0f,  0.0f,  1.0f, 0.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(3.0f, 0.0f, -5.0f),
    glm::vec3(0.0f, -5.0f, 0.0f),
};

glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 25.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float currentAngleDegrees = 0.0f;

int main()
{

    if (!glfwInit())
    {
        std::cout << "GLFW failed to initialize\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200, 1000, "Introduction To OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register the callback function to adjust the viewport when the window is resized
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ------------create shader program and object---------------------

    Shader colorShader("colorCube.vs", "colorCube.fs");
    Shader lightingShader("lightCube.vs", "lightCube.fs");

    // ---------------------Cube setup------------------------------

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // light normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer),
    // no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // -------- Render Loop ------------------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Combined clear calls
        glEnable(GL_DEPTH_TEST);

        // Common View and Projection matrices for this frame
        
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjMatrix(float(WIDTH), float(HEIGHT), 0.1f, 100.0f);

        // ==========================================
        // 1. Draw the Light Source
        // ==========================================
        lightingShader.use();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);

        glBindVertexArray(lightVAO);
        glm::mat4 model = glm::mat4(1.0f);

        // calculate the current angle to rotate
        float rotationSpeed = 2.0f;
        currentAngleDegrees += rotationSpeed * deltaTime;
        if (currentAngleDegrees >= 360)
        {
            currentAngleDegrees -= 360.0f;
        }

        model = glm::scale(model, glm::vec3(0.2f));
        model = glm::translate(model, cubePositions[0]);
        model = glm::rotate(model, currentAngleDegrees, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, lightPosCube);

        glm::vec3 lightPosition = glm::vec3(model[3]); // the extracted position of lightCube from the model matrix
        lightingShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ==========================================
        // 2. Draw the Lit Object
        // ==========================================

        colorShader.use();
        colorShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        colorShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        colorShader.setVec3("lightPos", lightPosition); // update the light position data in the shader

        colorShader.setMat4("view", view);
        colorShader.setMat4("projection", projection);

        glBindVertexArray(cubeVAO);
        colorShader.setVec3("viewPos", cameraPos);

        // draw many cubes
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, cubePositions[0] + glm::vec3(1.5 * i, 1.5 * j, 1.5 * k));
                    model = glm::rotate(model, currentAngleDegrees, glm::vec3(0.0f, 1.0f, 0.0f));
                    colorShader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        } 

        // ==========================================
        // 3. Draw the Lit Flooring
        // ==========================================

        colorShader.setVec3("objectColor", 0.5f, 0.2f, 0.88f);
        colorShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[2]);
        colorShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 36, 6);

        // glfw: swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processScrollInput(yoffset);
}

// Whenever the window size changed this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 9.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboardInput(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboardInput(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboardInput(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboardInput(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboardInput(UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.processKeyboardInput(DOWN, deltaTime);
}
