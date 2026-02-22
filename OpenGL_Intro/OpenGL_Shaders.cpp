#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../OpenGL_Shaders/Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // 
    // BLUE
    float vertices[] = {
        // Triangle Left H   // Colors
        -0.8f, -0.4f, 0.0f,  1.0f, 0.0f, 0.0f, // BL
        -0.6f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f, // BR
        -0.8f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f, // TL

        // Triangle Right H
        -0.2f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f, // BL
        -0.0f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, // BR
        -0.2f, 0.4f, 0.0f,  0.0f, 0.0f, 1.0f, // TL

        // Triangle Bridge 
        -0.6f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, // BL
        -0.2f, 0.1f, 0.0f,  0.0f, 1.0f, 0.0f, // TR
        -0.6f, 0.1f, 0.0f,  0.0f, 0.0f, 1.0f, // TL

        // Triangle I
        0.3f, -0.4f, 0.0f,  1.0f, 0.0f, 0.0f, // BL
        0.5f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f, // BR
        0.3f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f, // TL



        // Triangle Left H 
        -0.8f, 0.4f, 0.0f,  1.0f, 0.0f, 0.0f, // TL 
        -0.6f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, // BR
        -0.6f, 0.4f, 0.0f,  0.0f, 0.0f, 1.0f, // TR

        // Triangle Right H 
        -0.2f, 0.4f, 0.0f,  1.0f, 0.0f, 0.0f, // TL 
        -0.0f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, // BR
        -0.0f, 0.4f, 0.0f,  0.0f, 0.0f, 1.0f, // TR

        // Triangle Bridge 
        -0.6f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, // BL
        -0.2f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, // BR
        -0.2f, 0.1f, 0.0f,  0.0f, 0.0f, 1.0f, // TR

        // Triangle I
        0.3f, 0.4f, 0.0f,  1.0f, 0.0f, 0.0f,// TL 
        0.5f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, // BR
        0.5f, 0.4f, 0.0f,  0.0f, 0.0f, 1.0f, // TR
    };

    //unsigned int indices[] = {  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3    // second triangle
    //};

    GLFWwindow* window = glfwCreateWindow(800, 600, "Introduction To OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader Shader("vertexShader.vs", "fragmentShader.fs");

    //unsigned int EBO;
    //glGenBuffers(1, &EBO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // First triangle setup
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register the callback function to adjust the viewport when the window is resized


    while (!glfwWindowShouldClose(window)) // render loop
    {
        processInput(window);

        float xPosValue = sin(glfwGetTime()) / 2.0f;
        float yPosValue = cos(glfwGetTime()) / 2.0f;
        float alphaValue = sin(glfwGetTime()) + 1.0f;
        float scale = sin(glfwGetTime()) / 2.0f + 2.0f;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Shader.use();
        Shader.setFloat("xPos", xPosValue);
        Shader.setFloat("yPos", yPosValue);
        Shader.setFloat("alphaValue", alphaValue);
        Shader.setFloat("scale", scale);

        //glUseProgram(shaderProgram); // this sets the current shader program to be used for rendering

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 24);


        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

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
}

