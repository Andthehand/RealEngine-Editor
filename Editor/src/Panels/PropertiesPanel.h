#pragma once 
#include "RealEngine.h"

#include "Panel.h"

#include <filesystem>

namespace RealEngine {
	/**
	 * @class PropertiesPanel
	 * @brief A UI panel that displays and manages properties of the selected folder.
	 *
	 * This panel allows inspection of directory metadata and displays runtime statistics
	 * such as frame rate. It handles folder selection events and integrates with ImGui.
	 */
	class PropertiesPanel : public Panel {
	public:
		PropertiesPanel() = default;
		~PropertiesPanel() = default;

		virtual void OnImGui() override;
		void OnEvent(Event& event);
	private:
		template<typename T>
		void DisplayComponent(T* component);
		template<typename... Components>
		void DisplayComponents(ComponentList::ComponentGroup<Components...>);

		template<typename... Components>
		void DisplayAddComponentEntries(ComponentList::ComponentGroup<Components...>);

		void ShowFolderProperties();
		void ShowEntityProperties();
	private:
		std::filesystem::path m_SelectedFolder = "";
		Entity m_SelectedEntity;

		enum class CurrentView {
			FolderView,
			EntityView,
			None
		};

		CurrentView m_CurrentView = CurrentView::None;
	};
}