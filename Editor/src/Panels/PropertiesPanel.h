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
		PropertiesPanel();
		~PropertiesPanel() = default;

		void SelectFolder(const std::filesystem::path& path);

		virtual void OnImGui() override;
		void OnEvent(Event& event);
	private:
		std::filesystem::path m_SelectedFolder = "";
	};
}