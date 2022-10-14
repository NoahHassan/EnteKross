#pragma once

#include "Window.h"

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
	Window wnd;
	Graphics& gfx;
private:
	// USER VARIABLES //
	float c = 0.0f;
	float t = 0.0f;
	// USER VARIABLES //
};