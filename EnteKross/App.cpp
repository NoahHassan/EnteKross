#include "App.h"
#include "Geometry.h"
#include "imgui.h"

#include <cmath>


App::App()
	:
	wnd(800, 600, L"Ente Kross"),
	gfx(wnd.Gfx())
{}

void App::Setup()
{
	cam.UpdatePosition(gfx, { 0.0f,0.0f,5.0f });
}

void App::Update(float dt)
{
	// Keyboard Control
	if (wnd.keyboard.KeyIsPressed('D'))
	{
		phi -= camSpeed * dt;
	}
	else if (wnd.keyboard.KeyIsPressed('A'))
	{
		phi += camSpeed * dt;
	}

	if (wnd.keyboard.KeyIsPressed('W') && theta > theta_min)
	{
		theta -= camSpeed * dt;
	}
	else if (wnd.keyboard.KeyIsPressed('S') && theta < theta_max)
	{
		theta += camSpeed * dt;
	}
	// Keyboard Control

	cam.UpdatePosition(gfx, { r * std::sin(theta) * std::cos(phi),r * std::cos(theta),r * std::sin(theta) * std::sin(phi) });
}

void App::Draw()
{
	gfx.BeginFrame(0.15f, 0.15f, 0.25f);

	// DRAW CODE HERE //

	// IMGUI //
	// IMGUI //

	// DRAW CODE HERE //

	gfx.EndFrame();
}