#include "App.h"
#include "Geometry.h"
#include "imgui.h"

#include <cmath>
#include <random>


App::App()
	:
	wnd(800, 600, L"Ente Kross"),
	gfx(wnd.Gfx())
{}

void App::Setup()
{
	cam.UpdatePosition(gfx, { 0.0f,0.0f,5.0f });

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution dstX(-boxX / 2.0f, boxX / 2.0f);
	std::uniform_real_distribution dstY(-boxY / 2.0f, boxY / 2.0f);
	std::uniform_real_distribution dstZ(-boxZ / 2.0f, boxZ / 2.0f);

	while (std::size(matches) < nMatches)
	{
		std::unique_ptr<Sphere> s = std::make_unique<Sphere>(gfx, dstX(rng), dstY(rng), dstZ(rng));
		s->SetScale(0.1f);
		matches.push_back(std::move(s));
	}

	boundary = std::make_unique<Boundary>(gfx, 0.0f, 0.0f, 0.0f);
	boundary->SetScale(boxX, boxY, boxZ);
	ground = std::make_unique<Cube>(gfx, 0.0f, (-boxY + layerHeight) / 2.0f, 0.0f);
	ground->SetScale(boxX, layerHeight, boxZ);
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
	
	matches[0]->SetColor(gfx, 1.0f, 0.0f, 0.0f);

	cam.UpdatePosition(gfx, { r * std::sin(theta) * std::cos(phi),r * std::cos(theta),r * std::sin(theta) * std::sin(phi) });
}

void App::Draw()
{
	gfx.BeginFrame(0.15f, 0.15f, 0.25f);

	// IMGUI //
	// IMGUI //

	// DRAWING //

	for (const auto& s : matches)
	{
		s->Draw(gfx);
	}
	boundary->Draw(gfx);
	ground->Draw(gfx);

	// DRAWING //

	gfx.EndFrame();
}