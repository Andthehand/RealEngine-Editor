#pragma once

#include "Panel.h"

namespace RealEngine {
	class StatusPanel : public Panel {
		public:
		StatusPanel() = default;
		~StatusPanel() = default;

		virtual void OnImGui() override;
	};
}