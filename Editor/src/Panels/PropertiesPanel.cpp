#include "PropertiesPanel.h"

#include "Events/PanelEvents.h"

#include <imgui.h>

namespace RealEngine {	
	namespace Utils {
		void DrawVec3Control(const std::string& label, glm::vec3& values) {
			ImGui::Text(label.c_str());
			ImGui::SameLine();
			ImGui::DragFloat3(("##" + label).c_str(), &values.x, 0.1f, -100.0f, 100.0f);
		}
	}

	void PropertiesPanel::ShowFolderProperties() {
		ImGui::Text(("Selected Folder: " + m_SelectedFolder.string()).c_str());
	}

	void PropertiesPanel::ShowEntityProperties() {
		ImGui::Text("Entity Properties");
		if (m_SelectedEntity.HasComponent<TransformComponent>()) {
			auto& transform = m_SelectedEntity.GetComponent<TransformComponent>();
			Utils::DrawVec3Control("Position", transform.Position);
		}
	}
	
	void PropertiesPanel::OnImGui() {
		RE_PROFILE_FUNCTION();

		ImGui::Begin("Properties");

		switch (m_CurrentView) {
			case CurrentView::FolderView:
				ShowFolderProperties();
				break;
			case CurrentView::EntityView:
				ShowEntityProperties();
				break;
			case CurrentView::None:
				ImGui::Text("No selection");
				break;
		}

		float dt = Application::Get().GetWindow().GetDeltaTime();
		float fps = 1.0f / dt;
		ImGui::Text("FPS: %f", fps);
		ImGui::Text("DeltaTime: %f", dt);
		ImGui::End();
	}

	void PropertiesPanel::OnEvent(Event& event) {
		RE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<PannelFolderSelectEvent>([this](PannelFolderSelectEvent& e) {
			m_SelectedFolder = e.GetPath();
			m_CurrentView = CurrentView::FolderView;
			return true;
		});

		dispatcher.Dispatch<PannelEntitySelectEvent>([this](PannelEntitySelectEvent& e) {
			m_SelectedEntity = e.GetEntity();
			m_CurrentView = CurrentView::EntityView;
			return true;
		});
	}
}