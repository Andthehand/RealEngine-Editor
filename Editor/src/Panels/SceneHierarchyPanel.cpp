#include "SceneHierarchyPanel.h"

#include "Events/PanelEvents.h"

#include <imgui.h>

namespace RealEngine {
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> scene)
		: m_ActiveScene(scene) { }

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
			// Entity name
			bool node_open = ImGui::TreeNodeEx(tagComponent.Tag.c_str(), node_flags);

			// If the item is clicked (but not toggled open i.e. clicking the arrow)
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
				m_SelectedEntity = entity;

				PanelEntitySelectEvent event(entity);
				RE_RAISE_EVENT(event);
			}

			ImGui::OpenPopupOnItemClick(tagComponent.Tag.c_str(), ImGuiPopupFlags_MouseButtonRight);
			if (ImGui::BeginPopupContextItem(tagComponent.Tag.c_str())) {
				if (ImGui::Selectable("Remove Entity")) {
					if (m_SelectedEntity == entity) {
						m_SelectedEntity = {};

						PanelEntityDeselectEvent event;
						RE_RAISE_EVENT(event);
					}

					m_ActiveScene->DestroyEntity(entity);

					// Deleting the entity invalidates the iterator
					// This doesn't cause issues because we re-draw the panel every frame
					ImGui::EndPopup();
					ImGui::PopID();
					return;
				}

				ImGui::EndPopup();
			}

			if (node_open) {
				// TODO: Add children entities here

				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}

	void SceneHierarchyPanel::OnImGui() {
		RE_PROFILE_FUNCTION();
		if (ImGui::Begin("Scene Hierarchy")) {
			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
				if (ImGui::Selectable("Add Entity")) {
					m_ActiveScene->CreateEntity("New Entity");
				}

				ImGui::EndPopup();
			}

			DisplayAllEntities();
		}

		ImGui::End();
	}
}
