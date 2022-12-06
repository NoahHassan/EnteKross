#pragma once

#include "Window.h"
#include "Cube.h"
#include "Sphere.h"
#include "Boundary.h"
#include "ImGuiManager.h"
#include "Camera.h"

#include <vector>
#define PI 3.14159265359f

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
	Camera cam;
	float camSpeed = 1.0f;
	float theta = PI / 2.0f;
	float phi = 0.0f;
	float r	= 5.0f;
	float theta_max = PI - 0.01f;
	float theta_min = 0.01f;
	// USER VARIABLES //
};