// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

void runProgram(GLFWwindow *window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Set up your scene here (create Vertex Array Objects, etc.)

    unsigned int vao = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer = 0;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    float vertices[] = {
        -0.40f,
        -0.25f,
        0.6f,
        0.10f,
        -0.25f,
        0.6f,
        -0.15f,
        0.30f,
        0.6f,
        -0.10f,
        -0.25f,
        0.7f,
        0.40f,
        -0.25f,
        0.7f,
        0.15f,
        0.30f,
        0.7f,
        -0.25f,
        -0.58f,
        0.8f,
        0.25f,
        -0.58f,
        0.8f,
        0.00f,
        -0.03f,
        0.8f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glEnableVertexAttribArray(0);

    unsigned int indexBuffer = 1;

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    unsigned int indexBufferData[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferData), &indexBufferData, GL_STATIC_DRAW);

    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert",
                           "../gloom/shaders/simple.frag");

    shader.activate();

    // Rendering Loop

    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}

void handleKeyboardInput(GLFWwindow *window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
