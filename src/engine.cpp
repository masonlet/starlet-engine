#include "StarletEngine/engine.hpp"
#include "StarletLogger/logger.hpp"

#include "StarletScene/component/model.hpp"
#include "StarletScene/component/textureData.hpp"

#include "StarletScene/system/cameraMoveSystem.hpp"
#include "StarletScene/system/cameraLookSystem.hpp"
#include "StarletScene/system/cameraFovSystem.hpp"
#include "StarletScene/system/velocitySystem.hpp"

#include <GLFW/glfw3.h>

namespace Starlet::Engine {
  Engine::Engine() : renderer(resourceManager) {}

  void Engine::setAssetPaths(const std::string& path) {
    shaderManager.setBasePath((path + "/shaders/").c_str());
    resourceManager.setBasePath(path);
    sceneManager.setBasePath((path + "/scenes/").c_str());
  }

  bool Engine::initialize(const unsigned int width, const unsigned int height, const char* title) {
    Logger::debugLog("Engine", "initialize", "Start time: " + std::to_string(glfwGetTime()));

    if (!windowManager.createWindow(width, height, title))
      return Logger::error("Engine", "initialize", "Failed to initialize window");

    if (!shaderManager.createProgramFromPaths("shader1", "vertex_shader.glsl", "fragment_shader.glsl"))
      return Logger::error("Engine", "initialize", "Failed to create shader program from file");

    if (!glState.setProgram(shaderManager.getProgramID("shader1")))
      return Logger::error("GLStateManager", "init", "Failed to set initial program");

    if (!renderer.init(glState.getProgram()))
      return Logger::error("Engine", "initialize", "Failed to setup shaders for renderer");

    glState.setGLStateDefault();
    windowManager.setWindowPointer(this);
    return Logger::debugLog("Engine", "initialize", "Finish Time: " + std::to_string(glfwGetTime()));
  }

  bool Engine::loadScene(const std::string& sceneIn) {
    Logger::debugLog("Engine", "loadScene", "Start time: " + std::to_string(glfwGetTime()));

    if (sceneIn.empty()) {
      if (!sceneManager.loadTxtScene("EmptyScene.txt"))
        return Logger::error("Engine", "loadScene", "No scene loaded and failed to load Default \"EmptyScene\"");
    }
    else if (!sceneManager.loadTxtScene(sceneIn + ".txt"))
      return Logger::error("Engine", "loadScene", "Failed to load scene: " + sceneIn);

    Logger::debugLog("ResourceLoader", "loadMeshes", "Start time: " + std::to_string(glfwGetTime()));
    if (!resourceManager.loadMeshes(sceneManager.getScene().getComponentsOfType<Scene::Model>()))
      return Logger::error("Engine", "loadMeshes", "Failed to load meshes for scene: " + sceneIn);
    Logger::debugLog("ResourceLoader", "loadMeshes", "Finish time: " + std::to_string(glfwGetTime()));

    Logger::debugLog("ResourceLoader", "loadTextures", "Start time: " + std::to_string(glfwGetTime()));
    if (!resourceManager.loadTextures(sceneManager.getScene().getComponentsOfType<Scene::TextureData>()))
      return Logger::error("Engine", "loadTextures", "Failed to load textures for scene: " + sceneIn);
    Logger::debugLog("ResourceLoader", "loadTextures", "Finish time: " + std::to_string(glfwGetTime()));

    Logger::debugLog("ResourceLoader", "processPrimitives", "Start time: " + std::to_string(glfwGetTime()));
    if (!resourceManager.processPrimitives(sceneManager))
      return Logger::error("Engine", "processPrimitives", "Failed to process primitives for scene: " + sceneIn);
    Logger::debugLog("ResourceLoader", "processPrimitives", "Finish time: " + std::to_string(glfwGetTime()));

    Logger::debugLog("ResourceLoader", "processGrids", "Start time: " + std::to_string(glfwGetTime()));
    if (!resourceManager.processGrids(sceneManager))
      return Logger::error("Engine", "processGrids", "Failed to process grids for scene: " + sceneIn);
    Logger::debugLog("ResourceLoader", "processGrids", "Finish time: " + std::to_string(glfwGetTime()));

    Logger::debugLog("ResourceLoader", "processTextureConnection", "Start time: " + std::to_string(glfwGetTime()));
    if (!resourceManager.processTextureConnections(sceneManager.getScene()))
      return Logger::error("Engine", "processTextureConnection", "Failed to connect texture handles for scene: " + sceneIn);
    Logger::debugLog("ResourceLoader", "processTextureConnection", "Finish time: " + std::to_string(glfwGetTime()));

    sceneManager.getScene().registerSystem(std::make_unique<Scene::CameraMoveSystem>());
    sceneManager.getScene().registerSystem(std::make_unique<Scene::CameraLookSystem>());
    sceneManager.getScene().registerSystem(std::make_unique<Scene::CameraFovSystem>());
    sceneManager.getScene().registerSystem(std::make_unique<Scene::VelocitySystem>());

    return Logger::debugLog("Engine", "loadScene", "Finish Time: " + std::to_string(glfwGetTime()));
  }

  void Engine::run() {
    windowManager.switchActiveWindowVisibility();

    while (!windowManager.shouldClose()) {
      const float deltaTime = timer.tick();

      inputManager.reset();
      windowManager.pollEvents();
      inputManager.updateMousePosition(windowManager.getGLFWwindow());

      handleKeyEvents(inputManager.consumeKeyEvents());
      handleButtonEvents(inputManager.consumeButtonEvents());

      sceneManager.getScene().updateSystems(inputManager, deltaTime);
      renderer.renderFrame(glState.getProgram(), sceneManager.getScene(), windowManager.getAspect());

      windowManager.swapBuffers();
    }
  }

  void Engine::handleKeyEvents(const std::vector<KeyEvent>& keyEvents) {
    for (const KeyEvent& event : keyEvents) {
      if (event.action != GLFW_PRESS) continue;

      switch (event.key) {
      case GLFW_KEY_ESCAPE: windowManager.requestClose(); break;

#ifndef NDEBUG
      case GLFW_KEY_P: toggleWireframe();  break;
      case GLFW_KEY_C: toggleCursorLock(); break;
#endif
      }
    }
  }

  void Engine::handleButtonEvents(const std::vector<Input::MouseButtonEvent>& buttonEvents) {
    for (const Input::MouseButtonEvent& event : buttonEvents) {
      std::string buttonName;
      switch (event.button) {
      case GLFW_MOUSE_BUTTON_LEFT:   buttonName = "Left"; break;
      case GLFW_MOUSE_BUTTON_RIGHT:  buttonName = "Right"; break;
      case GLFW_MOUSE_BUTTON_MIDDLE: buttonName = "Middle"; break;
      case GLFW_MOUSE_BUTTON_4:      buttonName = "Side_Forward"; break;
      case GLFW_MOUSE_BUTTON_5:      buttonName = "Side_Backward"; break;
      default: buttonName = "Unknown"; break;
      }

      std::string actionName;
      switch (event.action) {
      case GLFW_PRESS:   actionName = "Pressed"; break;
      case GLFW_RELEASE: actionName = "Released"; break;
      default: return;
      }

      Logger::debugLog("Input", "Mouse", "Button " + buttonName + " " + actionName);
    }
  }
}