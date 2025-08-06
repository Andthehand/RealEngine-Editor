#include <RealEngine/Core/EntryPoint.h>
#include <RealEngine.h>

#include "EditorLayer.h"

namespace RealEngine {
	class Editor : public Application {
	public:
		Editor(const ApplicationSpecification& specification)
			: Application(specification) {
			PushLayer(new EditorLayer());
		}

		~Editor() = default;
	};

	Application* CreateApplication(const ApplicationCommandLineArgs& args) {
		RE_PROFILE_FUNCTION();

		RE_CORE_WARN("Current Working Directory: {0}", std::filesystem::current_path());

		ApplicationSpecification specification = { "Editor", args };
		return new Editor(specification);
	}
}
