
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"

"uniform vec4 ourColorGreen;\n"

"void main()\n"
"{\n"
"FragColor = ourColorGreen;\n"
"}\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"

"uniform vec4 ourColorBlue;\n" 

"void main()\n"
"{\n"
"   FragColor = ourColorBlue;\n"
"}\0";

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";


#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{

    if (!glfwInit())
    {
        std::cout << "GLFW failed to initialize\n";
        return -1;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // BLUE
    float HColor1[] = {
        // Triangle Left H
        -0.8f, -0.4f, 0.0f, // BL
        -0.6f, -0.4f, 0.0f, // BR
        -0.8f, 0.4f, 0.0f, // TL

        // Triangle Right H
        -0.2f, -0.4f, 0.0f, // BL
        -0.0f, -0.4f, 0.0f, // BR
        -0.2f, 0.4f, 0.0f, // TL

        // Triangle Bridge 
        -0.6f, -0.1f, 0.0f, // BL
        -0.2f, 0.1f, 0.0f, // TR
        -0.6f, 0.1f, 0.0f, // TL

        // Triangle I
        0.3f, -0.4f, 0.0f, // BL
        0.5f, -0.4f, 0.0f, // BR
        0.3f, 0.4f, 0.0f, // TL
    };

    // RED
    float HColor2[] = {
        // Triangle Left H 
        -0.8f, 0.4f, 0.0f, // TL 
        -0.6f, -0.4f, 0.0f, // BR
        -0.6f, 0.4f, 0.0f, // TR

        // Triangle Right H 
        -0.2f, 0.4f, 0.0f, // TL 
        -0.0f, -0.4f, 0.0f, // BR
        -0.0f, 0.4f, 0.0f, // TR

        // Triangle Bridge 
        -0.6f, -0.1f, 0.0f, // BL
        -0.2f, -0.1f, 0.0f, // BR
        -0.2f, 0.1f, 0.0f, // TR

        // Triangle I
        0.3f, 0.4f, 0.0f, // TL 
        0.5f, -0.4f, 0.0f, // BR
        0.5f, 0.4f, 0.0f, // TR

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

    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
   
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // this creates a vertex shader object and returns its ID, which is stored in the variable vertexShader

	// this attaches the source code of the vertex shader to the shader object and compiles it
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

	// this checks if the shader compilation was successful and prints any error messages if it failed
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShaders[2];

    fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaders[0], 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaders[0]);

    fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaders[1], 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShaders[1]);

    glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShaders[1], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Setup first shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShaders[0]);
    glLinkProgram(shaderProgram);

    glDeleteShader(fragmentShaders[0]);

    // Setup second shader program
    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();

    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShaders[1]);
    glLinkProgram(shaderProgram2);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaders[1]); 

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "PROGRAM LINK ERROR:\n" << infoLog << std::endl;
    }
   
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // First triangle setup
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(HColor1), HColor1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Second triangle setup
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(HColor2), HColor2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glViewport(0, 0, 800, 600);


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register the callback function to adjust the viewport when the window is resized


	while (!glfwWindowShouldClose(window)) // render loop
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        float greenValue = sin(glfwGetTime()) / 2.0f + 0.5f;
        // Querying from shaderProgram for ourColorGreen
        int greenLocation = glGetUniformLocation(shaderProgram, "ourColorGreen");
        glUniform4f(greenLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 12);



        glUseProgram(shaderProgram2);
        float blueValue = sin(glfwGetTime()) / 2.0f + 0.5f;
        // Querying from shaderProgram2 for ourColorBlue
        int blueLocation = glGetUniformLocation(shaderProgram2, "ourColorBlue");
        glUniform4f(blueLocation, 0.0f, 0.0f, blueValue, 1.0f);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 12);


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

