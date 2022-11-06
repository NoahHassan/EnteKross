#pragma once

#include "Window.h"
#include "Cube.h"
#include "ImGuiManager.h"

#include <vector>

class App
{
public:
	App();
	App(const App&) = delete;
	App& operator =(const App&) = delete;
public:
	void Setup();
	void Update(float dt);
	void Draw();
private:
	ImGuiManager imGuiManager;
	Window wnd;
	Graphics& gfx;
private:
	// USER VARIABLES //
	float c = 0.0f;
	float t = 0.0f;
	std::vector<std::unique_ptr<Cube>> cubes;
	// USER VARIABLES //
};