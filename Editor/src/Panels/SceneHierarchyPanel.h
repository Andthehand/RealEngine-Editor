#pragma once
#include "RealEngine.h"

#include "Panel.h"

namespace RealEngine {
	class SceneHierarchyPanel : public Panel {
		SceneHierarchyPanel(Ref<Scene> scene);

		void OnImGui() override;
	private:
		Ref<Scene> m_Scene;
	};
}
