// Example of text usage, using glfw for the window system binding.
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#include <GL/glfw.h>

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit()) return 1;

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glClearColor(1, 1, 1, 1);

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
