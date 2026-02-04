#pragma once
#include "RealEngine.h"

#include "Panels/FileExplorerPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/StatusPanel.h"
#include "Panels/LoggerPanel.h"

#include <glm/glm.hpp>

namespace RealEngine {
	/**
	 * @class EditorLayer
	 * @brief The core layer responsible for rendering and interacting with the editor interface.
	 *
	 * This class manages rendering the ImGui-based UI, handling the framebuffer viewport,
	 * and dispatching events to active panels like the File Explorer and Properties panel.
	 * The First command line argument is treated as the project path to load on startup.
	 */
	class EditorLayer : public Layer {
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(const float deltaTime) override;
		virtual void OnImGui() override;
		virtual void OnEvent(Event& event) override;
	private:
		void LoadProject();

		void StartDockspace();
		void EndDockspace();

		void CheckShortcuts();

		// Shortcut/menu actions
		void NewScene();
		void OpenScene();

		// Also saves the scene if it has a valid file path
		void SaveProject();

		bool OnWindowCloseEvent(WindowCloseEvent& e);
	private:
		enum class SceneState {
			Edit = 0,
			Play = 1
		};

		SceneState m_SceneState = SceneState::Edit;

		/// Framebuffer used to render the scene in the editor viewport.
		static constexpr float MIN_DOCKED_WINDOW_SIZE = 250.0f;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		EditorCamera m_EditorCamera;
		bool m_ViewportFocused = false;

		// This is used when in runtime to keep a copy of the "Real" Scene
		Ref<Scene> m_SwapScene;

		static constexpr float ICON_OFFSET_FROM_TOP = 10.0f;
		static constexpr float ICON_SIZE = 25.0f;
		Ref<Texture2D> m_PlayIcon;
		Ref<Texture2D> m_PauseIcon;

		/// File explorer panel for browsing assets and directories.
		FileExplorerPanel m_FileExplorerPanel;
		/// Properties panel for displaying and editing object properties.
		PropertiesPanel m_PropertiesPanel;
		/// Scene hierarchy panel for managing entities in the current scene.
		SceneHierarchyPanel m_SceneHierarchyPanel;
		LoggerPanel m_LoggerPanel;
		StatusPanel m_StatusPanel;
	};
}
