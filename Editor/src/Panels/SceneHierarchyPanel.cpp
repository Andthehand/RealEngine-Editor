#include "SceneHierarchyPanel.h"

#include "Events/PanelEvents.h"

#include <imgui.h>

namespace RealEngine {
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> scene)
		: m_ActiveScene(scene) {
	}

	void SceneHierarchyPanel::DisplayAllEntities() {
		RE_PROFILE_FUNCTION();
		constexpr ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		for (auto& [id, entity] : m_ActiveScene->m_EntityMap) {
			TagComponent& tagComponent = entity.GetComponent<TagComponent>();

			ImGuiTreeNodeFlags node_flags = base_flags;
			if (m_SelectedEntity == entity) {
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}

			ImGui::PushID(static_cast<int>(id));
			bool node_open = ImGui::TreeNodeEx(tagComponent.Tag.c_str(), node_flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
				m_SelectedEntity = entity;

				PannelEntitySelectEvent event(entity);
				RE_RAISE_EVENT(event);
			}

			if (node_open) {

				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}

	void SceneHierarchyPanel::OnImGui() {
		RE_PROFILE_FUNCTION();
		ImGui::Begin("Scene Hierarchy");

		DisplayAllEntities();

		ImGui::End();
	}
}
