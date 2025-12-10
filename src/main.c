#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include "kinematics.h"
#include "camera.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

// Vertex shader source code
const char* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"void main() {\n"
"   gl_Position = projection * view * vec4(aPos, 1.0);\n"
"}\0";

// Fragment shader with color uniform
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 uColor;\n"
"void main() {\n"
"   FragColor = uColor;\n"
"}\0";

int main() {
    // ---------------- GLFW init ----------------
    if (!glfwInit()) { printf("GLFW init failed\n"); return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,800,"Robotics",NULL,NULL);
    if(!window){ printf("Window creation failed\n"); glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ printf("GLAD failed\n"); return -1; }
    glViewport(0,0,800,800);
    glEnable(GL_DEPTH_TEST);
    // Set background to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // ---------------- Robot params ----------------
    vec6 alpha={M_PI/2,0,M_PI/2,-M_PI/2,M_PI/2,0};
    vec6 theta={0,0,0,0,0,0};
    vec6 Ai={0,0.2,0,0,0};
    vec6 Di={0.3,0,0,0.2,0,0.1};
    FKresults transforms = forwardKinematics6new(Ai, alpha, Di, theta);

    GLfloat vertices[7*3]; // robot joints
    // First vertex is base at origin
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;
    GLuint indices[] = {0,1,1,2,2,3,3,4,4,5,5,6};

    // ---------------- Camera ----------------
    camera_init(30.0f, 1.0f, 0.1f, 10.0f);
    camera_look_at(1.0f,1.0f,2.0f, 0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f);
    // Parameters for orbit
    float orbitRadius = 2.5f;       // distance from origin
    float orbitSpeed  = 0.000f;      // radians per frame
    float orbitAngle  = M_PI/2;       // current angle
    float fixedZ      = 1.0f;       // height of camera above XY-plane


    float projection[16], view[16];
    camera_get_projection(projection);
    camera_get_view(view);


    // ---------------- Shader ----------------
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint projLoc = glGetUniformLocation(shaderProgram,"projection");
    GLint viewLoc = glGetUniformLocation(shaderProgram,"view");
    GLint colorLoc = glGetUniformLocation(shaderProgram,"uColor");

    // ---------------- VAO/VBO for robot ----------------
    GLuint VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // ---------------- VAO/VBO for axes ----------------
    GLfloat axesVertices[7*3*2*3]; // 6 joints * 3 axes * 2 points * 3 coords
    GLuint axesIndices[7*3*2];    // sequential indices
    float axisLength=0.1f;
    for(int i=0;i<7*3;i++){ axesIndices[i*2]=i*2; axesIndices[i*2+1]=i*2+1; }

    GLuint axesVAO,axesVBO,axesEBO;
    glGenVertexArrays(1,&axesVAO);
    glGenBuffers(1,&axesVBO);
    glGenBuffers(1,&axesEBO);

    glBindVertexArray(axesVAO);
    glBindBuffer(GL_ARRAY_BUFFER,axesVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(axesVertices),axesVertices,GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,axesEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(axesIndices),axesIndices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    double tStart = glfwGetTime();
    int animationSequencer =0;
    // ---------------- Render loop ----------------
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double t = glfwGetTime();
        // Animate joints
        theta[1]=cos(tStart); theta[2]=cos(tStart);
        theta[3]=cos(tStart); theta[4]+=0.0000f; theta[5]-=0.000f;

     
        theta[animationSequencer]=cos(t-tStart);
        if ((t-tStart)>=2*M_PI){
            tStart=t;
            animationSequencer+=1;
            if (animationSequencer==5){
                animationSequencer=0;
            }
        }


        // for(int i=0;i<6;i=i+2){
        //     printf("theta[%d]: %f ",i,theta[i]);
        //     if(theta[i]>=M_PI)
        //         theta[i]-=2*M_PI;
        // }
        // for(int i=1;i<6;i=i+2){
        //     printf("theta[%d]: %f ",i,theta[i]);
        //     if(theta[i]<=-M_PI)
        //         theta[i]+=2*M_PI;
        // }
        // printf("\n");
        transforms = forwardKinematics6new(Ai, alpha, Di, theta);

        // for (int k = 0; k < 6; k++) {
        // printf("Matrix %d:\n", k + 1);
        // for (int i = 0; i < 4; i++) {
        //     for (int j = 0; j < 4; j++) {
        //         printf("%8.3f ", transforms.T[k].m[i][j]);
        //     }
        //     printf("\n");
        // }
        // 
        //}
        // Update robot vertices
        for(int i=0;i<6;i++){
            vertices[(i+1)*3+0]=(GLfloat)transforms.T[i].m[0][3];
            vertices[(i+1)*3+1]=(GLfloat)transforms.T[i].m[1][3];
            vertices[(i+1)*3+2]=(GLfloat)transforms.T[i].m[2][3];
        }

        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);


        // --- Update orbiting camera ---
        orbitAngle += orbitSpeed;
        if (orbitAngle > 2*M_PI) orbitAngle -= 2*M_PI;

        // Compute camera position in XY-plane around Z-axis
        float cameraX = orbitRadius * cos(orbitAngle);
        float cameraY = orbitRadius * sin(orbitAngle);
        float cameraZ = fixedZ;

        // Set camera to look at origin, with Z-up
        camera_look_at(cameraX, cameraY, cameraZ,   // eye
            0.0f, 0.0f, 0.0f,            // target
            0.0f, 0.0f, 1.0f);           // up along Z

        camera_get_view(view);  // get updated view matrix

        float axisLength = 0.1f; // length of each axis
        for (int j = 0; j < 6; j++) {
            // origin of joint in world coordinates
            float x = (float)transforms.T[j].m[0][3];
            float y = (float)transforms.T[j].m[1][3];
            float z = (float)transforms.T[j].m[2][3];

            // extract rotation matrix of joint
            float r00 = (float)transforms.T[j].m[0][0];
            float r01 = (float)transforms.T[j].m[0][1];
            float r02 = (float)transforms.T[j].m[0][2];

            float r10 = (float)transforms.T[j].m[1][0];
            float r11 = (float)transforms.T[j].m[1][1];
            float r12 = (float)transforms.T[j].m[1][2];

            float r20 = (float)transforms.T[j].m[2][0];
            float r21 = (float)transforms.T[j].m[2][1];
            float r22 = (float)transforms.T[j].m[2][2];

            // X axis
            axesVertices[j*18+0] = x; 
            axesVertices[j*18+1] = y; 
            axesVertices[j*18+2] = z;
            axesVertices[j*18+3] = x + r00*axisLength;
            axesVertices[j*18+4] = y + r10*axisLength;
            axesVertices[j*18+5] = z + r20*axisLength;

            // Y axis
            axesVertices[j*18+6] = x; 
            axesVertices[j*18+7] = y; 
            axesVertices[j*18+8] = z;
            axesVertices[j*18+9] = x + r01*axisLength;
            axesVertices[j*18+10] = y + r11*axisLength;
            axesVertices[j*18+11] = z + r21*axisLength;

            // Z axis
            axesVertices[j*18+12] = x; 
            axesVertices[j*18+13] = y; 
            axesVertices[j*18+14] = z;
            axesVertices[j*18+15] = x + r02*axisLength;
            axesVertices[j*18+16] = y + r12*axisLength;
            axesVertices[j*18+17] = z + r22*axisLength;
        }

        glBindBuffer(GL_ARRAY_BUFFER,axesVBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(axesVertices),axesVertices);

        // Use shader and set projection/view
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(projLoc,1,GL_FALSE,projection);
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,view);

        // Draw robot links
        glUniform4f(colorLoc,0.0f,0.0f,0.0f,1.0f);
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES,10,GL_UNSIGNED_INT,0);

        // Draw axes
        glBindVertexArray(axesVAO);
        for(int j=0;j<6;j++){
            // X axis
            glUniform4f(colorLoc,1.0f,0.0f,0.0f,1.0f);
            glDrawElements(GL_LINES,2,GL_UNSIGNED_INT,(void*)(sizeof(GLuint)*j*6));
            // Y axis
            glUniform4f(colorLoc,0.0f,1.0f,0.0f,1.0f);
            glDrawElements(GL_LINES,2,GL_UNSIGNED_INT,(void*)(sizeof(GLuint)*(j*6+2)));
            // Z axis
            glUniform4f(colorLoc,0.0f,0.0f,1.0f,1.0f);
            glDrawElements(GL_LINES,2,GL_UNSIGNED_INT,(void*)(sizeof(GLuint)*(j*6+4)));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ---------------- Cleanup ----------------
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&axesVAO);
    glDeleteBuffers(1,&axesVBO);
    glDeleteBuffers(1,&axesEBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
