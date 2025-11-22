#include "starlet-engine/window.hpp"
#include "starlet-logger/logger.hpp"

#include "starlet-controls/input_manager.hpp"

#include <GLFW/glfw3.h>
#include <cstdio>

namespace Starlet::Engine {
	Window::~Window() {
		if (window) {
			glfwDestroyWindow(window);
			window = nullptr;
		}
	}

	bool Window::createWindow(const unsigned int widthIn, const unsigned int heightIn, const char* title) {
		window = glfwCreateWindow(widthIn, heightIn, title, nullptr, nullptr);
		if (!window) return Logger::error("Window", "createWindow", "Failed to create GLFW window");

		width = widthIn;
		height = heightIn;
		return Logger::debug("Window", "createWindow", "Created window: " + std::string(title) + " - " + std::to_string(width) + " x " + std::to_string(height));
	}
	bool Window::shouldClose() const {
		return window ? glfwWindowShouldClose(window) : true;
	}

	void Window::pollEvents() const {
		if (window) glfwPollEvents();
	}
	void Window::swapBuffers() const {
		if (window) glfwSwapBuffers(window);
	}
	void Window::requestClose() const {
		if (window) glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void Window::setWindowPointer(void* userPointer) const {
		if (userPointer == nullptr) Logger::error("Window", "setWindowPointer", "User pointer cannot be null.");
		else if (window) glfwSetWindowUserPointer(window, userPointer);
	}
	void Window::setCurrentWindow() const {
		if (window) glfwMakeContextCurrent(window);
	}

	void Window::updateViewport(const unsigned int widthIn, const unsigned int heightIn) {
		if (window) {
			width = widthIn;
			height = heightIn;
			glViewport(0, 0, width, height);
		}
	}

	bool Window::switchActiveWindowVisibility() {
		if (!window) return Logger::error("Window", "switchActiveWindowVisibility", "No active window to switch visibility.");

		const int isVisible = (glfwGetWindowAttrib(window, GLFW_VISIBLE) == GLFW_TRUE) ? GLFW_FALSE : GLFW_TRUE;
		const bool windowVisible = (isVisible == GLFW_TRUE);
		
		windowVisible ? glfwShowWindow(window) : glfwHideWindow(window);
		windowVisible ? Logger::debug("Window", "switchWindowVisibility", "Window shown")
			            : Logger::debug("Window", "switchWindowVisibility", "Window hidden");
		return windowVisible;
	}
	bool Window::switchCursorLock() {
		if (!window) return Logger::error("Window", "switchCursorLock", "No active window to switch cursor lock.");

		const int cursorMode = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		const bool cursorLocked = (cursorMode == GLFW_CURSOR_DISABLED);
		cursorLocked ? Logger::debug("Window", "switchCursorLock", "Cursor locked")
			           : Logger::debug("Window", "switchCursorLock", "Cursor unlocked");
		return cursorLocked;
	}
}