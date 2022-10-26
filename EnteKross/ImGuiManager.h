#pragma once

#include "imgui.h"

class ImGuiManager
{
public:
	ImGuiManager()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
	}
	~ImGuiManager()
	{
		ImGui::DestroyContext();
	}
};