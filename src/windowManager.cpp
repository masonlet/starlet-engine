#include "starlet-engine/windowManager.hpp"
#include "starlet-logger/logger.hpp"

#include "starlet-engine/callbacks.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

namespace Starlet::Engine {
  static constexpr int GL_MAJOR{ 3 };
  static constexpr int GL_MINOR{ 3 };

  WindowManager::WindowManager() {
    if (!glfwInit()) Logger::error("WindowManager", "Constructor", "Failed to initialize GLFW\n");
  }
  WindowManager::~WindowManager() {
    glfwTerminate();
  }

  bool WindowManager::createWindow(const unsigned int width, const unsigned int height, const char* title) {
    Logger::debugLog("WindowManager", "createWindow", "Start time : " + std::to_string(glfwGetTime()));

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MINOR);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    activeWindow = std::make_unique<Window>();
    if (!activeWindow->createWindow(width, height, title)) {
      activeWindow.reset();
      return Logger::error("WindowManager", "createWindow", "Window creation failed");
    }

    activeWindow->setCurrentWindow();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      return Logger::error("WindowManager", "createWindow", "Failed to initialize GLAD");

    GLFWwindow* window = activeWindow->getGLFWwindow();
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    Logger::debugLog("Window", "OpenGL", "OpenGL Info");
    Logger::debugLog("Window", "OpenGL", "Version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    Logger::debugLog("Window", "OpenGL", "Vendor: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));
    Logger::debugLog("Window", "OpenGL", "Renderer: " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
    return Logger::debugLog("WindowManager", "createWindow", "Finish time: " + std::to_string(glfwGetTime()));
  }
}