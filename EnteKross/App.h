#pragma once

#include "Window.h"
#include "Cube.h"
#include "Sphere.h"
#include "Boundary.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Logger.h"

#include <vector>
#include <string>
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
	void InitializeSimulation(int n);
	bool Step();
private:
	ImGuiManager imGuiManager;
	Window wnd;
	Graphics& gfx;
private:
	Logger logger;

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

	float timeStep = 1.0f / 120.0f;
	float simulationSpeed = 20.0f;

	int nMin = 1;
	int nMax = 2;
	int nRep = 10;
	int nCur = 1;
	bool logToFile = false;
	char fileName[20];
	// SIMULATION SETTINGS //

	// RUNTIME VARIABLES //
	float layerHeight = 0.0f;
	bool running = false;
	bool runBatch = false;
	// RUNTIME VARIABLES //

	std::vector<std::unique_ptr<Sphere>> matches;
	std::unique_ptr<Boundary> boundary;
	std::unique_ptr<Cube> ground;
};