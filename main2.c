#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Fragment shader source code
const char* fragmentShaderSource = "#version 450 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec2 TexCoord;\n"
                                   "uniform sampler2D ourTexture;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = texture(ourTexture, TexCoord);\n"
                                   "}\n\0";

// Function to handle GLFW errors
static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Set GLFW error callback
    glfwSetErrorCallback(error_callback);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Texture Example", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    // Load image data
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *imageData = stbi_load("/home/ole/Bilder/rocket.png", &width, &height, &channels, 0);
    if (!imageData) {
        fprintf(stderr, "Failed to load image\n");
        glfwTerminate();
        return -1;
    }

    // Create texture object
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Free image data
    stbi_image_free(imageData);

    // Compile and link shaders
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

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define vertices and texture coordinates for a quad covering the entire viewport
    float vertices[] = {
            // Positions           // Texture Coords
            0.0f, 0.0f, 0.0f,    0.0f, 0.0f,
            0.5f, 0.0f, 0.0f,    2.0f, 0.0f,
            0.5f,  0.5f, 0.0f,    2.0f, 2.0f,
            0.0f,  0.5f, 0.0f,    0.0f, 2.0f
    };

    // Create a Vertex Buffer Object (VBO) and Vertex Array Object (VAO) for the quad
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specify vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the framebuffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the shader program
        glUseProgram(shaderProgram);

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Bind the VAO
        glBindVertexArray(VAO);

        // Draw the quad
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // Unbind the VAO
        glBindVertexArray(0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
