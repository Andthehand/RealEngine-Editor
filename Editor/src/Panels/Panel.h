#pragma once

namespace RealEngine {
	/**
	 * @class Panel
	 * @brief Abstract base class for all editor UI panels.
	 *
	 * Provides a visibility toggle and a pure virtual method for rendering
	 * ImGui content. All concrete panels should inherit from this class.
	 */
	class Panel {
	public:
		Panel() = default;
		virtual ~Panel() = default;

		virtual void OnImGui() = 0;
	public:
		/// Controls whether the panel is visible in the UI.
		bool IsVisible = true;
	};
}