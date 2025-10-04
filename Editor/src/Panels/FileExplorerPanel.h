#pragma once
#include <filesystem>

#include "Panel.h"
#include "RealEngine.h"

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

		void SetCurrentDirectory(const std::filesystem::path& path) { m_CurrentDirectory = path; }

		virtual void OnImGui() override;
	private:
		// Rendering steps broken out from OnImGui for clarity & testability
		void DrawNavigationBar();
		int  CalculateColumnCount() const;
		bool BeginFileTable(int columnCount);
		void DrawDirectoryEntries(int columnCount);
		void DrawEntry(const std::filesystem::directory_entry& entry, int index, int columnCount);
		void EndFileTable();

	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;

		static constexpr float FILE_ICON_SIZE = 96.0f;
	};
}