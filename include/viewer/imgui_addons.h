#ifndef IMGUI_ADDONS_H
#define IMGUI_ADDONS_H

#include <imgui.h>

namespace ImGui {
	/**
	 * \brief Render a progression bar for _Dear ImGui_.
	 * 
	 * Render a progression bar of specific size and colors, with a value
	 * between 0 and 1.
	 * 
	 * \param label Label of the item.
	 * \param value Current progression (between 0 and 1).
	 * \param size_arg Size of the bar.
	 * \param bg_col Background color.
	 * \param fg_col Foreground/accent color.
	 * \return Successfully rendered (`true`) or not (`false`).
	 */
	bool ProgressBar(const char* label, float value, const ImVec2& size_arg,
			const ImU32& bg_col, const ImU32& fg_col);
	/**
	 * \brief Render a spinner for _Dear ImGui_.
	 * 
	 * Render a spinner of specific size and color. The state of the spinner
	 * depends on the current time.
	 * 
	 * \param label Label of the item.
	 * \param radius Radius of the item.
	 * \param thickness Thickness of the circle.
	 * \param color Accent color.
	 * \return Successfully rendered (`true`) or not (`false`).
	 */
	bool Spinner(const char* label, float radius, int thickness,
			const ImU32& color);
};

#endif // IMGUI_ADDONS_H
