#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../OpenGL_Textures/Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void calculateTriangles(glm::vec2 A, glm::vec2 B, glm::vec2 C, int n); \
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
glm::vec2 midpoint(glm::vec2 A, glm::vec2 B);

float mixScale = 0.0f;
int triangleIterations = 0;
int sierpinskiCount = 0;

std::vector<float> allVertices;

float vertices[] = {
    // positions        // texture coords
     0.0f,  0.8f, 0.0f,   0.5f, 1.2f, // top
     -0.8f, -0.8f, 0.0f,   0.0f, 0.0f, // bottom left
     0.8f, -0.8f, 0.0f,   1.0f, 0.0f, // bottom right
};

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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Introduction To OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register the callback function to adjust the viewport when the window is resized
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ------------create shader program and object---------------------

    Shader Shader("vertexShader.vs", "fragmentShader.fs");

    // ---------------------Triangle setup------------------------------
    
    calculateTriangles(glm::vec2(vertices[0], vertices[1]), glm::vec2(vertices[5], vertices[6]), glm::vec2(vertices[10], vertices[11]), sierpinskiCount);

    unsigned int VBO, VAO, EBO;
    // glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(float), &allVertices[0], GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // Texture cord attibute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ----------------- TEXTURE SETUP --------------------------
    unsigned int texture1, texture2;

    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // set texture filtering to nearest neighbor to clearly see the texels/pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // ------- Load Image 1 --------- 
    int width, height, nrChannels;
    unsigned char* data = stbi_load("WoodenContainer.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // ------- Load Image 2 --------- 
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // set texture filtering to nearest neighbor to clearly see the texels/pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            // RGBA if using PNG cuz it has alpha values
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    stbi_image_free(data);

    Shader.use(); // don't forget to activate/use the shader before setting uniforms!
    Shader.setInt("texture1", 0);
    Shader.setInt("texture2", 1);

    // -------- Render Loop ------------------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        Shader.use();
        Shader.setFloat("mixScale", mixScale);

        // ------ TRI ONE ---------
        glm::mat4 trans = glm::mat4(1.0f);
        //trans = glm::translate(trans, glm::vec3(sin(glfwGetTime()) / 2.0f, cos(glfwGetTime()) / 2.0f, 1.0));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(1.0, 1.0, 1.0));

        unsigned int transformLoc = glGetUniformLocation(Shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(float), &allVertices[0], GL_STATIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, allVertices.size() / 5);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}

void calculateTriangles(glm::vec2 A, glm::vec2 B, glm::vec2 C, int n) 
{

    if (n < 0) return;

    if (n == 0 && triangleIterations == 0)
    {
        allVertices.push_back(A.x); allVertices.push_back(A.y); allVertices.push_back(0.0f); allVertices.push_back(0.0f); allVertices.push_back(0.0f);
        allVertices.push_back(C.x); allVertices.push_back(C.y); allVertices.push_back(0.0f); allVertices.push_back(1.0f); allVertices.push_back(0.0f);
        allVertices.push_back(B.x); allVertices.push_back(B.y); allVertices.push_back(0.0f); allVertices.push_back(0.5f); allVertices.push_back(1.0f);

        return;
    }
    else
    {
        triangleIterations++;
    }
   
    glm::vec2 abMid = midpoint(A, B);
    glm::vec2 bcMid = midpoint(B, C);
    glm::vec2 acMid = midpoint(A, C);
     
    //__X______________________________Y_______________________________Z_________________________TEXTURE_COORDS___________________________________________
    allVertices.push_back(abMid.x); allVertices.push_back(abMid.y); allVertices.push_back(0.0f); allVertices.push_back(0.0f); allVertices.push_back(0.0f);
    allVertices.push_back(acMid.x); allVertices.push_back(acMid.y); allVertices.push_back(0.0f); allVertices.push_back(1.0f); allVertices.push_back(0.0f);
    allVertices.push_back(bcMid.x); allVertices.push_back(bcMid.y); allVertices.push_back(0.0f); allVertices.push_back(0.5f); allVertices.push_back(1.0f);

    calculateTriangles(abMid, bcMid, B, n - 1);
    calculateTriangles(A, acMid, abMid, n - 1);
    calculateTriangles(acMid, C, bcMid, n - 1);

    
}

glm::vec2 midpoint(glm::vec2 A, glm::vec2 B)
{
    return (A + B) * 0.5f;
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
    {
        mixScale += 0.01f;
        if (mixScale >= 1.0f)
            mixScale = 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        mixScale -= 0.01f;
        if (mixScale <= 0.0f)
            mixScale = 0.0f;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        allVertices.clear();
        triangleIterations = 0;
        sierpinskiCount++;
        calculateTriangles(glm::vec2(vertices[0], vertices[1]), glm::vec2(vertices[5], vertices[6]), glm::vec2(vertices[10], vertices[11]), sierpinskiCount);
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        allVertices.clear();
        triangleIterations = 0;
        sierpinskiCount--;
        calculateTriangles(glm::vec2(vertices[0], vertices[1]), glm::vec2(vertices[5], vertices[6]), glm::vec2(vertices[10], vertices[11]), sierpinskiCount);
    }
}

