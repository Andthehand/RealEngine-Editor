#include "StatusPanel.h"

#include <imgui.h>

namespace RealEngine {
	void StatusPanel::OnImGui() {
		RE_PROFILE_FUNCTION();
		if (ImGui::Begin("Status Bar")) {
			ImGui::Text("FPS: %.3f", ImGui::GetIO().Framerate);

			if(ImGui::TreeNode("Render Stats")) {
				static const RenderStats& renderStats = Project::GetCurrentScene()->GetRenderStats();
				ImGui::Text("Draw Calls: %d", renderStats.DrawCalls);
				
				ImGui::Separator();
				
				// 2D
				ImGui::Text("2D:");
				ImGui::Text("  Draw Calls: %d", renderStats.Sprites.DrawCalls);
				ImGui::Text("  Quads: %d", renderStats.Sprites.QuadCount);
				ImGui::Text("  Texture Binds: %d", renderStats.Sprites.TextureBinds);
				
				ImGui::Separator();

				// Text
				ImGui::Text("Text:");
				ImGui::Text("  Draw Calls: %d", renderStats.Text.DrawCalls);
				ImGui::Text("  Glyphs: %d", renderStats.Text.GlyphCount);

				ImGui::TreePop();
			}
		}

		ImGui::End();
	}
}