#include "FileExplorerPanel.h"

#include "Events/PanelEvents.h"
#include "RealEngine/Asset/TextureImporter.h"

#include <imgui.h>

namespace RealEngine {

	FileExplorerPanel::FileExplorerPanel() {
		RE_PROFILE_FUNCTION();

		m_FolderIcon = TextureImporter::LoadTexture2D("assets/icons/FileExplorerPanel/folder.png");
		m_FileIcon   = TextureImporter::LoadTexture2D("assets/icons/FileExplorerPanel/document.png");
	}

	void FileExplorerPanel::OnImGui() {
		RE_PROFILE_FUNCTION();

		if (ImGui::Begin("File Explorer")) {
			DrawNavigationBar();

			if(m_ThumbnailsLoaded == false) {
				LoadThumbnails();
				m_ThumbnailsLoaded = true;
			}

			const int columnCount = CalculateColumnCount();
			if (BeginFileTable(columnCount)) {
				DrawDirectoryEntries(columnCount);
				EndFileTable();
			}
		}

		ImGui::End();
	}

	// ---------------- Helper Sections ---------------- //

	void FileExplorerPanel::DrawNavigationBar() {
		if (m_CurrentDirectory != Project::GetProjectPath()) {
			if (ImGui::Button("<-")) {
				SetCurrentDirectory(m_CurrentDirectory.parent_path());
			}
		}
	}

	int FileExplorerPanel::CalculateColumnCount() const {
		float panelWidth = ImGui::GetContentRegionAvail().x;
		float padding = ImGui::GetStyle().CellPadding.x * 2;
		int columnCount = static_cast<int>(panelWidth / (FILE_ICON_SIZE + padding));

		return columnCount < 1 ? 1 : columnCount;
	}

	bool FileExplorerPanel::BeginFileTable(int columnCount) {
		if (ImGui::BeginTable("##FileExplorerTable", columnCount,
			ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_NoClip)) {

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			return true;
		}
		return false;
	}

	void FileExplorerPanel::DrawDirectoryEntries(int columnCount) {
		RE_PROFILE_FUNCTION();

		int i = 0;
		for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			DrawEntry(entry, i, columnCount);
			i++;
		}
	}

	void FileExplorerPanel::DrawEntry(const std::filesystem::directory_entry& entry, int index, int columnCount) {
		const auto& path = entry.path();

		if (index % columnCount == 0)
			ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(index % columnCount);

		uint32_t buttonImage = entry.is_directory()
			? m_FolderIcon->GetRendererID()
			: m_FileIcon->GetRendererID();

		auto thumbnailIt = m_Thumbnails.find(path);
		if(thumbnailIt != m_Thumbnails.end()) {
			buttonImage = thumbnailIt->second->GetRendererID();
		}

		ImGui::PushID(index); // Ensure unique ID even if duplicate filenames
		if (ImGui::ImageButton(path.filename().string().c_str(), buttonImage,
			{ FILE_ICON_SIZE, FILE_ICON_SIZE }, { 0.0f, 1.0f }, { 1.0f, 0.0f })) {
			PanelFileSelectEvent event(path);
			RE_RAISE_EVENT(event);
		}

		// Handle double-click to navigate into directory
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && entry.is_directory()) {
			SetCurrentDirectory(path);
		}

		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + FILE_ICON_SIZE);
		ImGui::TextUnformatted(path.filename().string().c_str());
		ImGui::PopTextWrapPos();

		ImGui::PopID();
	}

	void FileExplorerPanel::EndFileTable() {
		ImGui::PopStyleColor(); // Button color
		ImGui::EndTable();
	}

	void FileExplorerPanel::LoadThumbnails() {
		RE_PROFILE_FUNCTION();

		for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			AssetManager assetManager = Project::GetAssetManager();
			if (entry.is_regular_file()) {
				AssetHandle handle(entry.path());

				if (assetManager.IsAssetValid(handle)) {
					Ref<Texture2D> thumbnail = assetManager.GetAsset<Texture2D>(handle);
					m_Thumbnails[entry.path()] = thumbnail;
				}
			}
		}
	}
}