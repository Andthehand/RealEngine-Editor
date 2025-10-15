#include "LoggerPanel.h"

#include "RealEngine.h"

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

		m_IconInfo = Texture2D::Create("assets/icons/LoggerPanel/info.png", 4);
		m_IconWarning = Texture2D::Create("assets/icons/LoggerPanel/warn.png", 4);
		m_IconError = Texture2D::Create("assets/icons/LoggerPanel/error.png", 4);
	}

	void LoggerPanel::OnImGui() {
		RE_PROFILE_FUNCTION();

		if (ImGui::Begin("Logger")) {
			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar)) {
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
				
				const auto& logQueue = ImGuiLogSink::GetQueue();

				static ImGuiSelectionBasicStorage selection;
				ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect1d;
				ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(flags, selection.Size, logQueue.Count());
				selection.ApplyRequests(ms_io);


				ImGuiListClipper clipper;
				clipper.Begin(static_cast<int>(logQueue.Count()));
				if (ms_io->RangeSrcItem != -1)
					clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem); // Ensure RangeSrc item is not clipped.

				static float iconSizeMultiplier = 1.0f;
				ImGui::DragFloat("Icon Size", &iconSizeMultiplier, 0.01f, 0.1f, 5.0f);

				while (clipper.Step()) {
					for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
						const auto& it = logQueue.begin() + i;

						ImGui::PushID(i);

						ImGui::PushStyleColor(ImGuiCol_Text, Utils::GetLogColor(it->Severity));
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
						bool item_is_selected = selection.Contains((ImGuiID)i);
						ImGui::SetNextItemSelectionUserData(i);


						float textHeight = ImGui::CalcTextSize(it->Text.c_str(), NULL, true).y;
						ImGui::Image(GetLogIcon(it->Severity), {textHeight * iconSizeMultiplier, textHeight * iconSizeMultiplier}, {0.0f, 1.0f}, {1.0f, 0.0f});
						
						ImGui::SameLine();
						ImGui::Selectable(it->Text.c_str(), item_is_selected, 0, ImVec2(0, textHeight));
						ImGui::PopStyleColor(2);
						ImGui::PopStyleVar();

						ImGui::PopID();
					}
				}

				ms_io = ImGui::EndMultiSelect();
				selection.ApplyRequests(ms_io);

				//Auto-scroll when new log messages are added
				if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
					ImGui::SetScrollHereY(1.0f);

				ImGui::PopStyleVar();
			}
			ImGui::EndChild();
		}

		ImGui::End();
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
			case quill::LogLevel::Critical:
				return m_IconError->GetRendererID();
			case quill::LogLevel::Backtrace:
			case quill::LogLevel::None:
			default:
				RE_CORE_ASSERT(false, "Unknown log level!");
				return m_IconInfo->GetRendererID();
		}
	}
}