#include "StatusPanel.h"

#include <imgui.h>

namespace RealEngine {
	void StatusPanel::OnImGui() {
		if (ImGui::Begin("Status Bar")) {
			ImGui::Text("FPS: %.3f", ImGui::GetIO().Framerate);
		}

		ImGui::End();
	}
}