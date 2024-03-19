#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define SPEED 0.125

void readKeyboard(GLFWwindow *window, float *x_direction, float *y_direction);

int main()
{
    printf("Start\n");

    if (!glfwInit()) {
        printf("no glfw :(\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow  *window = glfwCreateWindow(1000, 1000, "space", NULL, NULL);
    if (!window) {
        printf("no window :(\n");
        return 1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("no glad :(\n");
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, 1000, 1000);

    const char* vShaderCode =
            "#version 460 core\n"
            "layout (location = 0) in vec2 vertex_position;\n"
            "uniform float x_pos;\n"
            "uniform float y_pos;\n"
            "void main() {\n"
            "    gl_Position = vec4(vertex_position.x + x_pos, vertex_position.y + y_pos, 0.0, 1.0);\n"
            "}\0";

    const char* fragmentShaderSource =
            "#version 460 core\n"
            "out vec4 frag_color;\n"
            "void main(){\n"
            "    frag_color = vec4(0.1412, 0.1608, 0.1843, 1.0);\n"
            "}\0";


    FILE *vertexShaderFile;
    char* vertexShaderSource;

    /* Read File to get size */
    vertexShaderFile = fopen("/home/ole/CLionProjects/test/vertex.glsl", "rb");
    long size = 0;
    if (vertexShaderFile == NULL) {
        printf("error\n");
    }
    fseek(vertexShaderFile, 0L, SEEK_END);
    size = ftell(vertexShaderFile)+1;
    fclose(vertexShaderFile);

    /* Read File for Content */
    vertexShaderFile = fopen("/home/ole/CLionProjects/test/vertex.glsl", "r");
    vertexShaderSource = memset(malloc(size), '\0', size);
    fread(vertexShaderSource, 1, size-1, vertexShaderFile);
    fclose(vertexShaderFile);

    unsigned int vertex_shader;
    unsigned int fragment_shader;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_shader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, fragment_shader);
    glLinkProgram(shaderProgram);

    float vertecies[] = {
            -0.1f, -0.1f,
            -0.1f, 0.1f,
            0.1f, 0.1f,
            0.1f, -0.1f
    };
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);


    unsigned int amount = 3;
    float positions[] =  {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.0f, 0.5f
    };


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(0);

    float x_pos = 0.0f;
    float y_pos = 0.0f;




    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // read keyboard
        //readKeyboard(window, &x_pos, &y_pos);
        x_pos += 0.005f;
        // draw
        glUseProgram(shaderProgram);
        glUniform1f(glGetUniformLocation(shaderProgram, "x_pos"), x_pos);
        glUniform1f(glGetUniformLocation(shaderProgram, "y_pos"), y_pos);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void readKeyboard(GLFWwindow *window, float *x_direction, float *y_direction)
{
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        *y_direction += SPEED;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        *y_direction -= SPEED;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        *x_direction -= SPEED;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        *x_direction += SPEED;
    }
}