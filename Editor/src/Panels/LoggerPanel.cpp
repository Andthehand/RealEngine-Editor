#include "LoggerPanel.h"

#include "RealEngine.h"

#include "RealEngine/Asset/TextureImporter.h"

#include <imgui.h>

namespace RealEngine {
	namespace Utils {
		static ImVec4 GetLogColor(quill::LogLevel level) {
			switch (level) {
				case quill::LogLevel::TraceL3:
				case quill::LogLevel::TraceL2:
				case quill::LogLevel::TraceL1:
					return ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Gray
				case quill::LogLevel::Debug:
					return ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
				case quill::LogLevel::Info:
					return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
				case quill::LogLevel::Notice:
					return ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
				case quill::LogLevel::Warning:
					return ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
				case quill::LogLevel::Error:
					return ImVec4(1.0f, 0.5f, 0.0f, 1.0f); // Orange
				case quill::LogLevel::Critical:
					return ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
				case quill::LogLevel::Backtrace:
					return ImVec4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta
				case quill::LogLevel::None:
				default:
					RE_CORE_ASSERT(false, "Unknown log level!");
					return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default to white
			}
		}
	}

	LoggerPanel::LoggerPanel() {
		RE_PROFILE_FUNCTION();

        m_IconInfo = TextureImporter::LoadTexture2D("assets/icons/LoggerPanel/info.png", { 4 });
        m_IconWarning = TextureImporter::LoadTexture2D("assets/icons/LoggerPanel/warn.png", { 4 });
        m_IconError = TextureImporter::LoadTexture2D("assets/icons/LoggerPanel/error.png", { 4 });
        m_IconCritical = TextureImporter::LoadTexture2D("assets/icons/LoggerPanel/critical.png", { 4 });
	}

    void LoggerPanel::OnImGui() {
        RE_PROFILE_FUNCTION();

        // Window padding zero to use full area
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        if (ImGui::Begin("Logger", nullptr, ImGuiWindowFlags_MenuBar)) {
            const auto& logQueue = ImGuiLogSink::GetQueue();

            // -------- Menu bar --------
            if (ImGui::BeginMenuBar()) {
                const float initialRegionAvailable = ImGui::GetContentRegionAvail().x;

                if (ImGui::MenuItem("Clear")) {
                    ImGuiLogSink::GetQueue().Clear();
                }

                if (ImGui::BeginMenu("Options")) {
                    ImGui::MenuItem("Auto-scroll", nullptr, &m_AutoScroll);
                    ImGui::EndMenu();
                }

                const std::string countLabel = std::to_string(logQueue.Count()) + " / " + std::to_string(logQueue.Size());
                const float xOffset = ImGui::CalcTextSize(countLabel.c_str()).x;
                ImGui::SetCursorPosX(initialRegionAvailable - xOffset);
                ImGui::TextUnformatted(countLabel.c_str());

                ImGui::EndMenuBar();
            }

            // -------- Scrolling table --------
            constexpr ImGuiTableFlags tableFlags =
                ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
                ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter;

            if (ImGui::BeginTable("ScrollingRegion", 2, tableFlags)) {
                static ImGuiSelectionBasicStorage selection;
                const ImGuiMultiSelectFlags msFlags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect1d;
                ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(msFlags, selection.Size, logQueue.Count());
                selection.ApplyRequests(ms_io);

                // Use clipper for large queues
                ImGuiListClipper clipper;
                clipper.Begin(static_cast<int>(logQueue.Count()));
                if (ms_io->RangeSrcItem != -1) {
                    clipper.IncludeItemByIndex(static_cast<int>(ms_io->RangeSrcItem)); // Ensure RangeSrc item is not clipped.
                }

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });
                while (clipper.Step()) {
                    for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                        const auto& entry = logQueue.begin() + i; // random-access by index

                        ImGui::PushID(i);
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        // Color & icon
                        ImGui::PushStyleColor(ImGuiCol_Text, Utils::GetLogColor(entry->Severity));
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

                        const bool item_is_selected = selection.Contains(static_cast<ImGuiID>(i));
                        ImGui::SetNextItemSelectionUserData(i);

                        const float textHeight = ImGui::CalcTextSize(entry->Text.c_str(), nullptr, true).y;
                        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(GetLogIcon(entry->Severity))),
                            ImVec2{ textHeight, textHeight },
                            ImVec2{ 0.0f, 1.0f },
                            ImVec2{ 1.0f, 0.0f });

                        ImGui::SameLine();
                        ImGui::TableNextColumn();
                        ImGui::Selectable(entry->Text.c_str(),
                            item_is_selected,
                            ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap,
                            ImVec2{ 0.0f, textHeight });

                        ImGui::PopStyleColor(2); // ImGuiCol_Text ImGuiCol_Button
                        ImGui::PopID();
                    }
                }
                ImGui::PopStyleVar(); // ImGuiStyleVar_FramePadding

                // For clipping
                ms_io = ImGui::EndMultiSelect();
                selection.ApplyRequests(ms_io);

                // Auto-scroll when at bottom and new messages appear
                if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                    ImGui::SetScrollHereY(1.0f);
                }

                ImGui::EndTable();
            }
        }

        ImGui::End();
        ImGui::PopStyleVar(); // ImGuiStyleVar_WindowPadding
    }

	uint32_t LoggerPanel::GetLogIcon(quill::LogLevel level) {
		switch (level) {
			case quill::LogLevel::TraceL3:
			case quill::LogLevel::TraceL2:
			case quill::LogLevel::TraceL1:
			case quill::LogLevel::Debug:
			case quill::LogLevel::Info:
			case quill::LogLevel::Notice:
				return m_IconInfo->GetRendererID();
			case quill::LogLevel::Warning:
				return m_IconWarning->GetRendererID();
			case quill::LogLevel::Error:
				return m_IconError->GetRendererID();
			case quill::LogLevel::Critical:
				return m_IconCritical->GetRendererID();
			case quill::LogLevel::Backtrace:
			case quill::LogLevel::None:
			default:
				RE_CORE_ASSERT(false, "Unknown log level!");
				return m_IconInfo->GetRendererID();
		}
	}
}