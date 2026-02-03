#pragma once
#include "RealEngine.h"

namespace RealEngine {
	class ProjectCreatorLayer : public Layer {
	public:
		ProjectCreatorLayer();
		virtual ~ProjectCreatorLayer() = default;

		virtual void OnImGui() override;
		virtual void OnEvent(Event& e) override;
	};
}
