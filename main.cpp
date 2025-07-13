#include <glad/glad.h>
#include </Users/shobhitmehrotra/Documents/glfw-3.4/include/GLFW/glfw3.h>
#include <iostream>
#include "include/shader.h"
#include "include/window.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}
)";
const char *fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(ourColor, 1.0f);
}
)";


int main() {
    GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT, "Porter");
    if (!window) return -1;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader(vertexShaderSource, fragmentShaderSource);
    glEnable(GL_DEPTH_TEST);
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, 
        4, 5, 6, 6, 7, 4, 
        4, 5, 1, 1, 0, 4, 
        6, 7, 3, 3, 2, 6, 
        4, 7, 3, 3, 0, 4, 
        5, 6, 2, 2, 1, 5  
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    auto loadIdentity = [](float* mat) {
        for (int i = 0; i < 16; ++i) mat[i] = 0.0f;
        mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
    };
    auto mat4mul = [](float* out, const float* a, const float* b) {
        float r[16];
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                r[i*4+j] = 0.0f;
                for (int k = 0; k < 4; ++k)
                    r[i*4+j] += a[i*4+k] * b[k*4+j];
            }
        for (int i = 0; i < 16; ++i) out[i] = r[i];
    };

    float model[16], view[16], projection[16];
    loadIdentity(model);
    loadIdentity(view);
    loadIdentity(projection);

    float angleX = 30.0f * 3.14159f / 180.0f;
    float angleY = 45.0f * 3.14159f / 180.0f;
    float rotX[16], rotY[16];
    loadIdentity(rotX);
    loadIdentity(rotY);
    rotX[5] = cos(angleX); rotX[6] = -sin(angleX);
    rotX[9] = sin(angleX); rotX[10] = cos(angleX);
    rotY[0] = cos(angleY); rotY[2] = sin(angleY);
    rotY[8] = -sin(angleY); rotY[10] = cos(angleY);
    mat4mul(model, rotY, rotX);

    float eye[3] = {1.5f, 1.5f, 3.0f}, center[3] = {0,0,0}, up[3] = {0,1,0};
    float f[3] = {center[0]-eye[0], center[1]-eye[1], center[2]-eye[2]};
    float flen = sqrt(f[0]*f[0]+f[1]*f[1]+f[2]*f[2]);
    for(int i=0;i<3;++i) f[i]/=flen;
    float s[3] = {
        f[1]*up[2]-f[2]*up[1],
        f[2]*up[0]-f[0]*up[2],
        f[0]*up[1]-f[1]*up[0]
    };
    float slen = sqrt(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]);
    for(int i=0;i<3;++i) s[i]/=slen;
    float u[3] = {
        s[1]*f[2]-s[2]*f[1],
        s[2]*f[0]-s[0]*f[2],
        s[0]*f[1]-s[1]*f[0]
    };
    loadIdentity(view);
    view[0]=s[0]; view[4]=s[1]; view[8]=s[2];
    view[1]=u[0]; view[5]=u[1]; view[9]=u[2];
    view[2]=-f[0]; view[6]=-f[1]; view[10]=-f[2];
    view[12]=-(s[0]*eye[0]+s[1]*eye[1]+s[2]*eye[2]);
    view[13]=-(u[0]*eye[0]+u[1]*eye[1]+u[2]*eye[2]);
    view[14]=f[0]*eye[0]+f[1]*eye[1]+f[2]*eye[2];

    float fov = 45.0f, aspect = (float)SCR_WIDTH/SCR_HEIGHT, near = 0.1f, far = 100.0f;
    float tanHalfFovy = tanf(fov/2.0f * 3.14159f/180.0f);
    for(int i=0;i<16;i++) projection[i]=0.0f;
    projection[0] = 1.0f/(aspect*tanHalfFovy);
    projection[5] = 1.0f/tanHalfFovy;
    projection[10] = -(far+near)/(far-near);
    projection[11] = -1.0f;
    projection[14] = -(2.0f*far*near)/(far-near);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shader.deleteProgram();
    glfwTerminate();
    return 0;
}


