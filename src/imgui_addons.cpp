#include "imgui_addons.h"

#include <imgui_internal.h>
#include <cstdlib>

/* From: https://github.com/ocornut/imgui/issues/1901#issue-335266223 */
bool ImGui::ProgressBar(const char* label, float value, const ImVec2& size_arg,
		const ImU32& bg_col, const ImU32& fg_col) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;
	
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = size_arg;
	size.x -= style.FramePadding.x * 2;

	const ImRect bb(pos, ImVec2((pos.x + size.x), (pos.y + size.y)));
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;
	
	window->DrawList->AddRectFilled(bb.Min,
			ImVec2((pos.x + size.x), bb.Max.y), bg_col);
	window->DrawList->AddRectFilled(bb.Min,
			ImVec2((pos.x + (size.x * value)), bb.Max.y), fg_col);

	return true;
}

/* From: https://github.com/ocornut/imgui/issues/1901#issue-335266223 */
bool ImGui::Spinner(const char* label, float radius, int thickness,
		const ImU32& color) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size((radius * 2), ((radius + style.FramePadding.y) * 2));

	const ImRect bb(pos, ImVec2((pos.x + size.x), (pos.y + size.y)));
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	// Render
	window->DrawList->PathClear();

	int num_segments = (int) radius * 8;
	int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

	const float a_min = IM_PI * 2.f *
			((float) start) / ((float) num_segments);
	const float a_max = IM_PI * 2.f *
			((float) num_segments - 3) / ((float) num_segments);

	const ImVec2 center = ImVec2((pos.x + radius),
			(pos.y + radius + style.FramePadding.y));

	for (int i = 0; i < num_segments; i++) {
		const float a = a_min + ((float) i / (float) num_segments)
				* (a_max - a_min);
		window->DrawList->PathLineTo(
				ImVec2(center.x + ImCos(a + (g.Time * 8)) * radius,
						center.y + ImSin(a + (g.Time * 8)) * radius));
	}

	window->DrawList->PathStroke(color, false, thickness);

	return true;
}
