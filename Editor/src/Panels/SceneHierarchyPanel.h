#pragma once
#include "RealEngine.h"

#include "Panel.h"

namespace RealEngine {
	class SceneHierarchyPanel : public Panel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Ref<Scene> scene);

		void DisplayAllEntities();

		void OnImGui() override;
	private:
		Ref<Scene> m_ActiveScene;

		Entity m_SelectedEntity;
	};
}
