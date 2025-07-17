// COMP 371 - Lab 03 - Final Full Version

#include <iostream>
#include <list>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace std;
using namespace glm;

class Projectile {
public:
    Projectile(vec3 position, vec3 velocity, int shaderProgram)
        : mPosition(position), mVelocity(velocity) {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    }

    void Update(float dt) {
        mPosition += mVelocity * dt;
    }

    void Draw() {
        mat4 worldMatrix = translate(mat4(1.0f), mPosition) *
                           rotate(mat4(1.0f), radians(180.0f), vec3(0, 1, 0)) *
                           scale(mat4(1.0f), vec3(0.2f));
        glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    vec3 GetPosition() const { return mPosition; }

private:
    vec3 mPosition;
    vec3 mVelocity;
    GLuint mWorldMatrixLocation;
};

// ---- Shader Sources ----
const char* getVertexShaderSource() {
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix;"
        "uniform mat4 projectionMatrix;"
        "out vec3 vertexColor;"
        "void main() {"
        "   vertexColor = aColor;"
        "   gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(aPos, 1.0);"
        "}";
}

const char* getFragmentShaderSource() {
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main() {"
        "   FragColor = vec4(vertexColor, 1.0);"
        "}";
}

int compileAndLinkShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSrc = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSrc = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int createVertexBufferObject() {
    vec3 vertexArray[] = {
    // Left (-X) - Red
    vec3(-0.5f, -0.5f, -0.5f), vec3(1, 0, 0),
    vec3(-0.5f, -0.5f,  0.5f), vec3(1, 0, 0),
    vec3(-0.5f,  0.5f,  0.5f), vec3(1, 0, 0),
    vec3(-0.5f, -0.5f, -0.5f), vec3(1, 0, 0),
    vec3(-0.5f,  0.5f,  0.5f), vec3(1, 0, 0),
    vec3(-0.5f,  0.5f, -0.5f), vec3(1, 0, 0),

    // Far (-Z) - Blue
    vec3( 0.5f,  0.5f, -0.5f), vec3(0, 0, 1),
    vec3(-0.5f, -0.5f, -0.5f), vec3(0, 0, 1),
    vec3(-0.5f,  0.5f, -0.5f), vec3(0, 0, 1),
    vec3( 0.5f,  0.5f, -0.5f), vec3(0, 0, 1),
    vec3( 0.5f, -0.5f, -0.5f), vec3(0, 0, 1),
    vec3(-0.5f, -0.5f, -0.5f), vec3(0, 0, 1),

    // Bottom (-Y) - Turquoise
    vec3( 0.5f, -0.5f,  0.5f), vec3(0, 1, 1),
    vec3(-0.5f, -0.5f, -0.5f), vec3(0, 1, 1),
    vec3( 0.5f, -0.5f, -0.5f), vec3(0, 1, 1),
    vec3( 0.5f, -0.5f,  0.5f), vec3(0, 1, 1),
    vec3(-0.5f, -0.5f,  0.5f), vec3(0, 1, 1),
    vec3(-0.5f, -0.5f, -0.5f), vec3(0, 1, 1),

    // Near (+Z) - Green
    vec3(-0.5f,  0.5f,  0.5f), vec3(0, 1, 0),
    vec3(-0.5f, -0.5f,  0.5f), vec3(0, 1, 0),
    vec3( 0.5f, -0.5f,  0.5f), vec3(0, 1, 0),
    vec3( 0.5f,  0.5f,  0.5f), vec3(0, 1, 0),
    vec3(-0.5f,  0.5f,  0.5f), vec3(0, 1, 0),
    vec3( 0.5f, -0.5f,  0.5f), vec3(0, 1, 0),

    // Right (+X) - Purple
    vec3( 0.5f,  0.5f,  0.5f), vec3(1, 0, 1),
    vec3( 0.5f, -0.5f, -0.5f), vec3(1, 0, 1),
    vec3( 0.5f,  0.5f, -0.5f), vec3(1, 0, 1),
    vec3( 0.5f, -0.5f, -0.5f), vec3(1, 0, 1),
    vec3( 0.5f,  0.5f,  0.5f), vec3(1, 0, 1),
    vec3( 0.5f, -0.5f,  0.5f), vec3(1, 0, 1),

    // Top (+Y) - Yellow
    vec3( 0.5f,  0.5f,  0.5f), vec3(1, 1, 0),
    vec3( 0.5f,  0.5f, -0.5f), vec3(1, 1, 0),
    vec3(-0.5f,  0.5f, -0.5f), vec3(1, 1, 0),
    vec3( 0.5f,  0.5f,  0.5f), vec3(1, 1, 0),
    vec3(-0.5f,  0.5f, -0.5f), vec3(1, 1, 0),
    vec3(-0.5f,  0.5f,  0.5f), vec3(1, 1, 0)
};


    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    return vao;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "COMP371 - Lab 03", NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) return -1;

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); // Optional: makes sure everything is visible
    glClearColor(0.1f, 0.1f, 0.3f, 1.0f); // dark blue background

    GLuint shader = compileAndLinkShaders();
    glUseProgram(shader);

    GLuint worldLoc = glGetUniformLocation(shader, "worldMatrix");
    GLuint viewLoc = glGetUniformLocation(shader, "viewMatrix");
    GLuint projLoc = glGetUniformLocation(shader, "projectionMatrix");

    mat4 proj = perspective(radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);

    vec3 camPos(0.0f, 2.0f, 10.0f);
    vec3 camUp(0.0f, 1.0f, 0.0f);
    float camYaw = 90.0f, camPitch = 0.0f;
    bool firstPerson = true;

    float lastTime = glfwGetTime(), cubeAngle = 0.0f;
    double lastX, lastY; glfwGetCursorPos(window, &lastX, &lastY);
    int lastMouseLeft = GLFW_RELEASE;

    int vao = createVertexBufferObject();
    list<Projectile> projectiles;

    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float dt = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        // Mouse input
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float dx = xpos - lastX;
        float dy = ypos - lastY;
        lastX = xpos;
        lastY = ypos;

        camYaw -= dx * 0.1f;
        camPitch += dy * 0.1f;
        camPitch = glm::clamp(camPitch, -85.0f, 85.0f);

        vec3 camDir = normalize(vec3(
            cos(radians(camPitch)) * sin(radians(camYaw)),
            sin(radians(camPitch)),
            -cos(radians(camPitch)) * cos(radians(camYaw))
        ));

        // Movement
        float speed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 2.0f : 1.0f;
        vec3 right = normalize(cross(camDir, camUp));
        vec3 forward = normalize(vec3(camDir.x, 0, camDir.z));
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camPos += forward * dt * speed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camPos -= forward * dt * speed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camPos -= right * dt * speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camPos += right * dt * speed;
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) firstPerson = true;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) firstPerson = false;

        mat4 view;
        if (firstPerson) view = lookAt(camPos, camPos + camDir, camUp);
        else view = lookAt(camPos + vec3(0, 3, 10) - 10.0f * camDir, camPos, camUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        // Draw ground
        mat4 ground = translate(mat4(1.0f), vec3(0, -0.01f, 0)) *
                      scale(mat4(1.0f), vec3(100.0f, 0.02f, 100.0f));
        glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &ground[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Spawn pillars
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j) {
                vec3 pos(-20 + i * 5.0f, 5.0f, -20 + j * 5.0f);
                mat4 pillar = translate(mat4(1.0f), pos) *
                              scale(mat4(1.0f), vec3(1, 10, 1));
                glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &pillar[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

        // Shoot projectiles
        int mouseLeft = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (mouseLeft == GLFW_PRESS && lastMouseLeft == GLFW_RELEASE)
            projectiles.push_back(Projectile(camPos, 5.0f * camDir, shader));
        lastMouseLeft = mouseLeft;

        for (auto it = projectiles.begin(); it != projectiles.end(); ) {
            it->Update(dt);
            it->Draw();
            if (length(it->GetPosition()) > 200.0f) it = projectiles.erase(it);
            else ++it;
        }

        // Spinning cube (in front of camera in first person)
        cubeAngle += 180.0f * dt;
        if (firstPerson) {
            mat4 viewModel = view *
                translate(mat4(1.0f), vec3(0.3f, -0.2f, -0.5f)) *
                rotate(mat4(1.0f), radians(cubeAngle), vec3(0, 1, 0)) *
                scale(mat4(1.0f), vec3(0.1f));
            glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &mat4(1.0f)[0][0]);
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewModel[0][0]);
        } else {
            mat4 cube = translate(mat4(1.0f), camPos) *
                        rotate(mat4(1.0f), radians(cubeAngle), vec3(0, 1, 0)) *
                        scale(mat4(1.0f), vec3(0.1f));
            glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &cube[0][0]);
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
