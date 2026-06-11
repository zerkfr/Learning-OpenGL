#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
    
public:

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float pitch, yaw, fov;
    bool firstMouse = true;

    float movementSpeed;
    float mouseSensitivity;

    /* camera constructor takes:
        1. camera position
        2. camera front
        3. camera up
        4. camera pitch
        5. camera yaw
        6. camera fov
        7. camera speed
        8. camera sensitivity
    */
    Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 25.0f),
           glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f), 
           glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f), 
           float cameraPitch = 0.0f, float cameraYaw = -90.0f, float cameraFOV = 45.0f, 
           float cameraSpeed = 9.0f, float cameraSensitivity = 0.1f)
    {
        position = cameraPos;
        front = cameraFront;
        up = cameraUp;

        pitch = cameraPitch;
        yaw = cameraYaw;
        fov = cameraFOV;

        movementSpeed = cameraSpeed;
        mouseSensitivity = cameraSensitivity;
    }

    // returns the lookAt matrix of the camera object
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    // returns the projection matrix 
    glm::mat4 getProjMatrix(float WIDTH, float HEIGHT, float near, float far)
    {
        return glm::perspective(glm::radians(fov), WIDTH / HEIGHT, near, far);
    }

    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset; 

        calculateCameraAngle();
    }

    void processKeyboardInput(Movement direction, float deltaTime)
    {
        if (direction == FORWARD) // forwards
            position += movementSpeed * front * deltaTime;

        if (direction == BACKWARD) // backwards
            position -= movementSpeed * front * deltaTime;

        if (direction == LEFT) // left
            position -= glm::normalize(glm::cross(front, up)) * movementSpeed * deltaTime;

        if (direction == RIGHT) // right
            position += glm::normalize(glm::cross(front, up)) * movementSpeed * deltaTime;

        if (direction == UP) // up
            position.y += movementSpeed * deltaTime;

        if (direction == DOWN) // down
            position.y -= movementSpeed * deltaTime;
    }

    void processScrollInput(float yoffset)
    {
        fov -= (float)yoffset;

        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

    void calculateCameraAngle()
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
    }

    glm::vec3 getPosition()
    {
        return position;
    }

    glm::vec3 getFront()
    {
        return front;
    }
};


#endif 
