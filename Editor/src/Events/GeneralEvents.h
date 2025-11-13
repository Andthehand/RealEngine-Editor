#include "RealEngine.h"

namespace RealEngine {
	class ProjectChangeEvent : public Event {
	public:
		ProjectChangeEvent() = default;

		virtual std::string ToString() const override {
			return "ProjectChangeEvent";
		}
		EVENT_CLASS_TYPE(ProjectChange)
	};
}
