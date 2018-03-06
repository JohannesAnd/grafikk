// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "textures/utilities.hpp"
#include "textures/OBJLoader.h"
#include "gloom/shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

int generateVertexArray(float3 *vertices, float2 *textureCoordinates, float3 *normalVectors, unsigned int *indices, unsigned int vertexCount, unsigned int indexCount)
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

    unsigned int normalBufferID;
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, normalVectors, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
    glEnableVertexAttribArray(2);

    unsigned int indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indices, GL_STATIC_DRAW);

    return vertexArrayID;
}

int loadGLTexture(PNGImage image)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image.pixels[0]);

    glGenerateMipmap(GL_TEXTURE_2D);

    printGLError();

    return textureID;
}

glm::vec3 cameraPos = glm::vec3(-2.0f, 1.0f, -8.0f);
glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 1.0f);
glm::mat4 perspectiveMatrix = glm::perspective(glm::radians(90.0f), (float)windowWidth / windowHeight, 0.1f, 1000.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 camera = glm::mat4(0.0f);

glm::vec3 light = glm::vec3(0.0f, 200.0f, 400.0f);
glm::float32 ambientIntensity = glm::float32(0.50f);
glm::mat4 roatationMatrix = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 MVP = perspectiveMatrix * camera;
glm::mat4 MV = camera * roatationMatrix;
glm::mat4 V = camera;

glm::float32 rotation = 0;

void runProgram(GLFWwindow *window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    PNGImage texture = loadPNGFile("../gloom/src/assets/MFEP_Rock_3_DefaultMaterial_AlbedoTransparency.png");
    std::cout << "Loaded image with size " << texture.width << ", " << texture.height << std::endl;
    int textureID = loadGLTexture(texture);

    PNGImage ambient = loadPNGFile("../gloom/src/assets/MFEP_Rock_3_DefaultMaterial_AmbientOcclusion.png");
    std::cout << "Loaded image with size " << ambient.width << ", " << ambient.height << std::endl;
    int ambientID = loadGLTexture(ambient);

    PNGImage animation = loadPNGFile("../gloom/src/assets/animation.png");
    std::cout << "Loaded image with size " << ambient.width << ", " << ambient.height << std::endl;
    int animationID = loadGLTexture(animation);

    PNGImage normal = loadPNGFile("../gloom/src/assets/MFEP_Rock_3_DefaultMaterial_Normal.png");
    std::cout << "Loaded image with size " << ambient.width << ", " << ambient.height << std::endl;
    int normalID = loadGLTexture(normal);

    Mesh mesh = loadOBJ("../gloom/src/assets/MFEP_Rock_3.obj");

    unsigned int vaoID = generateVertexArray(mesh.vertices, mesh.textureCoordinates, mesh.normals, mesh.indices, mesh.vertexCount, mesh.indexCount);

    glBindTextureUnit(0, textureID);
    glBindTextureUnit(1, ambientID);
    glBindTextureUnit(2, animationID);
    glBindTextureUnit(3, normalID);

    Gloom::Shader shader;
    shader.attach("../gloom/shaders/simple.vert");
    shader.attach("../gloom/shaders/simple.frag");
    shader.link();
    shader.activate();

    GLuint MVPUniform = glGetUniformLocation(shader.get(), "MVP");
    GLuint MVUniform = glGetUniformLocation(shader.get(), "MV");
    GLuint VUniform = glGetUniformLocation(shader.get(), "V");
    GLuint lightUniform = glGetUniformLocation(shader.get(), "light");
    GLuint intensityUniform = glGetUniformLocation(shader.get(), "ambientIntensity");
    GLuint timeUniform = glGetUniformLocation(shader.get(), "time");
    GLuint cameraPositionUniform = glGetUniformLocation(shader.get(), "cameraPosition");

    float time = 0;

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        time = glfwGetTime();
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        light = glm::vec3(MV * glm::vec4(glm::vec3(sin(time) * 400.0f, 0.0f, cos(time) * 400.0f), 1.0f));
        //light = glm::vec3(0.0f);
        camera = glm::lookAt(cameraPos, cameraPos + orientation, up);

        V = camera;
        MV = camera * roatationMatrix;
        MVP = perspectiveMatrix * camera * roatationMatrix;

        glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(MVUniform, 1, GL_FALSE, &MV[0][0]);
        glUniformMatrix4fv(VUniform, 1, GL_FALSE, &V[0][0]);
        glUniform3fv(lightUniform, 1, &light[0]);
        glUniform3fv(cameraPositionUniform, 1, &cameraPos[0]);
        glUniform1fv(intensityUniform, 1, &ambientIntensity);
        glUniform1fv(timeUniform, 1, &time);

        // Draw your scene here
        glBindVertexArray(vaoID);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);

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
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        roatationMatrix = roatationMatrix * glm::rotate(glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        roatationMatrix = roatationMatrix * glm::rotate(glm::radians(-3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}