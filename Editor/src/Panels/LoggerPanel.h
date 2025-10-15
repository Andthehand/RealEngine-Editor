#pragma once
#include "Panel.h"	

#include "RealEngine.h"

namespace RealEngine {
	class LoggerPanel : public Panel {
	public:
		LoggerPanel();
		virtual ~LoggerPanel() = default;
		virtual void OnImGui() override;

	private:
		uint32_t GetLogIcon(quill::LogLevel level);
	private:
		Ref<Texture2D> m_IconInfo;
		Ref<Texture2D> m_IconWarning;
		Ref<Texture2D> m_IconError;
		Ref<Texture2D> m_IconCritical;

		bool m_AutoScroll = true;
	};
}