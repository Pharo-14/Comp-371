//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle
// g++ Tut02.cpp -o Tut02 \
  -std=c++11 \
  -I/opt/homebrew/opt/glew/include \
  -I/opt/homebrew/opt/glfw/include \
  -I/opt/homebrew/opt/glm/include \
  -L/opt/homebrew/opt/glew/lib \
  -L/opt/homebrew/opt/glfw/lib \
  -framework OpenGL -lglfw -lGLEW


#include <iostream>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const char* getVertexShaderSource()
{
// Added viewMatrix and projectionMatrix uniforms
// Used full transformation pipeline: projection * view * world
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 vertexColor;\n"
        "uniform mat4 worldMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 projectionMatrix;\n"
        "void main() {\n"
        "    vertexColor = aColor;\n"
        "    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(aPos, 1.0);\n"
        "}";
}


const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    FragColor = vec4(vertexColor, 1.0f);\n"
        "}";
}

glm::vec3 squareArray[] = {
    // First Triangle
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),

    // Second Triangle
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
};

int compileAndLinkShaders()
{
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vsSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vsSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader Compile Error:\n" << infoLog << std::endl;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fsSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fsSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment Shader Compile Error:\n" << infoLog << std::endl;
    }

    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Program Linking Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int createVertexArrayObject(const glm::vec3* vertexArray, int arraySize)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vao;
}

int main()
{
// Variables for poistions
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "COMP 371 - Lab 02", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    int shaderProgram = compileAndLinkShaders();
    int squareVAO = createVertexArrayObject(squareArray, sizeof(squareArray));

    // Variables for rotation
    float angle = 0.0f;
    float rotationSpeed = 90.0f; // degrees/sec
    float lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        angle += rotationSpeed * deltaTime;
        if (angle > 360.0f) angle -= 360.0f;

        // World transform (rotation)
        glm::mat4 worldMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

// pass metrices to shader
        GLuint worldLoc = glGetUniformLocation(shaderProgram, "worldMatrix");
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
GLuint projLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
        glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
// create view matrices
glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);



glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projectionMatrix[0][0]);

        glBindVertexArray(squareVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();

//shift to increase speed

float cameraSpeed = 2.5f * deltaTime;
      if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
          cameraSpeed *= 2.0f;

// move with WASD
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    glfwTerminate();
    return 0;
}
