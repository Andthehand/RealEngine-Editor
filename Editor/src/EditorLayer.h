#pragma once
#include "RealEngine.h"

#include "Panels/FileExplorerPanel.h"
#include "Panels/PropertiesPanel.h"

#include <glm/glm.hpp>

namespace RealEngine {
	/**
	 * @class EditorLayer
	 * @brief The core layer responsible for rendering and interacting with the editor interface.
	 *
	 * This class manages rendering the ImGui-based UI, handling the framebuffer viewport,
	 * and dispatching events to active panels like the File Explorer and Properties panel.
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
		Ref<Scene> m_ActiveScene;

		/// Framebuffer used to render the scene in the editor viewport.
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		/// File explorer panel for browsing assets and directories.
		FileExplorerPanel m_FileExplorerPanel;
		/// Properties panel for displaying and editing object properties.
		PropertiesPanel m_PropertiesPanel;
	};
}
