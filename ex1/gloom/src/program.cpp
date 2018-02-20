// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "textures/utilities.hpp"

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
        -0.5f, -0.5f, 0.6f, 0.0f, 0.0f,
        0.50f, -0.5f, 0.6f, 1.0f, 1.0f,
        -0.5f, 0.50f, 0.6f, 1.0f, 0.0f};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)12);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Index buffer

    unsigned int indexBuffer = 1;

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    unsigned int indexBufferData[] = {0, 1, 2};

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferData), &indexBufferData, GL_STATIC_DRAW);

    // Texture

    unsigned int texture = 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    PNGImage image = loadPNGFile("../gloom/src/textures/diamond.png");

    std::cout << image.pixels[0] << std::endl;

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image.pixels[0]);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTextureUnit(0, texture);

    // Shaders

    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert",
                           "../gloom/shaders/simple.frag");

    // Rendering Loop

    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        shader.activate();

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
