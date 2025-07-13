#ifndef WINDOW_H
#define WINDOW_H

#include </Users/shobhitmehrotra/Documents/glfw-3.4/include/GLFW/glfw3.h>

GLFWwindow* createWindow(int width, int height, const char* title);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif // WINDOW_H
