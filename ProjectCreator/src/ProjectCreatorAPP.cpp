#include <RealEngine/Core/EntryPoint.h>
#include <RealEngine.h>

#include "ProjectCreatorLayer.h"

namespace RealEngine {
	class ProjectCreator : public Application {
	public:
		ProjectCreator(const ApplicationSpecification& specification)
			: Application(specification) {
			PushLayer(new ProjectCreatorLayer);
		}

		~ProjectCreator() = default;
	};

	Application* CreateApplication(const ApplicationCommandLineArgs& args) {
		RE_PROFILE_FUNCTION();

		ApplicationSpecification specification = { "Project Creation", args };
		return new ProjectCreator(specification);
	}
}
