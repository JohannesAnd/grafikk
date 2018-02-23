// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "textures/utilities.hpp"
#include "gloom/shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

int generateVertexArray(float *vertices, float *textureCoordinates, unsigned int *indices, unsigned int vertexCount, unsigned int triangleCount)
{
    unsigned int vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    unsigned int vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
    glEnableVertexAttribArray(0);

    unsigned int textureBufferID;
    glGenBuffers(1, &textureBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, textureBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 2, textureCoordinates, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);
    glEnableVertexAttribArray(1);

    unsigned int indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangleCount * 3, indices, GL_STATIC_DRAW);

    return vertexArrayID;
}

int loadGLTexture(PNGImage image)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image.pixels[0]);

    glGenerateMipmap(GL_TEXTURE_2D);

    printGLError();

    return textureID;
}

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
glm::mat4 perspectiveMatrix = glm::perspective(glm::radians(90.0f), (float)windowWidth / windowHeight, 0.1f, 1000.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 mvp = glm::mat4(1.0f);

void runProgram(GLFWwindow *window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    PNGImage image = loadPNGFile("../gloom/src/textures/minecraftBig.png");
    std::cout << "Loaded image with size " << image.width << ", " << image.height << std::endl;
    int textureID = loadGLTexture(image);

    float vertices[] = {
        -20.0f, -20.0f, -10.0f,
        20.0f, -20.0f, -10.0f,
        20.0f, 20.0f, -10.0f,
        -20.0f, 20.0f, -10.0f};

    float textureCoordinates[] = {
        0, 0,
        1, 0,
        1, 1,
        0, 1};
    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    unsigned int vertexCount = 4;
    unsigned int triangleCount = 2;

    unsigned int vaoID = generateVertexArray(vertices, textureCoordinates, indices, vertexCount, triangleCount);

    glBindTextureUnit(0, textureID);

    Gloom::Shader shader;
    shader.attach("../gloom/shaders/simple.vert");
    shader.attach("../gloom/shaders/simple.frag");
    shader.link();
    shader.activate();

    GLuint matrixUniform = glGetUniformLocation(shader.get(), "matrix");

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mvp = perspectiveMatrix * glm::lookAt(cameraPos, cameraPos + orientation, up);
        glm::mat4 matrix = mvp;
        glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &matrix[0][0]);

        // Draw your scene here
        glBindVertexArray(vaoID);
        glDrawElements(GL_TRIANGLES, 3 * triangleCount, GL_UNSIGNED_INT, 0);

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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos.y -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos.y += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos.x += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos.x -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        cameraPos.z += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        cameraPos.z -= 0.1f;
    }
}