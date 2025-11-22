#include "PropertiesPanel.h"

#include "Events/PanelEvents.h"
#include "Events/GeneralEvents.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_internal.h>

namespace RealEngine {	
	namespace Utils {
		bool DrawVec3Control(const std::string& label, glm::vec3* values, float labelWidth = 110.0f) {
			ImGui::TextUnformatted(label.c_str());

			ImGui::SameLine();
			ImGui::SetCursorPosX(labelWidth);

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			return ImGui::DragFloat3(("##" + label).c_str(), &values->x, 0.1f, -100.0f, 100.0f);
		}

		void DrawVec4Control(const std::string& label, glm::vec4* values, float labelWidth = 110.0f) {
			ImGui::TextUnformatted(label.c_str());

			ImGui::SameLine();
			ImGui::SetCursorPosX(labelWidth);

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::DragFloat4(("##" + label).c_str(), &values->x, 0.1f, -100.0f, 100.0f);
		}

		void DrawColorEdit4(const std::string& label, glm::vec4* color, float labelWidth = 110.0f) {
			ImGui::TextUnformatted(label.c_str());
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(labelWidth);

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::ColorEdit4(("##" + label).c_str(), &color->x);
		}

		int OpenGLFilterToArrayIndex(TextureFilterMode filter) {
			switch(filter) {
				case TextureFilterMode::NEAREST: return 0;
				case TextureFilterMode::BILINEAR: return 1;
				case TextureFilterMode::TRILINEAR: return 2;
				default: 
					RE_CORE_ASSERT(false, "Unknown TextureFilterMode: {0}", static_cast<GLenum>(filter));
					return 0; // Default to Nearest if unknown
			}
		}

		TextureFilterMode ArrayIndexToOpenGLFilter(int index) {
			switch(index) {
				case 0: return TextureFilterMode::NEAREST;
				case 1: return TextureFilterMode::BILINEAR;
				case 2: return TextureFilterMode::TRILINEAR;
				default: 
					RE_CORE_ASSERT(false, "Unknown TextureFilterMode index: {0}", index);
					return TextureFilterMode::NEAREST; // Default to Nearest if unknown
			}
		}

		TextureWrapMode ArrayIndexToOpenGLWrap(int index) {
			switch(index) {
				case 0: return TextureWrapMode::REPEAT;
				case 1: return TextureWrapMode::MIRRORED_REPEAT;
				case 2: return TextureWrapMode::CLAMP_TO_EDGE;
				case 3: return TextureWrapMode::CLAMP_TO_BORDER;
				default: 
					RE_CORE_ASSERT(false, "Unknown TextureWrapMode index: {0}", index);
					return TextureWrapMode::REPEAT; // Default to Repeat if unknown
			}
		}

		int OpenGLWrapToArrayIndex(TextureWrapMode wrap) {
			switch(wrap) {
				case TextureWrapMode::REPEAT: return 0;
				case TextureWrapMode::MIRRORED_REPEAT: return 1;
				case TextureWrapMode::CLAMP_TO_EDGE: return 2;
				case TextureWrapMode::CLAMP_TO_BORDER: return 3;
				default: 
					RE_CORE_ASSERT(false, "Unknown TextureWrapMode: {0}", static_cast<GLenum>(wrap));
					return 0; // Default to Repeat if unknown
			}
		}

		// Utility function to get substring after the last '.'
		const char* GetStringAfterLastDot(const std::string_view input) {
			size_t pos = input.rfind('.');
			if (pos == std::string::npos || pos + 1 >= input.size())
				return input.data();

			return input.data() + pos + 1;
		}
	}

	template<typename T>
	void PropertiesPanel::DisplayComponent(T* Component) {
		RE_CORE_ASSERT(false, "No display function for component type: {0}", T::GetName());
	}

	template<>
	void PropertiesPanel::DisplayComponent<TransformComponent>(TransformComponent* component) {
		RE_PROFILE_FUNCTION();

		glm::vec3 position = component->GetPosition();
		if (Utils::DrawVec3Control("Position", &position))
			component->SetPosition(position);

		glm::vec3 eulerRotation = component->GetRotationEuler();
		if (Utils::DrawVec3Control("Rotation", &eulerRotation))
			component->SetRotationEuler(eulerRotation);

		glm::vec3 scale = component->GetScale();
		if (Utils::DrawVec3Control("Scale", &scale))
			component->SetScale(scale);
	}

	template<>
	void PropertiesPanel::DisplayComponent<SpriteRendererComponent>(SpriteRendererComponent* component) {
		RE_PROFILE_FUNCTION();

		Utils::DrawColorEdit4("Color", &component->Color);
		if (ImGui::Button("Texture")) {
			const char* filter = "Image Files (*.png;*.jpg;*.jpeg)\0*.png;*.jpg;*.jpeg;*.bmp;*.tga\0All Files (*.*)\0*.*\0";
			std::filesystem::path texturePath = FileDialogs::OpenFile(filter);

			if (!texturePath.empty()) {
				AssetHandle handle = Project::GetAssetManager().ImportAssetIfNeeded(texturePath);

				if (handle) {
					component->Texture = Project::GetAssetManager().GetAsset<Texture2D>(handle);
				}
			}
		}
	}

	template<>
	void PropertiesPanel::DisplayComponent<ScriptComponent>(ScriptComponent* component) {
		RE_PROFILE_FUNCTION();

		if(ImGui::BeginCombo("Scripts: ", Utils::GetStringAfterLastDot(component->ClassName))) {
			for (const std::string& scriptClass : m_ValidScriptClasses) {
				if (ImGui::Selectable(Utils::GetStringAfterLastDot(scriptClass))) {
					component->ClassName = scriptClass;
				}
			}

			ImGui::EndCombo();
		}
	}


	template<typename... Components>
	void PropertiesPanel::DisplayComponents(ComponentList::ComponentGroup<Components...>) {
		RE_PROFILE_FUNCTION();

		// Fold expression over comma operator to handle each component
		(([this]() {
			if(m_SelectedEntity.HasComponent<Components>() == false)
				return;

			// Has to be deferred until later so OpenPopupOnItemClick works correctly
			bool isOpen = ImGui::CollapsingHeader(Components::GetName(), ImGuiTreeNodeFlags_DefaultOpen);
			
			// Right-click context menu to remove component
			ImGui::OpenPopupOnItemClick(Components::GetName(), ImGuiPopupFlags_MouseButtonRight);
			if (ImGui::BeginPopupContextItem(Components::GetName())) {
				if (ImGui::Selectable("Remove component")) {
					m_SelectedEntity.RemoveComponent<Components>();
				}

				ImGui::EndPopup();
			}

			// Display component details if the header is open
			if (isOpen) {
				if (auto* component = m_SelectedEntity.TryGetComponent<Components>()) {
					DisplayComponent(component);
				}
			}

		}()), ...);
	}

	template<typename ...Components>
	void PropertiesPanel::DisplayAddComponentEntries(ComponentList::ComponentGroup<Components...>) {
		(([this]() {
			if(m_SelectedEntity.HasComponent<Components>())
				return;

			if (ImGui::Selectable(Components::GetName())) {
				m_SelectedEntity.AddComponent<Components>();
			}
		}()), ...);
	}

	void PropertiesPanel::ShowFileProperties() {
		RE_PROFILE_FUNCTION();

		AssetHandle handle(m_SelectedFile);
		bool isAsset = Project::GetAssetManager().IsAssetValid(handle);

		ImGui::Text("File Properties: %s", m_SelectedFile.filename().string().c_str());
		ImGui::Separator();
		if (isAsset) {
			AssetMetadata* metadata = Project::GetAssetManager().GetAssetMetadata(handle);
			if (Texture2DMetadata* texture2DMetadata = std::any_cast<Texture2DMetadata>(&metadata->CustomMetadata)) {
				int mipLevels = texture2DMetadata->MipLevels;
				if (ImGui::DragInt("Mip Levels", &mipLevels, 1, 1, 16)) {
					texture2DMetadata->MipLevels = mipLevels;
				}

				// Filter Mode
				const char* textureFilterOptions[] = { "Nearest", "Bilinear", "Trilinear"};
				int currentFilterIndex = Utils::OpenGLFilterToArrayIndex(texture2DMetadata->FilterMode);
				if (ImGui::Combo("Filter Mode", &currentFilterIndex, textureFilterOptions, IM_ARRAYSIZE(textureFilterOptions))) {
					texture2DMetadata->FilterMode = Utils::ArrayIndexToOpenGLFilter(currentFilterIndex);
				}

				// Wrap Mode
				const char* textureWrapOptions[] = { "Repeat", "Mirrored Repeat", "Clamp to Edge", "Clamp to Border" };
				int currentWrapIndex = Utils::OpenGLWrapToArrayIndex(texture2DMetadata->WrapMode);
				if (ImGui::Combo("Wrap Mode", &currentWrapIndex, textureWrapOptions, IM_ARRAYSIZE(textureWrapOptions))) {
					texture2DMetadata->WrapMode = Utils::ArrayIndexToOpenGLWrap(currentWrapIndex);
				}
			}
		} else {
			ImGui::TextUnformatted("Not an imported asset.");
		}
	}

	void PropertiesPanel::ShowEntityProperties() {
		RE_PROFILE_FUNCTION();

		m_SelectedEntity.GetComponent<TagComponent>();
		const char* addComponentBtnLabel = "+";

		const float framePaddingX = ImGui::GetStyle().FramePadding.x * 2.0f;
		const float addComponentBtnWidth = ImGui::CalcTextSize(addComponentBtnLabel).x + framePaddingX;
		const float availableWidth = ImGui::GetContentRegionAvail().x;

		ImGui::PushItemWidth(availableWidth - addComponentBtnWidth - framePaddingX);
		ImGui::InputText("##Tag", &m_SelectedEntity.GetComponent<TagComponent>().Tag);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushFont(boldFont);
		if (ImGui::Button(addComponentBtnLabel, ImVec2(addComponentBtnWidth, 0.0f))) {
			ImGui::OpenPopup("AddComponent");
		}
		ImGui::PopFont(); // Bold font for the button

		if (ImGui::BeginPopup("AddComponent")) {
			DisplayAddComponentEntries(ComponentList::GetAllComponents());

			ImGui::EndPopup();
		}

		DisplayComponents(ComponentList::GetAllComponents());
	}
	
	void PropertiesPanel::OnImGui() {
		RE_PROFILE_FUNCTION();

		if (ImGui::Begin("Properties")) {
			switch (m_CurrentView) {
				case CurrentView::FileView:
					ShowFileProperties();
					break;
				case CurrentView::EntityView:
					ShowEntityProperties();
					break;
				case CurrentView::None:
					ImGui::TextUnformatted("No selection");
					break;
			}
		}

		ImGui::End();
	}

	void PropertiesPanel::OnEvent(Event& event) {
		RE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<PanelFileSelectEvent>([this](PanelFileSelectEvent& e) {
			if (std::filesystem::is_regular_file(e.GetPath())) {
				m_SelectedFile = e.GetPath();
				m_CurrentView = CurrentView::FileView;
			}

			// Handled because this should be the only panel responding to this event
			return true;
		});

		dispatcher.Dispatch<PanelEntitySelectEvent>([this](PanelEntitySelectEvent& e) {
			m_SelectedEntity = e.GetEntity();
			m_CurrentView = CurrentView::EntityView;

			// Handled because this should be the only panel responding to this event
			return true;
		});

		dispatcher.Dispatch<SceneChangedEvent>(RE_BIND_EVENT_FN(PropertiesPanel::DeselectEntityEvent));
		dispatcher.Dispatch<PanelEntityDeselectEvent>(RE_BIND_EVENT_FN(PropertiesPanel::DeselectEntityEvent));

		dispatcher.Dispatch<ProjectChangeEvent>([this](ProjectChangeEvent& e) {
			m_ValidScriptClasses = Project::GetScriptEngine()->GetValidScriptClasses();

			return false;
		});
	}

	template<typename T>
	bool PropertiesPanel::DeselectEntityEvent(T& e) {
		m_SelectedEntity = {};

		if (m_CurrentView == CurrentView::EntityView)
			m_CurrentView = CurrentView::None;

		return false;
	}
}