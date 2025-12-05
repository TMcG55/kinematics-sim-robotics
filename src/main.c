#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include "kinematics.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

// Vertex Shader source code
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment Shader source code
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\0";

int main() {

    vec2 alpha={0,0};
    vec2 theta={0,0};
    vec2 linkLength={0.2,0.2};
    vec2 Di={0,0};
    vec3 endPosition;
    mat3 endRotation;
    mat4 T01;
    mat4 T12;
    mat4 T02;

    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Specify OpenGL version 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Robotics", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    // Viewport
    glViewport(0, 0, 800, 800);

    // Define initial points
    vec3 start = {0.0, 0.0, 0.0};
    vec3 end   = {1.0, 0.0, 0.0};

    // Vertex data
    GLfloat vertices[6];
    GLuint indices[] = {1,0,0,2};

    // Create shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create VAO, VBO, EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // Bind and fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    // Vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO/VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        if (theta[0] >=M_PI){
            theta[0] = -M_PI;
        }
        if (theta[1] <=-M_PI){
            theta[1] = M_PI;
        }
        theta[0]+=0.005;
        theta[1]-=0.01;

        dhTransformationMatrix(alpha[0],theta[0], linkLength[0],Di[0],T01);

        vec3 endPos1= {T01[0][3],T01[1][3],T01[2][3]};

        dhTransformationMatrix(alpha[1],theta[1], linkLength[1],Di[1],T12);

        mat4_mul(T01,T12,T02);

        vec3 endPos2= {T02[0][3],T02[1][3],T02[2][3]};

        // Vertex data
        vertices[0]=(GLfloat) endPos1[0];
        vertices[1]=(GLfloat) endPos1[1];
        vertices[2]=(GLfloat) endPos1[2];
        vertices[3]=(GLfloat) endPos2[0];
        vertices[4]=(GLfloat) endPos2[1];
        vertices[5]=(GLfloat) endPos2[2];

        //sub in new data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);


        // Draw lines
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, 0);

        // swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
