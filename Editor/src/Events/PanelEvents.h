#include "RealEngine.h"

#include <filesystem>

namespace RealEngine {
	class PanelFolderSelectEvent : public Event {
	public:
		PanelFolderSelectEvent(const std::filesystem::path& path)
			: m_Path(path) {
		}

		inline const std::filesystem::path& GetPath() const { return m_Path; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "PanelFolderSelectEvent: " << m_Path.string();
			return ss.str();
		}

		EVENT_CLASS_TYPE(PannelFolderSelect)
	private:
		const std::filesystem::path m_Path;
	};

	class PannelEntitySelectEvent : public Event {
	public:
		PannelEntitySelectEvent(const Entity entityID)
			: m_Entity(entityID) {
		}

		inline const Entity GetEntity() const { return m_Entity; }

		virtual std::string ToString() const override {
			std::stringstream ss;

			const TagComponent& tag = m_Entity.GetComponent<TagComponent>();
			ss << "PannelEntitySelectEvent: " << tag.Tag;

			return ss.str();
		}

		EVENT_CLASS_TYPE(PannelEntitySelect)
	private:
		Entity m_Entity;
	};
}