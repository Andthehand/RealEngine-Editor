#include "ProjectCreatorLayer.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

namespace RealEngine {
	ProjectCreatorLayer::ProjectCreatorLayer()
		: Layer("ProjectCreatorLayer") {}
	
	void ProjectCreatorLayer::OnImGui() {
		RE_PROFILE_FUNCTION();

		static bool s_DockspaceOpen = true;
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &s_DockspaceOpen, window_flags);
		ImGui::PopStyleVar(3);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if(ImGui::MenuItem("Exit")) {
					Application::Get().Stop();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		ImGuiStyle& style = ImGui::GetStyle();
		const float centerOfCanvas = ImGui::GetContentRegionAvail().x * 0.5f;
		ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y * 0.25f);

		// Title
		auto boldFont = ImGui::GetIO().Fonts->Fonts[0];
		ImGui::PushFont(boldFont);
		ImGui::SetWindowFontScale(2.0f);
		const float titleWidth = ImGui::CalcTextSize("Create New Project").x;
		ImGui::SetCursorPosX(centerOfCanvas - (titleWidth * 0.5f));
		ImGui::TextUnformatted("Create New Project");
		ImGui::SetWindowFontScale(1.0f);
		ImGui::PopFont();

		// Input fields
		static std::string projectName;
		static std::string projectLocation;

		const float buttonWidth = ImGui::CalcTextSize("Browse...").x + style.FramePadding.x * 2.0f;
		const float inputWidth = 0.35f * ImGui::GetContentRegionAvail().x;
		const float totalWidth = inputWidth + buttonWidth + style.ItemSpacing.x;

		ImGui::SetCursorPosX(centerOfCanvas - (totalWidth * 0.5f));
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputTextWithHint("##Name", "MyProject", &projectName);
		ImGui::SameLine();
		if (ImGui::Button("Browse...")) {
			std::filesystem::path path = FileDialogs::OpenFile();
			if (!path.empty()) {
				projectLocation = path.parent_path().string() + std::string(1, std::filesystem::path::preferred_separator);
			}
		}

		// Location display
		const bool hasValidInput = !projectLocation.empty() && !projectName.empty();
		if (hasValidInput) {
			const std::string fullPath = projectLocation + projectName;
			const float pathWidth = ImGui::CalcTextSize(fullPath.c_str()).x;
			ImGui::SetCursorPosX(centerOfCanvas - (pathWidth * 0.5f));
			ImGui::TextUnformatted(fullPath.c_str());
		} else {
			const float invalidWidth = ImGui::CalcTextSize("Invalid Location").x;
			ImGui::SetCursorPosX(centerOfCanvas - (invalidWidth * 0.5f));
			ImGui::TextUnformatted("Invalid Location");
		}

		// Create button
		if (hasValidInput) {
			const float createButtonWidth = ImGui::CalcTextSize("Create Project").x + style.FramePadding.x * 2.0f;
			ImGui::SetCursorPosX(centerOfCanvas - (createButtonWidth * 0.5f));
			if (ImGui::Button("Create Project")) {
				Project::CreateNewProject(projectName, projectLocation);
			}
		}

		ImGui::End();
	}
	
	void ProjectCreatorLayer::OnEvent(Event& e) {
	}
}
