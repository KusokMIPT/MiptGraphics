#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "common/shader.hpp"

using std::cout;
using namespace glm;


GLFWwindow* window;

int main() {

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "HW 2", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }


//////////////////////////////////////

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create and compile our GLSL program from the shaders
    GLuint leftProgramID = LoadShaders("../Transform.vertexshader", "../Fragment1.fragmentshader");
    GLuint rightProgramID = LoadShaders("../Transform.vertexshader", "../Fragment2.fragmentshader");

    static const GLfloat g_vertex_buffer_data[] = {
            0.f, 1.f, 0.0f,
            0.5f, 0.f, 0.0f,
            -0.5f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.0f, 0.1f, 0.0f,
    };

    static const GLfloat g_color_buffer_data[] = {
            0.0f, 0.f, 0.0f,        //1
            0.0f, 1.0f, 0.f,
            1.0f, 1.f, 0.0f,

            1.0f, 0.54f, 0.0f,        //2
            0.4f, 0.4f, 0.4f,
            0.0f, 0.0f, 0.f,
    };

    // Get a handle for our "MVP" uniform
    GLuint leftMatrixID = glGetUniformLocation(leftProgramID, "MVP");
    GLuint rightMatrixID = glGetUniformLocation(rightProgramID, "MVP");

    // переспектива
    glm::mat4 Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);


    // Model matrix : переносит точку отсчета координат из фигуры во вне
    glm::mat4 Model = glm::mat4(5.f);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    double degree = 0;

    do {

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0, // The attribute we want to configure
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*) 0            // array buffer offset
        );

        // CAMERA
        double x = cos(degree) * 2;
        double y = sin(degree) * 2;

        // Camera matrix: оперирует позиции матрицы
        glm::mat4 View = glm::lookAt(
                glm::vec3(x, 0, y),
                glm::vec3(0, 0, 0), //
                glm::vec3(0, 1, 0)  // Head
        );

        // Our ModelViewProjection : создаём нашу MVP матрицу
        glm::mat4 MVP = Projection * View * Model; // композиция

        // комплилим шрейдер делаем преобрзаование марицы
        glUseProgram(leftProgramID);
        glUniformMatrix4fv(leftMatrixID, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(rightProgramID);
        glUniformMatrix4fv(rightMatrixID, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 3, 6);

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        degree += 0.01;

    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(leftProgramID);
    glDeleteProgram(rightProgramID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();


    return 0;
}
