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
	void InitializeSimulation();
private:
	ImGuiManager imGuiManager;
	Window wnd;
	Graphics& gfx;
private:
	// CAMERA //
	Camera cam;
	float camSpeed = 1.0f;
	float theta = PI / 2.0f;
	float phi = 0.0f;
	float r	= 10.0f;
	float theta_max = PI - 0.01f;
	float theta_min = 0.01f;
	// CAMERA //

	// SIMULATION SETTINGS //
	int nMatches = 50;
	float matchRadius = 1.0f;
	float matchVelocity = 1.6f;
	float matchVolume = 0.00f;
	float filling = 1.1f;

	float timeStep = 1.0f / 60.0f;
	float simulationSpeed = 20.0f;
	// SIMULATION SETTINGS //

	// RUNTIME VARIABLES //
	float layerHeight = 0.0f;
	bool running = false;
	// RUNTIME VARIABLES //

	std::vector<std::unique_ptr<Sphere>> matches;
	std::unique_ptr<Boundary> boundary;
	std::unique_ptr<Cube> ground;
};