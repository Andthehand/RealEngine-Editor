#include "EditorLayer.h"

#include "Events/PanelEvents.h"

#define IMGUI_DEFINE_MATH_OPERATORS
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

		//TODO: Refactor
		LoadProject();
		m_FileExplorerPanel.SetCurrentDirectory(Project::GetAssetsPath());

		m_PlayIcon = TextureImporter::LoadTexture2D("assets/icons/EditorLayer/play.png");
		m_PauseIcon = TextureImporter::LoadTexture2D("assets/icons/EditorLayer/pause.png");
	}

	void EditorLayer::OnDetach() {
		RE_PROFILE_FUNCTION();

		Project::Delete();
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
				//TODO: Remove this camera in runtime mode and use a camera from the scene
				m_EditorCamera.OnUpdate(deltaTime);

				Project::GetCurrentScene()->OnUpdateRuntime(deltaTime, m_EditorCamera);
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
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
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
		if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoDecoration)) {
			m_ViewportFocused = ImGui::IsWindowFocused();

			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();

			ImVec2 viewpoerPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewpoerPanelSize.x, viewpoerPanelSize.y };

			uint64_t textureID = m_Framebuffer->GetAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			// Centered Play Button
			float size = ICON_SIZE + ImGui::GetStyle().FramePadding.x * 2.0f;
			float avail = ImGui::GetContentRegionAvail().x;

			float off = (avail - size) * 0.5f;
			if (off > 0.0f)
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + off, ICON_OFFSET_FROM_TOP));

			ImTextureRef currentIcon = m_SceneState == SceneState::Edit ? m_PlayIcon->GetRendererID() : m_PauseIcon->GetRendererID();
			if (ImGui::ImageButton("Icon", currentIcon, ImVec2(ICON_SIZE, ICON_SIZE))) {
				// Switch to Play mode
				switch (m_SceneState) {
					case SceneState::Edit:
						m_SceneState = SceneState::Play;

						// Copy the scene so the runtime sceen doesn't screw with the editor scene
						m_SwapScene = Project::GetCurrentScene();
						Project::SetCurrentScene(Scene::Copy(m_SwapScene));
						Project::GetCurrentScene()->OnRuntimeStart();
						break;
					case SceneState::Play:
						m_SceneState = SceneState::Edit;
						Project::GetCurrentScene()->OnRuntimeStop();

						Project::SetCurrentScene(m_SwapScene);
						m_SwapScene = nullptr;
						break;
					default:
						RE_CORE_ASSERT(false, "Unknown scene state!");
						break;
				};
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();

		EndDockspace();

		ImGui::ShowDemoWindow();
	}

	void EditorLayer::OnEvent(Event& event) {
		RE_PROFILE_FUNCTION();

		if(m_ViewportFocused)
			m_EditorCamera.OnEvent(event);
		m_PropertiesPanel.OnEvent(event);
	}

	void EditorLayer::LoadProject() {
		bool projectLoaded = false;

		const ApplicationCommandLineArgs& args = Application::Get().GetCommandLineArgs();
		if (args.Count > 1) {
			std::filesystem::path projectPath = Application::Get().GetCommandLineArgs()[1];
			projectLoaded = Project::Load(projectPath);
		}

		while (!projectLoaded) {
			std::filesystem::path path = FileDialogs::OpenFile({
				{.name = "Real Engine Project", .spec = "reproj"}
			});

			if (!path.empty()) {
				projectLoaded = Project::Load(path);
				break;
			}
		}
	}

	void EditorLayer::StartDockspace() {
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
		RE_PROFILE_FUNCTION();

		if (ImGui::Shortcut(ImGuiKey_N | ImGuiMod_Ctrl, ImGuiInputFlags_RouteGlobal)) {
			NewScene();
		}
		if (ImGui::Shortcut(ImGuiKey_O | ImGuiMod_Ctrl, ImGuiInputFlags_RouteGlobal)) {
			OpenScene();
		}
		if (ImGui::Shortcut(ImGuiKey_S | ImGuiMod_Ctrl, ImGuiInputFlags_RouteGlobal)) {
			SaveProject();
		}
	}

	void EditorLayer::NewScene() {
		RE_PROFILE_FUNCTION();

		Project::GetCurrentScene()->Save();

		Ref<Scene> newScene = CreateRef<Scene>();
		while (newScene->GetFilePath().empty()) {
			std::filesystem::path sceneFile = FileDialogs::SaveFile({
				{.name = "Real Engine Scene", .spec = "rescene" }
			});

			if (!sceneFile.empty()) {
				newScene->SetFilePath(sceneFile);
			}
		}

		Project::SetCurrentScene(CreateRef<Scene>());
	}

	void EditorLayer::OpenScene() {
		RE_PROFILE_FUNCTION();

		std::filesystem::path sceneFile = FileDialogs::OpenFile({
			{.name = "Real Engine Scene", .spec = "rescene" }
		});
		
		if (!sceneFile.empty()) {
			Project::GetCurrentScene()->Save();
			Project::SetCurrentScene(CreateRef<Scene>(sceneFile));
		} else {
			RE_CORE_WARN("Scene open was canceled or failed!");
		}
	}

	void EditorLayer::SaveProject() {
		RE_PROFILE_FUNCTION();

		Project::Save();
	}
}
