#pragma once

#include "starlet-engine/window_manager.hpp"
#include "starlet-engine/timer.hpp"
#include "starlet-controls/input_manager.hpp"

#include "starlet-scene/manager/scene_manager.hpp"

#include "starlet-graphics/manager/gl_state_manager.hpp"
#include "starlet-graphics/manager/resource_manager.hpp"
#include "starlet-graphics/manager/shader_manager.hpp"
#include "starlet-graphics/renderer/renderer.hpp"

namespace Starlet::Engine {
	class Engine {
	public:
		Engine();
		~Engine() = default;

		void setAssetPaths(const std::string& path);
		bool initialize(const unsigned int width, const unsigned int height, const char* title);

		bool loadScene(const std::string& sceneIn = "Default");
		void run();

		void updateViewport(const int width, const int height) { windowManager.updateViewport(width, height); }

		void onKey(const KeyEvent& event) { inputManager.onKey(event); }
		void onScroll(const Input::ScrollEvent& event) { inputManager.onScroll(event); }
		void onButton(const Input::MouseButtonEvent& event) { inputManager.onButton(event); }

		void toggleCursorLock() { inputManager.setCursorLocked(windowManager.switchCursorLock()); }
		void toggleWireframe() { glState.toggleWireframe(); };

	private:
		WindowManager windowManager;
		Timer timer;
		Input::InputManager inputManager;
		Graphics::GLStateManager glState;

		Graphics::ShaderManager shaderManager;
		Graphics::ResourceManager resourceManager;
		Scene::SceneManager sceneManager;

		Graphics::Renderer renderer;

		void handleKeyEvents(const std::vector<KeyEvent>& keyEvents);
		void handleButtonEvents(const std::vector<Input::MouseButtonEvent>& buttonEvents);
	};
}