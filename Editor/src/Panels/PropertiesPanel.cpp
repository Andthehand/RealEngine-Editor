#include "PropertiesPanel.h"

#include "Events/PanelEvents.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace RealEngine {	
	namespace Utils {
		bool DrawVec3Control(const std::string& label, glm::vec3* values, float labelWidth = 110.0f) {
			ImGui::TextUnformatted(label.c_str());

			ImGui::SameLine();
			ImGui::SetCursorPosX(labelWidth);

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			return ImGui::DragFloat3(("##" + label).c_str(), &values->x, 0.1f, -100.0f, 100.0f);
		}

		void DrawVec4Control(const std::string& label, glm::vec4* values, float labelWidth = 110.0f) {
			ImGui::TextUnformatted(label.c_str());

			ImGui::SameLine();
			ImGui::SetCursorPosX(labelWidth);

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::DragFloat4(("##" + label).c_str(), &values->x, 0.1f, -100.0f, 100.0f);
		}

		void DrawColorEdit4(const std::string& label, glm::vec4* color, float labelWidth = 110.0f) {
			ImGui::TextUnformatted(label.c_str());
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(labelWidth);

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::ColorEdit4(("##" + label).c_str(), &color->x);
		}
	}

	template<typename T>
	void PropertiesPanel::DisplayComponent(T* Component) {
		RE_CORE_ASSERT(false, "No display function for component type: {0}", T::GetName());
	}

	template<>
	void PropertiesPanel::DisplayComponent<TransformComponent>(TransformComponent* component) {
		glm::vec3 position = component->GetPosition();
		if (Utils::DrawVec3Control("Position", &position))
			component->SetPosition(position);

		glm::vec3 eulerRotation = component->GetRotationEuler();
		if (Utils::DrawVec3Control("Rotation", &eulerRotation))
			component->SetRotationEuler(eulerRotation);

		glm::vec3 scale = component->GetScale();
		if (Utils::DrawVec3Control("Scale", &scale))
			component->SetScale(scale);
	}

	template<>
	void PropertiesPanel::DisplayComponent<SpriteRendererComponent>(SpriteRendererComponent* component) {
		Utils::DrawColorEdit4("Color", &component->Color);
	}

	template<typename... Components>
	void PropertiesPanel::DisplayComponents(ComponentList::ComponentGroup<Components...>) {
		// Fold expression over comma operator to handle each component
		(([this]() {
			if(m_SelectedEntity.HasComponent<Components>() == false)
				return;

			// Has to be deferred until later so OpenPopupOnItemClick works correctly
			bool isOpen = ImGui::CollapsingHeader(Components::GetName(), ImGuiTreeNodeFlags_DefaultOpen);
			
			// Right-click context menu to remove component
			ImGui::OpenPopupOnItemClick(Components::GetName(), ImGuiPopupFlags_MouseButtonRight);
			if (ImGui::BeginPopupContextItem(Components::GetName())) {
				if (ImGui::Selectable("Remove component")) {
					m_SelectedEntity.RemoveComponent<Components>();
				}

				ImGui::EndPopup();
			}

			// Display component details if the header is open
			if (isOpen) {
				if (auto* component = m_SelectedEntity.TryGetComponent<Components>()) {
					DisplayComponent(component);
				}
			}

		}()), ...);
	}

	template<typename ...Components>
	void PropertiesPanel::DisplayAddComponentEntries(ComponentList::ComponentGroup<Components...>) {
		(([this]() {
			if(m_SelectedEntity.HasComponent<Components>())
				return;

			if (ImGui::Selectable(Components::GetName())) {
				m_SelectedEntity.AddComponent<Components>();
			}
		}()), ...);
	}

	void PropertiesPanel::ShowFolderProperties() {
		ImGui::TextUnformatted(("Selected Folder: " + m_SelectedFolder.string()).c_str());
	}

	void PropertiesPanel::ShowEntityProperties() {
		m_SelectedEntity.GetComponent<TagComponent>();
		const char* addComponentBtnLabel = "+";

		const float framePaddingX = ImGui::GetStyle().FramePadding.x * 2.0f;
		const float addComponentBtnWidth = ImGui::CalcTextSize(addComponentBtnLabel).x + framePaddingX;
		const float availableWidth = ImGui::GetContentRegionAvail().x;

		ImGui::PushItemWidth(availableWidth - addComponentBtnWidth - framePaddingX);
		ImGui::InputText("##Tag", &m_SelectedEntity.GetComponent<TagComponent>().Tag);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushFont(boldFont);
		if (ImGui::Button(addComponentBtnLabel, ImVec2(addComponentBtnWidth, 0.0f))) {
			ImGui::OpenPopup("AddComponent");
		}
		ImGui::PopFont(); // Bold font for the button

		if (ImGui::BeginPopup("AddComponent")) {
			DisplayAddComponentEntries(ComponentList::GetAllComponents());

			ImGui::EndPopup();
		}

		DisplayComponents(ComponentList::GetAllComponents());
	}
	
	void PropertiesPanel::OnImGui() {
		RE_PROFILE_FUNCTION();

		if (ImGui::Begin("Properties")) {
			switch (m_CurrentView) {
			case CurrentView::FolderView:
				ShowFolderProperties();
				break;
			case CurrentView::EntityView:
				ShowEntityProperties();
				break;
			case CurrentView::None:
				ImGui::TextUnformatted("No selection");
				break;
			}
		}

		ImGui::End();
	}

	void PropertiesPanel::OnEvent(Event& event) {
		RE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<PanelFolderSelectEvent>([this](PanelFolderSelectEvent& e) {
			m_SelectedFolder = e.GetPath();
			m_CurrentView = CurrentView::FolderView;
			return true;
		});

		dispatcher.Dispatch<PanelEntitySelectEvent>([this](PanelEntitySelectEvent& e) {
			m_SelectedEntity = e.GetEntity();
			m_CurrentView = CurrentView::EntityView;
			return true;
		});

		dispatcher.Dispatch<PanelEntityDeselectEvent>([this](PanelEntityDeselectEvent& e) {
			m_SelectedEntity = {};

			if(m_CurrentView == CurrentView::EntityView)
				m_CurrentView = CurrentView::None;

			return true;
		});
	}
}