#ifndef IMGUI_ADDONS_H
#define IMGUI_ADDONS_H

#include <imgui.h>

namespace ImGui {
	bool ProgressBar(const char* label, float value, const ImVec2& size_arg,
			const ImU32& bg_col, const ImU32& fg_col);
	bool Spinner(const char* label, float radius, int thickness,
			const ImU32& color);
};

#endif // IMGUI_ADDONS_H
