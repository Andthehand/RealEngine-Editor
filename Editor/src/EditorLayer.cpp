#include "EditorLayer.h"

#include <imgui.h>

namespace RealEngine {
	EditorLayer::EditorLayer()
		: Layer("Editor"), m_EditorCamera(90.0f, Application::Get().GetWindow().GetAspectRatio(), 0.0f, 1000.0f) { }

	void EditorLayer::OnAttach() {
		RE_PROFILE_FUNCTION();

		RenderCommands::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		spec.Attachments = { FramebufferTextureFormat::RGBA8 };

		m_Framebuffer = Framebuffer::Create(spec);
		m_EditorCamera.SetViewportSize((float)spec.Width, (float)spec.Height);

		Project::CreateNewProject();
		m_FileExplorerPanel.SetCurrentDirectory(Project::GetAssetsPath());
	}

	void EditorLayer::OnDetach() {
		RE_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(const float deltaTime) {
		RE_PROFILE_FUNCTION();

		// Resize the camera and framebuffer if the viewport size has changed
		// Static because we only need a reference once
		static const FramebufferSpecification& spec = m_Framebuffer->GetSpecification();
		if(m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {

			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_Framebuffer->Bind();
		RenderCommands::Clear();

		switch (m_SceneState) {
			case SceneState::Edit:
				m_EditorCamera.OnUpdate(deltaTime);

				Project::GetCurrentScene()->OnUpdateEditor(deltaTime, m_EditorCamera);
				break;
			case SceneState::Play:
				Project::GetCurrentScene()->OnUpdateRuntime(deltaTime);
				break;
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGui() {
		RE_PROFILE_FUNCTION();

		StartDockspace();
		CheckShortcuts();

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				// Shortcuts are handled in CheckShortcuts()
				if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
					NewScene();
				}
				if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
					OpenScene();
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
					SaveScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("New Project", "Ctrl+Shift+N")) {
					NewProject();
				}
				if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O")) {
					OpenProject();
				}
				if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S")) {
					SaveProject();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) {
					Application::Get().Stop();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {
				ImGui::MenuItem("Properties", NULL, &m_PropertiesPanel.IsVisible);
				ImGui::MenuItem("FileExplorer", NULL, &m_FileExplorerPanel.IsVisible);
				ImGui::MenuItem("SceneHierarchy", NULL, &m_SceneHierarchyPanel.IsVisible);
				ImGui::MenuItem("Status", NULL, &m_StatusPanel.IsVisible);
				ImGui::MenuItem("Logger", NULL, &m_LoggerPanel.IsVisible);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window")) {
				if (ImGui::MenuItem("Vsync", NULL, Application::Get().GetWindow().GetVsync())) {
					Application::Get().GetWindow().SetVSync(!Application::Get().GetWindow().GetVsync());
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (m_PropertiesPanel.IsVisible)
			m_PropertiesPanel.OnImGui();
		if (m_FileExplorerPanel.IsVisible)
			m_FileExplorerPanel.OnImGui();
		if (m_SceneHierarchyPanel.IsVisible)
			m_SceneHierarchyPanel.OnImGui();
		if (m_StatusPanel.IsVisible)
			m_StatusPanel.OnImGui();
		if (m_LoggerPanel.IsVisible)
			m_LoggerPanel.OnImGui();

		//ImGui Viewport Window/Panel
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();

		ImVec2 viewpoerPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewpoerPanelSize.x, viewpoerPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		EndDockspace();

		ImGui::ShowDemoWindow();
	}

	void EditorLayer::OnEvent(Event& event) {
		RE_PROFILE_FUNCTION();

		m_EditorCamera.OnEvent(event);
		m_PropertiesPanel.OnEvent(event);
	}

	void EditorLayer::StartDockspace() {
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

		// Sets the minimum window size for docked windows
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(MIN_DOCKED_WINDOW_SIZE, ImGui::GetFrameHeight()));
		// Starts the dockspace
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		ImGui::PopStyleVar();
	}

	void EditorLayer::EndDockspace() {
		ImGui::End();
	}

	void EditorLayer::CheckShortcuts() {
		if (ImGui::Shortcut(ImGuiKey_N | ImGuiMod_Ctrl, ImGuiInputFlags_RouteGlobal)) {
			NewScene();
		}
		if (ImGui::Shortcut(ImGuiKey_O | ImGuiMod_Ctrl, ImGuiInputFlags_RouteGlobal)) {
			OpenScene();
		}
		if (ImGui::Shortcut(ImGuiKey_S | ImGuiMod_Ctrl, ImGuiInputFlags_RouteGlobal)) {
			SaveScene();
		}

		if (ImGui::Shortcut(ImGuiKey_N | ImGuiMod_Ctrl | ImGuiMod_Shift, ImGuiInputFlags_RouteGlobal)) {
			NewProject();
		}
		if (ImGui::Shortcut(ImGuiKey_O | ImGuiMod_Ctrl | ImGuiMod_Shift, ImGuiInputFlags_RouteGlobal)) {
			OpenProject();
		}
		if (ImGui::Shortcut(ImGuiKey_S | ImGuiMod_Ctrl | ImGuiMod_Shift, ImGuiInputFlags_RouteGlobal)) {
			SaveProject();
		}
	}

	void EditorLayer::NewScene() {
		Project::GetCurrentScene()->Save();
		Project::SetCurrentScene(CreateRef<Scene>());
	}

	void EditorLayer::OpenScene() {
		if (!Project::IsFullyInitialized()) {
			RE_CORE_WARN("Cannot open a scene when no project is loaded!");
			return;
		}

		std::filesystem::path sceneFile = FileDialogs::OpenFile("Real Engine Scene (*.rescene)\0*.rescene\0");
		
		if (!sceneFile.empty()) {
			Project::GetCurrentScene()->Save();
			Project::SetCurrentScene(CreateRef<Scene>(sceneFile));
		} else {
			RE_CORE_WARN("Scene open was canceled or failed!");
		}
	}

	void EditorLayer::SaveScene() {
		if (!Project::IsFullyInitialized()) {
			RE_CORE_WARN("Cannot save a scene when no project is loaded!");
			return;
		}

		if (Project::GetCurrentScene()->GetFilePath().empty()) {
			std::filesystem::path sceneFile = FileDialogs::SaveFile("Real Engine Scene (*.rescene)\0*.rescene\0");
			if (sceneFile.empty()) {
				RE_CORE_WARN("Scene save was canceled or failed!");
				return;
			}

			Project::GetCurrentScene()->SetFilePath(sceneFile);
		}


		Project::Save();
	}

	void EditorLayer::NewProject() {
		Project::CreateNewProject();
		m_FileExplorerPanel.SetCurrentDirectory(Project::GetProjectPath());
	}

	void EditorLayer::OpenProject() {
		std::filesystem::path projectFile = FileDialogs::OpenFile("Real Engine Project (*.reproj)\0*.reproj\0");
		
		if (!projectFile.empty()) {
			Project::Load(projectFile);
			m_FileExplorerPanel.SetCurrentDirectory(Project::GetProjectPath());
		}
	}

	void EditorLayer::SaveProject() {
		Project::Save();
	}
}
