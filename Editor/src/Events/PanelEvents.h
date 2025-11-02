#include "RealEngine.h"

#include <filesystem>

namespace RealEngine {
	class PanelFileSelectEvent : public Event {
	public:
		PanelFileSelectEvent(const std::filesystem::path& path)
			: m_Path(path) {
		}

		inline const std::filesystem::path& GetPath() const { return m_Path; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "PanelFileSelectEvent: " << m_Path.string();
			return ss.str();
		}

		EVENT_CLASS_TYPE(PannelFolderSelect)
	private:
		const std::filesystem::path m_Path;
	};

	class PanelEntitySelectEvent : public Event {
	public:
		PanelEntitySelectEvent(const Entity entityID)
			: m_Entity(entityID) {
		}

		inline const Entity GetEntity() const { return m_Entity; }

		virtual std::string ToString() const override {
			std::stringstream ss;

			const TagComponent& tag = m_Entity.GetComponent<TagComponent>();
			ss << "PanelEntitySelectEvent: " << tag.Tag;

			return ss.str();
		}

		EVENT_CLASS_TYPE(PannelEntitySelect)
	private:
		Entity m_Entity;
	};

	class PanelEntityDeselectEvent : public Event {
	public:
		PanelEntityDeselectEvent() = default;
		
		virtual std::string ToString() const override {
			return "PanelEntityDeselectEvent";
		}

		EVENT_CLASS_TYPE(PanelEntityDeselect)
	};
}