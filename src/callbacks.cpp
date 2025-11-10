#include "starlet-engine/callbacks.hpp"

#include "starlet-engine/engine.hpp"

#include <GLFW/glfw3.h>
#include <stdio.h>

void error_callback(const int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}
void key_callback(GLFWwindow* window, const int key, const int scanCode, const int action, const int mods) {
	Starlet::Engine::Engine* engine = static_cast<Starlet::Engine::Engine*>(glfwGetWindowUserPointer(window));
	if (!engine) return;
	engine->onKey({ key, action, mods });
}
void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
	Starlet::Engine::Engine* engine = static_cast<Starlet::Engine::Engine*>(glfwGetWindowUserPointer(window));
	if (!engine) return;
	engine->updateViewport(width, height);
}
void scroll_callback(GLFWwindow* window, const double xOffset, const double yOffset) {
	Starlet::Engine::Engine* engine = static_cast<Starlet::Engine::Engine*>(glfwGetWindowUserPointer(window));
	if (!engine) return;
	engine->onScroll({ xOffset, yOffset });
}
void mouse_button_callback(GLFWwindow* window, const int button, const int action, const int mods) {
	Starlet::Engine::Engine* engine = static_cast<Starlet::Engine::Engine*>(glfwGetWindowUserPointer(window));
	if (!engine) return;
	engine->onButton({ button, action, mods });
}