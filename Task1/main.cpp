#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

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
    window = glfwCreateWindow(1024, 768, "HW 1", NULL, NULL);
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

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint leftProgramID = LoadShaders("../VertexShader.vertexshader", "../Fragment1.fragmentshader");
    GLuint rightProgramID = LoadShaders("../VertexShader.vertexshader", "../Fragment2.fragmentshader");

    // массив с координатами треугольников
    static const GLfloat g_vertex_buffer_data[] = {
            0.f, 1.f, 0.0f,
            0.5f, 0.f, 0.0f,
            -0.5f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.0f, 0.1f, 0.0f,
    };

    // загрузка массива в OpenGL (закид в буффер)
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // цикл отрисовки
    do {
        glClear(GL_COLOR_BUFFER_BIT); //заполняем цвета фон

        // обращаение в буффер
        // Указываем, что первым буфером атрибутов будут вершины
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // Атрибут 0. Подробнее об этом будет рассказано в части, посвященной шейдерам.
                3,                  // Размер
                GL_FLOAT,           // Тип
                GL_FALSE,           // Указывает, что значения не нормализованы
                0,                  // Шаг
                (void*)0            // Смещение массива в буфере
        );


        // используем первый шейдерю
        glUseProgram(leftProgramID);

        // Начиная с вершины 0, всего 3 вершины -> один треугольник
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(rightProgramID);

        // Начиная с вершины 3, всего 3 вершины -> второй треугольник
        glDrawArrays(GL_TRIANGLES, 3, 6);

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);


    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(leftProgramID);
    glDeleteProgram(rightProgramID);

    glfwTerminate();
    return 0;
}
