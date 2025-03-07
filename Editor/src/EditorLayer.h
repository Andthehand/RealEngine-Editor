#pragma once
#include "RealEngine.h"

#include "Panels/FileExplorerPanel.h"
#include "Panels/PropertiesPanel.h"

#include <glm/glm.hpp>

namespace RealEngine {
	class EditorLayer : public Layer {
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(const float deltaTime) override;
		virtual void OnImGui() override;
		virtual void OnEvent(Event& event) override;

	private:
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		FileExplorerPanel m_FileExplorerPanel;
		PropertiesPanel m_PropertiesPanel;
	};
}
