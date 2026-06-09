#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../OpenGL_Camera/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 1000;

float mixScale = 0.0f;
float lastX = 600, lastY = 500;
bool firstMouse = true;

glm::vec3 lightPosCube(20.0f, 0.0f, -20.0f);

Camera camera;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    -8.0f,  -0.0f,-8.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     8.0f,  -0.0f,-8.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     8.0f,  -0.0f, 8.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     8.0f,  -0.0f, 8.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -8.0f,  -0.0f, 8.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -8.0f,  -0.0f,-8.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

    // ------------ Texture ----------

    unsigned int diffuseMap = loadTexture("containerwithborder.png");
    unsigned int specularMap = loadTexture("container2_specular.png");
    unsigned int emissionMap = loadTexture("matrix.jpg");

    // ---------------------Cube setup------------------------------

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // light normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer),
    // no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
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

        // Calculate the color of the light 
        glm::vec3 light = glm::vec3(1.0f, 1.0f, 1.0f);
        light.x = (sin(glfwGetTime() * 1.8f) + 1.0f) * 0.5f;
        light.y = (sin(glfwGetTime() * 0.6f) + 1.0f) * 0.5f;
        light.z = (sin(glfwGetTime() * 1.2f) + 1.0f) * 0.5f;

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // bind emission map
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap);

        // ==========================================
        // 1. Draw the Light Source
        // ==========================================

        lightingShader.use();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("lightCol", light);

        glBindVertexArray(lightVAO);
        glm::mat4 model = glm::mat4(1.0f);

        // calculate the current angle to rotate
        float rotationSpeed = 1.0f;
        currentAngleDegrees += rotationSpeed * deltaTime;
        if (currentAngleDegrees >= 360)
        {
            currentAngleDegrees = 0;
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
        colorShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        colorShader.setInt("material.diffuse", 0);
        colorShader.setInt("material.specular", 1);
        colorShader.setInt("material.emission", 2);
        colorShader.setFloat("material.shininess", 64.0f);
        colorShader.setFloat("time", glfwGetTime() * 0.2);

        colorShader.setVec3("light.ambient", light * glm::vec3(0.2f));
        colorShader.setVec3("light.diffuse", light * glm::vec3(0.5f));
        colorShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        colorShader.setVec3("light.position", lightPosition); // update the light position data in the shader

        colorShader.setMat4("view", view);
        colorShader.setMat4("projection", projection);

        glBindVertexArray(cubeVAO);
        colorShader.setVec3("viewPos", camera.getPosition());

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
        colorShader.setInt("material.diffuse", 0);
        colorShader.setInt("material.specular", 1);
        colorShader.setFloat("material.shininess", 32.0f);

        colorShader.setVec3("light.ambient", light * glm::vec3(0.2f));
        colorShader.setVec3("light.diffuse", light * glm::vec3(0.5f));
        colorShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        colorShader.setVec3("light.position", lightPosition); // update the light position data in the shader

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

unsigned int loadTexture(char const * path)
{

    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, numComponents;
    unsigned char* data = stbi_load(path, &width, &height, &numComponents, 0);

    if (data)
    {
        
        GLenum format;
        if (numComponents == 1) format = GL_RED;
        if (numComponents == 3) format = GL_RGB;
        if (numComponents == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return texture;
}