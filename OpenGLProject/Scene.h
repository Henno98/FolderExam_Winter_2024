#pragma once
//#include "glad/glad.h"
//#include "GLFW/glfw3.h"
//#include <iostream>

class Scene
{
//
//	GLFWwindow* window;
//	int Width{};
//	int Height{};
//
//public:
//	Scene(int width, int height) : Width(width), Height(height), window(nullptr) {}
//    void Init()
//    {
//        // Initialize GLFW first
//        if (!glfwInit())
//        {
//            std::cout << "Failed to initialize GLFW\n";
//            return;
//        }
//
//        // Set OpenGL version BEFORE creating window
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//        // Create window
//        window = glfwCreateWindow(Width, Height, "OpenGLProject", NULL, NULL);
//        if (window == NULL)
//        {
//            std::cout << "Failed to create GLFW window\n";
//            glfwTerminate();
//            return;
//        }
//
//        // Make context current
//        glfwMakeContextCurrent(window);
//
//        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//        {
//            std::cout << "Failed to initialize GLAD\n";
//            return;
//        }
//
//        // Set viewport
//        glViewport(0, 0, Width, Height);
//        std::cout << glGetString(GL_VERSION) << std::endl;
//    }
//
//    void Update()
//    {
//        while (!glfwWindowShouldClose(window))
//        {
//            glClearColor(0.01f, 0.01f, 0.1f, 1.0f);
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//            glfwSwapBuffers(window);
//            glfwPollEvents();
//        }
//    }
//
//    ~Scene()
//    {
//        if (window) {
//            glfwDestroyWindow(window);
//            glfwTerminate();
//        }
//    }
//
//    GLFWwindow* GetWindow() const { return window; }
//	int GetWidth() const { return Width; }
//	int GetHeight() const { return Height; }
};

