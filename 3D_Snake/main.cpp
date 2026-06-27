#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/random.hpp>
#include "../OpenGL_Camera/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void updateSnakePos();
void updateFoodSpaceAdd(glm::vec3 location, int index);
void updateFoodSpaceRemove(glm::vec3 location, int index);
void checkFoodCollision();
bool checkWallCollision();
void printGrid();
glm::vec3 generateRandFoodSpawn();

// define screen dimensions
const unsigned int HEIGHT = 720;
const unsigned int WIDTH = 1280;

// time and mouse variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = WIDTH / 2;
float lastY = WIDTH / 2;
bool firstMouse = true;
bool shiftPressed = false;
bool gameOver = false;

// snake
enum Direction
{
    SNAKE_LEFT,
    SNAKE_RIGHT,
    SNAKE_UP,
    SNAKE_DOWN
};
glm::vec3 getDirectionVectorFromInputBuffer(std::vector<Direction> buffer, int index);


Direction snakeDir = SNAKE_DOWN;
float snakeSpeedCounter = 15.0f;
glm::vec3 previousHeadPos;
glm::vec3 snakeDirection = glm::vec3(0.0f, 0.0f, 1.0f);
int currentFoodAmount = 0;
int startingFoodAmount = 5;
glm::vec3 snakeColor = glm::vec3(0.0f, 1.0f, 0.0f);
bool firstPressed = false;
bool moveActive = false;
std::vector<Direction> inputBuffer;

static bool movedRight, movedLeft, movedUp, movedDown = false;

const int platformLength = 16;
const int halfPlatformLength = 8;
const int totalCells = platformLength * platformLength;
int grid[platformLength][platformLength] = { 0 };

// create camera
Camera camera;

float vertices[] =
{
    // Front face
    - 1.5f, -1.5f, -1.5f,
     1.5f, -1.5f, -1.5f,
     1.5f,  1.5f, -1.5f,
     1.5f,  1.5f, -1.5f,
    -1.5f,  1.5f, -1.5f,
    -1.5f, -1.5f, -1.5f,

    // Back face
    -1.5f, -1.5f,  1.5f,
     1.5f, -1.5f,  1.5f,
     1.5f,  1.5f,  1.5f,
     1.5f,  1.5f,  1.5f,
    -1.5f,  1.5f,  1.5f,
    -1.5f, -1.5f,  1.5f,

    // Left face
    -1.5f,  1.5f,  1.5f,
    -1.5f,  1.5f, -1.5f,
    -1.5f, -1.5f, -1.5f,
    -1.5f, -1.5f, -1.5f,
    -1.5f, -1.5f,  1.5f,
    -1.5f,  1.5f,  1.5f,

    // Right face
     1.5f,  1.5f,  1.5f,
     1.5f,  1.5f, -1.5f,
     1.5f, -1.5f, -1.5f,
     1.5f, -1.5f, -1.5f,
     1.5f, -1.5f,  1.5f,
     1.5f,  1.5f,  1.5f,

    // Bottom face
    -1.5f, -1.5f, -1.5f,
     1.5f, -1.5f, -1.5f,
     1.5f, -1.5f,  1.5f,
     1.5f, -1.5f,  1.5f,
    -1.5f, -1.5f,  1.5f,
    -1.5f, -1.5f, -1.5f,

    // Top face
    -1.5f,  1.5f, -1.5f,
     1.5f,  1.5f, -1.5f,
     1.5f,  1.5f,  1.5f,
     1.5f,  1.5f,  1.5f,
    -1.5f,  1.5f,  1.5f,
    -1.5f,  1.5f, -1.5f,

    // Platform Face
    float(-halfPlatformLength + 0.5),  0.0f, float(-halfPlatformLength + 0.5),
     float(halfPlatformLength + 0.5),  0.0f, float(-halfPlatformLength + 0.5),
     float(halfPlatformLength + 0.5),  0.0f,  float(halfPlatformLength + 0.5),
     float(halfPlatformLength + 0.5),  0.0f,  float(halfPlatformLength + 0.5),
    float(-halfPlatformLength + 0.5),  0.0f,  float(halfPlatformLength + 0.5),
    float(-halfPlatformLength + 0.5),  0.0f, float(-halfPlatformLength + 0.5),

    // platform bottom
    float(-halfPlatformLength + 0.5),  -1.5f, float(-halfPlatformLength + 0.5),
     float(halfPlatformLength + 0.5),  -1.5f, float(-halfPlatformLength + 0.5),
     float(halfPlatformLength + 0.5),  -1.5f,  float(halfPlatformLength + 0.5),
     float(halfPlatformLength + 0.5),  -1.5f,  float(halfPlatformLength + 0.5),
    float(-halfPlatformLength + 0.5),  -1.5f,  float(halfPlatformLength + 0.5),
    float(-halfPlatformLength + 0.5),  -1.5f, float(-halfPlatformLength + 0.5),

    // x - green
    // platform front
    float(-halfPlatformLength + 0.5), -1.5f, float(halfPlatformLength + 0.5), 
     float(halfPlatformLength + 0.5), -1.5f, float(halfPlatformLength + 0.5), 
     float(halfPlatformLength + 0.5),  0.0f, float(halfPlatformLength + 0.5), 
     float(halfPlatformLength + 0.5),  0.0f, float(halfPlatformLength + 0.5),
    float(-halfPlatformLength + 0.5),  0.0f, float(halfPlatformLength + 0.5),
    float(-halfPlatformLength + 0.5), -1.5f, float(halfPlatformLength + 0.5)
};


glm::vec3 platformLocation = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 defaultFoodLocations[] =
{

    glm::vec3(1.0f, 0.5f, 0.0f),
    glm::vec3(-3.0f, 0.5f, -1.0f),
    glm::vec3(-4.0f, 0.5f, 1.0f),
    glm::vec3(-3.0f, 0.5f, -3.0f),
    glm::vec3(2.0f, 0.5f, 1.0f),

};

std::vector<glm::vec3> foodLocations;
std::vector<glm::vec3> snakePos;

int numSnakeParts = 5;

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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Snake", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // setup VBO/VAO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // setup shaders
    Shader platformShader("floorShader.vs", "floorShader.fs");
    Shader foodShader("foodShader.vs", "foodShader.fs");
    Shader snakeShader("snakeShader.vs", "snakeShader.fs");

    // set camera starting position and direction
    camera.setPosition(glm::vec3(0.0f, 16.828f, 10.0104));
    camera.setFront(glm::vec3(0.0f, -0.904827f, -0.42578f));

    // set default head pos
    snakePos.push_back(glm::vec3(0.0f, 0.2f, 0.0f));
    snakePos.push_back(glm::vec3(0.0f, 0.2f, -1.0f));
    snakePos.push_back(glm::vec3(0.0f, 0.2f, -2.0f));
    snakePos.push_back(glm::vec3(0.0f, 0.2f, -3.0f));
    snakePos.push_back(glm::vec3(0.0f, 0.2f, -4.0f));
    previousHeadPos = snakePos[0];

    // set default food spawns
    for (int i = 0; i < startingFoodAmount; i++)
    {
        glm::vec3 location = defaultFoodLocations[i];
        updateFoodSpaceAdd(location, i);
    }

    /*
    for (int i = 0; i < halfPlatformLength; i++)
    {
        for (int k = 0; k < halfPlatformLength; k++)
        {
            std::cout << grid[i][k] << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;

    */

    // -------- MAIN RENDER FUNCTION ---------
	while (!glfwWindowShouldClose(window))
	{

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // matrices for current frame
        glm::mat4 projection = camera.getProjMatrix(float(WIDTH), float(HEIGHT), 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        
        /*
        for (Direction dir : inputBuffer)
        {
            switch (dir)
            {
            case 0:
                std::cout << "LEFT ";
                break;
            case 1:
                std::cout << "RIGHT ";
                break;
            case 2:
                std::cout << "UP ";
                break;
            case 3:
                std::cout << "DOWN ";
                break;
            }
        }
        std::cout << std::endl;
        */

        checkFoodCollision();

        // send needed camera matrices to our shader
        platformShader.use();
        platformShader.setMat4("projection", projection);
        platformShader.setMat4("view", view);
        foodShader.use();
        foodShader.setMat4("projection", projection);
        foodShader.setMat4("view", view);
        snakeShader.use();
        snakeShader.setMat4("projection", projection);
        snakeShader.setMat4("view", view);
        snakeShader.setVec3("color", snakeColor);

        glm::vec3 deadColor = glm::vec3(1.0f, 0.0f, 0.0f) * glm::vec3(glm::sin(glfwGetTime()));
        if (gameOver)
        {
            snakeShader.setBool("dead", true);
            snakeShader.setVec3("deadColor", deadColor);
        }
        else
            snakeShader.setBool("dead", false);
        
        glBindVertexArray(VAO);

        // draw the platform 
        platformShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f));
        model = glm::translate(model, platformLocation);
        platformShader.setMat4("model", model);
        platformShader.setVec3("color", glm::vec3(0.0f, 0.0f, 0.12f));
        glDrawArrays(GL_TRIANGLES, 36, 18);

        foodShader.use();
        foodShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));

        // drawing foods
        float rotation = glm::radians(glfwGetTime() * 4.0f);

        for (int i = 0; i < currentFoodAmount; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, foodLocations[i]);
            model = glm::rotate(model, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.13));
            foodShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // drawing snake 
        snakeShader.use();

        for (int i = 0; i < numSnakeParts; i++)
        {
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, snakePos[i]);
            model = glm::scale(model, glm::vec3(float(1) / float(3)));
            snakeShader.setMat4("model", model);
            snakeShader.setVec3("color", snakeColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }

        // snake move 
        if (gameOver == false)
        {
            if (snakeSpeedCounter <= 0.0f && firstPressed)
            {

                if (checkWallCollision())
                {
                    gameOver = true;
                    std::cout << "You hit a wall\n";
                }

                for (int i = 4; i < numSnakeParts; i++) // start at 4, because you can't hit the first 3 parts of yourself
                {
                    if ((snakePos[0] + snakeDirection) == snakePos[i])
                    {
                        gameOver = true;
                        std::cout << "You hit yourself\n";
                        std::cout << "HEAD AT (" << snakePos[0].x + snakeDirection.x << ", " << snakePos[0].z + snakeDirection.z << ")\n";
                        std::cout << "HIT PART AT INDEX: " << i << " -- (" << snakePos[i].x << ", " << snakePos[i].z << ")\n\n";
                    }
                }

                updateSnakePos();

                // if the inputBuffer is not empty, add the direction from the first movement index in the buffer
                if (!inputBuffer.empty())
                {
                    glm::vec3 snakeBufferDirection = getDirectionVectorFromInputBuffer(inputBuffer, 0);
                    snakePos[0] += snakeBufferDirection;
                    inputBuffer.erase(inputBuffer.begin());
                }
                else // otherwise just use the stored location from the input function
                {
                    snakePos[0] += snakeDirection;
                }

                snakeSpeedCounter = 15.0f;
            }
            snakeSpeedCounter -= 1.0f;

            snakePos[0].x = glm::clamp(snakePos[0].x, (float(-platformLength / 2) + 1.0f), (float(platformLength / 2)));
            snakePos[0].z = glm::clamp(snakePos[0].z, (float(-platformLength / 2) + 1.0f), (float(platformLength / 2)));

            snakeShader.setVec3("pos", snakePos[0]);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

	}

    glfwTerminate();
    return 0;
}

glm::vec3 getDirectionVectorFromInputBuffer(std::vector<Direction> buffer, int index)
{

    switch (buffer[index])
    {
        case SNAKE_UP:
            return glm::vec3(0.0f, 0.0f, -1.0f);
            break;
        case SNAKE_DOWN:
            return glm::vec3(0.0f, 0.0f, 1.0f);
            break;
        case SNAKE_LEFT:
            return glm::vec3(-1.0f, 0.0f, 0.0f);
            break;
        case SNAKE_RIGHT:
            return glm::vec3(1.0f, 0.0f, 0.0f);
            break;
    }

}

bool checkWallCollision()
{
    if ((snakePos[0].x + snakeDirection.x) < (-halfPlatformLength + 1)) return true;
    if ((snakePos[0].x + snakeDirection.x) > halfPlatformLength)        return true;

    if ((snakePos[0].z + snakeDirection.z) < (-halfPlatformLength + 1)) return true;
    if ((snakePos[0].z + snakeDirection.z) > halfPlatformLength)        return true;

    return false;
}

void checkFoodCollision()
{
    // if the head of the snake enters a food cell
    for (int i = 0; i < currentFoodAmount; i++)
    {

        if (snakePos[0].x == foodLocations[i].x && snakePos[0].z == foodLocations[i].z)
        {

            updateFoodSpaceRemove(foodLocations[i], i);
            snakePos.push_back(snakePos.back());
            numSnakeParts++;

        }
    } 
}

void updateFoodSpaceRemove(glm::vec3 location, int index)
{
    int xCell = location.x + halfPlatformLength;
    int zCell = location.z + halfPlatformLength;


    grid[xCell][zCell] = 0;

    //std::cout << "REMOVING FOOD GRID CELL (X, Z) : (" << xCell-platformLength / 2 << ", " << zCell-platformLength / 2 << ")\n";

    foodLocations[index] = foodLocations.back(); 
    foodLocations.pop_back();
    updateFoodSpaceAdd(generateRandFoodSpawn(), 0);

    currentFoodAmount--;
    //printGrid();

}

void updateFoodSpaceAdd(glm::vec3 location, int index)
{
    int xCell = location.x + halfPlatformLength;
    int zCell = location.z + halfPlatformLength;

    grid[xCell][zCell] = 1;

    foodLocations.push_back(location);
    currentFoodAmount++;

}

glm::vec3 generateRandFoodSpawn()
{
    // while the generated spawn location is not occupied by a snake or another food --------------- TO BE DONE 
    int randomX = glm::linearRand(-halfPlatformLength + 1, halfPlatformLength);
    int randomZ = glm::linearRand(-halfPlatformLength + 1, halfPlatformLength);

    glm::vec3 spawnLoc = glm::vec3(randomX, 0.5f, randomZ);
    // std::cout << "SPAWNED FOOD AT (X, Z) = (" << randomX << ", " << randomZ << ")\n";
    return spawnLoc;
}

// help me see the gridddddddddd
void printGrid()
{
    for (int i = 0; i < halfPlatformLength; i++)
    {
        for (int k = 0; k < halfPlatformLength; k++)
        {
            std::cout << grid[i][k] << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void updateSnakePos()
{
    for (int i = numSnakeParts - 1; i > 0; i--)
    {
        snakePos[i] = snakePos[i - 1];
    } 
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// processes all keyboard inputs
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        shiftPressed = true;

    // player movement controls 
    // MOVE SNAKE UP
    if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && snakeDir != SNAKE_DOWN)
    {

        if (movedUp == false)
        {
            movedUp = !movedUp;
            snakeDirection = glm::vec3(0.0f, 0.0f, -1.0f);

            if (snakeDir != SNAKE_UP)
            {
                snakeDir = SNAKE_UP;
                inputBuffer.push_back(snakeDir);               
            }
        }

        if (firstPressed == false) firstPressed = true; 
    }
    else
    {
        movedUp = false;
    }

    // MOVE SNAKE DOWN
    if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && snakeDir != SNAKE_UP)
    {
        if (!movedDown)
        {
            movedDown = !movedDown;
            snakeDirection = glm::vec3(0.0f, 0.0f, 1.0f);

            if (snakeDir != SNAKE_DOWN)
            {
                snakeDir = SNAKE_DOWN;
                inputBuffer.push_back(snakeDir);
            }

            if (firstPressed == false) firstPressed = true;
        }
    }
    else
    {
        movedDown = false;
    }

    // MOVE SNAKE LEFT
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && snakeDir != SNAKE_RIGHT)
    {
        if (movedLeft == false)
        {
            movedLeft = !movedLeft;
            snakeDirection = glm::vec3(-1.0f, 0.0f, 0.0f);

            if (snakeDir != SNAKE_LEFT)
            {
                snakeDir = SNAKE_LEFT;
                inputBuffer.push_back(snakeDir);
            }

            if (firstPressed == false) firstPressed = true;
        }
    }
    else
    {
        movedLeft = false;
    }

    // MOVE SNAKE RIGHT
    if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && snakeDir != SNAKE_LEFT)
    {
        if (!movedRight)
        {
            movedRight = !movedRight;
            snakeDirection = glm::vec3(1.0f, 0.0f, 0.0f);

            if (snakeDir != SNAKE_RIGHT)
            {
                snakeDir = SNAKE_RIGHT;
                inputBuffer.push_back(snakeDir);
            }

            if (firstPressed == false) firstPressed = true;
        }
    }
    else
    {
        movedRight = false;
    }

    if (shiftPressed)
    {
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

}

// process mouse movements
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}