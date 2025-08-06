#include "SceneHierarchyPanel.h"

#include <imgui.h>

namespace RealEngine {
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> scene)
		: m_Scene(scene) {
	}

	void SceneHierarchyPanel::OnImGui() {
		RE_PROFILE_FUNCTION();
		ImGui::Begin("Scene Hierarchy");



		ImGui::End();
	}
}
