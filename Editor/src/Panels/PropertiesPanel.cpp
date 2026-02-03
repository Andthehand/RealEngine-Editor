#include "PropertiesPanel.h"

#include "Events/PanelEvents.h"

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

		void DrawColorEdit3(const std::string& label, glm::vec3* color, float labelWidth = 110.0f) {
			ImGui::TextUnformatted(label.c_str());

			ImGui::SameLine();
			ImGui::SetCursorPosX(labelWidth);

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::ColorEdit3(("##" + label).c_str(), &color->x);
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

		glm::vec3 position = component->GetTransation();
		if (Utils::DrawVec3Control("Position", &position))
			component->SetTransation(position);

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
	void PropertiesPanel::DisplayComponent<TextRendererComponent>(TextRendererComponent* component) {
		RE_PROFILE_FUNCTION();

		ImGui::TextUnformatted("Text");
		ImGui::InputTextMultiline("##Text", &component->Text, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeight() * 5));

		Utils::DrawColorEdit4("Color", &component->Color);

		if (ImGui::Button("Font")) {
			const char* filter = "Font Files (*.ttf)\0*.ttf\0All Files (*.*)\0*.*\0";
			std::filesystem::path texturePath = FileDialogs::OpenFile(filter);

			if (!texturePath.empty()) {
				AssetHandle handle = Project::GetAssetManager().ImportAssetIfNeeded(texturePath);

				if (handle) {
					component->TextFont = Project::GetAssetManager().GetAsset<Font>(handle);
				}
			}
		}
	}

	template<>
	void PropertiesPanel::DisplayComponent<ScriptComponent>(ScriptComponent* component) {
		RE_PROFILE_FUNCTION();

		if(ImGui::BeginCombo("Scripts: ", Utils::GetStringAfterLastDot(component->ClassName))) {
			for (const auto& scriptClass : m_ScriptClassFieldMap) {
				if (ImGui::Selectable(Utils::GetStringAfterLastDot(scriptClass.first))) {
					component->ClassName = scriptClass.first;
				}
			}

			ImGui::EndCombo();
		}

		// TODO: Remove this early return when scripting fields editing is implemented in runtime
		if (true)
			return;

		std::vector<ScriptField>& fields = m_ScriptClassFieldMap[component->ClassName];
		ScriptInstance* instance = component->Instance.get();
		for (const auto& field : fields) {
			switch (field.Type) {
				case ScriptFieldType::Float: {
					float floatValue = instance->GetFieldValue<float>(field.Name);
					if (ImGui::DragFloat(field.Name.c_str(), &floatValue)) {
						instance->SetFieldValue<float>(field.Name, floatValue);
					}
					break;
				}
				case ScriptFieldType::Double: {
					double doubleValue = instance->GetFieldValue<double>(field.Name);
					if (ImGui::DragScalar(field.Name.c_str(), ImGuiDataType_Double, &doubleValue)) {
						instance->SetFieldValue<double>(field.Name, doubleValue);
					}
					break;
				}
				case ScriptFieldType::Bool: {
					bool boolValue = instance->GetFieldValue<bool>(field.Name);
					if (ImGui::Checkbox(field.Name.c_str(), &boolValue)) {
						instance->SetFieldValue<bool>(field.Name, boolValue);
					}
					break;
				}
				case ScriptFieldType::String: {
					std::string stringValue = instance->GetFieldValue<std::string>(field.Name);
					if (ImGui::InputText(field.Name.c_str(), &stringValue)) {
						instance->SetFieldValue<std::string>(field.Name, stringValue);
					}
					break;
				}
				case ScriptFieldType::Char: {
					char charValue = instance->GetFieldValue<char>(field.Name);
					if (ImGui::InputText(field.Name.c_str(), &charValue, 2)) {
						instance->SetFieldValue<char>(field.Name, charValue);
					}
					break;
				}
				case ScriptFieldType::Byte: {
					int8_t byteValue = instance->GetFieldValue<int8_t>(field.Name);
					if (ImGui::DragScalar(field.Name.c_str(), ImGuiDataType_S8, &byteValue)) {
						instance->SetFieldValue<int8_t>(field.Name, byteValue);
					}
					break;
				}
				case ScriptFieldType::Short: {
					int16_t shortValue = instance->GetFieldValue<int16_t>(field.Name);
					if (ImGui::DragScalar(field.Name.c_str(), ImGuiDataType_S16, &shortValue)) {
						instance->SetFieldValue<int16_t>(field.Name, shortValue);
					}
					break;
				}
				case ScriptFieldType::Int: {
					int32_t intValue = instance->GetFieldValue<int32_t>(field.Name);
					if (ImGui::DragScalar(field.Name.c_str(), ImGuiDataType_S32, &intValue)) {
						instance->SetFieldValue<int32_t>(field.Name, intValue);
					}
					break;
				}
				case ScriptFieldType::Long: {
					int64_t longValue = instance->GetFieldValue<int64_t>(field.Name);
					if (ImGui::DragScalar(field.Name.c_str(), ImGuiDataType_S64, &longValue)) {
						instance->SetFieldValue<int64_t>(field.Name, longValue);
					}
					break;
				}
				case ScriptFieldType::UByte: {
					uint8_t ubyteValue = instance->GetFieldValue<uint8_t>(field.Name);
					if (ImGui::DragScalar(field.Name.c_str(), ImGuiDataType_U8, &ubyteValue)) {
						instance->SetFieldValue<uint8_t>(field.Name, ubyteValue);
					}
					break;
				}
				case ScriptFieldType::UShort: {
					uint16_t ushortValue = instance->GetFieldValue<uint16_t>(field.Name);
					if (ImGui::DragScalar(field.Name.c_str(), ImGuiDataType_U16, &ushortValue)) {
						instance->SetFieldValue<uint16_t>(field.Name, ushortValue);
					}
					break;
				}
				case ScriptFieldType::UInt: {
					uint32_t uintValue = instance->GetFieldValue<uint32_t>(field.Name);
					if (ImGui::DragScalar(field.Name.c_str(), ImGuiDataType_U32, &uintValue)) {
						instance->SetFieldValue<uint32_t>(field.Name, uintValue);
					}
					break;
				}
				case ScriptFieldType::ULong: {
					uint64_t ulongValue = instance->GetFieldValue<uint64_t>(field.Name);
					if (ImGui::DragScalar(field.Name.c_str(), ImGuiDataType_U64, &ulongValue)) {
						instance->SetFieldValue<uint64_t>(field.Name, ulongValue);
					}
					break;
				}
				case ScriptFieldType::Vector2: {
					glm::vec2 vec2Value = instance->GetFieldValue<glm::vec2>(field.Name);
					if (ImGui::DragFloat2(field.Name.c_str(), &vec2Value.x)) {
						instance->SetFieldValue<glm::vec2>(field.Name, vec2Value);
					}
					break;
				}
				case ScriptFieldType::Vector3: {
					glm::vec3 vec3Value = instance->GetFieldValue<glm::vec3>(field.Name);
					if (ImGui::DragFloat3(field.Name.c_str(), &vec3Value.x)) {
						instance->SetFieldValue<glm::vec3>(field.Name, vec3Value);
					}
					break;
				}
				case ScriptFieldType::Vector4: {
					glm::vec4 vec4Value = instance->GetFieldValue<glm::vec4>(field.Name);
					if (ImGui::DragFloat4(field.Name.c_str(), &vec4Value.x)) {
						instance->SetFieldValue<glm::vec4>(field.Name, vec4Value);
					}
					break;
				}
				case ScriptFieldType::Entity: {
					break;
				}
				case ScriptFieldType::Unknown: {
					break;
				}
			};
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
	}

	template<typename T>
	bool PropertiesPanel::DeselectEntityEvent(T& e) {
		m_SelectedEntity = {};

		if (m_CurrentView == CurrentView::EntityView)
			m_CurrentView = CurrentView::None;

		return false;
	}
}