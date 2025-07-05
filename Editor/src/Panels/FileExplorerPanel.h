#pragma once
#include <filesystem>

#include "Panel.h"

namespace RealEngine {
	/**
	 * @class FileExplorerPanel
	 * @brief A UI panel for browsing and selecting directories in the filesystem.
	 *
	 * Allows users to navigate through directories, emit folder selection events,
	 * and integrates with ImGui for interactive rendering.
	 */
	class FileExplorerPanel : public Panel {
	public:
		FileExplorerPanel();
		~FileExplorerPanel() = default;

		virtual void OnImGui() override;
	private:
		std::filesystem::path m_CurrentDirectory;
	};
}