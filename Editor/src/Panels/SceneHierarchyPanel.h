#pragma once
#include "RealEngine.h"

#include "Panel.h"

namespace RealEngine {
	class SceneHierarchyPanel : public Panel {
	public:
		SceneHierarchyPanel() = default;
		~SceneHierarchyPanel() = default;

		void DisplayAllEntities();

		void OnImGui() override;
	private:
		Entity m_SelectedEntity;
	};
}
