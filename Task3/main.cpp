#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "common/shader.hpp"

using std::cout;
using std::endl;
using namespace glm;


GLFWwindow* window;

int main() {

    if (!glfwInit())
    {
        cout << "An error occured while GLFW initializing!" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x smoothing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // in order to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // the same reason
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // new OpenGL

    // open the window and create the OpenGL context here
    window = glfwCreateWindow(1024, 768, "HW3", NULL, NULL);

    if (window == NULL) {
        cout << "Can't open the window!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        cout << "An error occured while GLFW initializing!" << endl;
        glfwTerminate();
        return -1;
    }

    // button press reaction
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // background
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // create Vertex Array Object
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // create program from shader
    GLuint programID = LoadShaders("../Vertex.vertexshader", "../Triangle.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP"); // M V P - композиция

    // Projection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f * 2.f), 4.0f * 2.f / 3.0f, 0.1f, 100.0f);

    // Model matrix : an identity matrix
    glm::mat4 Model = glm::mat4(1.0f);

    // аргументы для камера
    vec3 cameraPos = glm::vec3(4, 3, 0);
    vec3 originPos = glm::vec3(0, 0, 0);
    vec3 headPos = glm::vec3(0, 1, 0);

    glm::mat4 View = glm::lookAt(cameraPos, originPos, headPos);

    // ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model;

    // a regular icosahedron consists of 20 triangles, so there will be 3*20=60 vertices
    static const GLfloat g_vertex_buffer_data[] = {
            2.f, 0.f, 0.f, // c b e
            0.f, 2.f, 0.f,
            0.f, 0.0f, 2.f,

            2.f, 0.f, 0.f, // b e d
            0.f, -2.f, 0.f,
            0.f, 0.0f, 2.f,

            -2.f, 0.f, 0.f, // a b d
            0.f, -2.f, 0.f,
            0.f, 0.0f, 2.f,

            -2.f, 0.f, 0.f, // a b c
            0.f, 2.f, 0.f,
            0.f, 0.0f, 2.f,

            0.f, 0.f, 0.f, //
            -2.f, 0.f, 0.f,
            0.f, 2.f, 0.f,

            0.f, 0.f, 0.f, //
            2.f, 0.f, 0.f,
            0.f, 2.f, 0.f,

            0.f, 0.f, 0.f, //
            2.f, 0.f, 0.f,
            0.f, -2.f, 0.f,

            0.f, 0.f, 0.f, //
            -2.f, 0.f, 0.f,
            0.f, -2.f, 0.f,

    };

    // One color for each vertex.
    static const GLfloat g_color_buffer_data[] = {
            0.0f, 0.f, 0.0f,		//1
            0.0f, 1.0f, 0.f,
            1.0f, 1.f, 0.0f,

            1.0f, 0.54f, 0.0f,	    //2
            0.4f, 0.4f, 0.4f,
            0.0f, 0.0f, 0.f,

            0.4f, 0.4f, 0.4f,
            0.0f, 1.0f, 0.267f,
            0.0f, 0.f, 0.0f,

            0.4f, 0.4f, 0.4f,
            0.0f, 0.0f, 0.0f,
            1.0f, 1.f, 0.0f,

            1.0f, 0.0f, 0.078f,
            0.467f, 0.02f, 1.0f,
            0.467f, 0.02f, 1.0f,

            1.0f, 0.0f, 0.078f,
            0.467f, 0.02f, 1.0f,
            0.467f, 0.02f, 1.0f,

            1.0f, 0.0f, 0.078f,
            0.467f, 0.02f, 1.0f,
            0.467f, 0.02f, 1.0f,

            1.0f, 0.0f, 0.078f,
            0.467f, 0.02f, 1.0f,
            0.467f, 0.02f, 1.0f,

    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // пермер для вращения
    double pos = 0.0;

    do {
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use program
        glUseProgram(programID);

        glm::mat4 View = glm::lookAt(cameraPos, originPos, headPos);

        // ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP = Projection * View * Model;

        pos += M_PI / 180 ; // угл для вращения камены

        cameraPos = glm::vec3(cos(pos), 2 * cos(pos), 3 * sin(pos));

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // vertices will be the 1st buffer attribute
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

        // 3=size, GL_FLOAT=type, GL_FALSE=values are not normalized, 0=step
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // colors will be the 2nd buffer attribute
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // отрисовываем треугольники
        glDrawArrays(GL_TRIANGLES, 0, 3 * 8);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();

    } // pressing Escape or window closing
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // cleanup
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);

    // Close OpenGL window
    glfwTerminate();

    cout << "The end." << endl;

    return 0;
}
