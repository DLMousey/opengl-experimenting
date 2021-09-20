#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    // ----------------------------------------------------------------------------------------------------------------
    // Initialise openGL via GLFW, tell it we want to use OpenGL 3.3, and we want to use core mode for more control
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ----------------------------------------------------------------------------------------------------------------
    // Initialise GLAD, manages function pointers for OpenGL (hardware/OS abstraction?)

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD!" << std::endl;
        return -1;
    }

    Shader shader("vertex", "fragment");

    // ----------------------------------------------------------------------------------------------------------------
    // SET UP VERTEX DATA + BUFFERS, CONFIGURE VERTEX ATTRIBUTES

    // Set of vertices to define a triangle, afaik it's in X Y Z format, Z is at 0.0f for all because we only want a 2d triangle
    float vertices[] = {
        // positions                // colours
        0.5f,   -0.5f,  0.0f,       0.8f, 0.0f, 0.0f,
        -0.5f,  -0.5f,  0.0f,       0.5f, 1.0f, 0.5f,
        0.0f,   0.5f,   0.0f,       0.2f, 0.6f, 0.6f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Uncomment to draw in wireframe
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Set up a render loop and actually do the thing
    while(!glfwWindowShouldClose(window))
    {
        // Run any logic required to process user input before rendering anything, since we'll be responding
        // to user input to change what renders
        processInput(window);

        // Do rendering here, in this case clear the screen and replace everything on it with a solid green colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers prevents flashing of not-rendered content
        glfwSwapBuffers(window);

        // Check for any other events that need responding to
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // User requested the window to close, clean up and exit
    glfwTerminate();
    return 0;
}

// Ensure that the openGL viewport is resized when the user resizes the window, this is a callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}