#include "App.h"
#include "Geometry.h"
#include "imgui.h"

#include <cmath>
#include <random>
#include <algorithm>


App::App()
	:
	wnd(800, 600, L"Ente Kross"),
	gfx(wnd.Gfx())
{}

void App::Setup()
{
	cam.UpdatePosition(gfx, { 0.0f,0.0f,r });

	boundary = std::make_unique<Boundary>(gfx, 0.0f, 0.0f, 0.0f);
	boundary->SetScale(5.0f, 3.0f, 5.0f);
	InitializeSimulation();
}

void App::Update(float dt)
{
	// DO SIMULATION //
	if (running) {
		for (auto& match : matches)
		{
			match->position.y -= timeStep * matchVelocity * simulationSpeed;
			if (match->position.y < -boundary->scale.y / 2.0f + layerHeight)
			{
				// [matchVolume] mm^2
				// [boundaryScale] cm
				// --> layerHeight multiplied by mm^2 to cm^2 conversion of 0.01f

				layerHeight += filling * matchVolume * 0.01f / boundary->scale.z; // * boundary->scale.x
				ground->SetScale(boundary->scale.x, layerHeight, boundary->scale.z);
				ground->position.posY = (-boundary->scale.y + layerHeight) / 2.0f;
				matches.pop_back();
			}
		}

		if (matches.empty())
			running = false;
	}
	// DO SIMULATION //

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

	// IMGUI //
	if (ImGui::Begin("Simulation"))
	{
		ImGui::Text("Framerate: %.2f ms (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Simulation speed", &simulationSpeed, 1.0f, 100.0f, "%.1f");
		ImGui::SliderInt("n", &nMatches, 1, 100);
		ImGui::SliderFloat3("Boxdimensions", reinterpret_cast<float*>(&boundary->scale), 1.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("velocity", &matchVelocity, 0.1f, 3.0f, "%.2f");
		ImGui::SliderFloat("match radius", &matchRadius, 0.5f, 2.0f, "%.1f");
		ImGui::InputFloat("packing coeff.", &filling);

		if (ImGui::Button("Init"))
		{
			InitializeSimulation();
		}
		if (ImGui::Button("Run"))
		{
			running = true;
		}
	}
	ImGui::End();
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

void App::InitializeSimulation()
{
	layerHeight = 0.0f;
	ground = std::make_unique<Cube>(gfx, 0.0f, (-boundary->scale.y + layerHeight) / 2.0f, 0.0f);
	ground->SetScale(boundary->scale.x, layerHeight, boundary->scale.z);
	ground->SetColor(gfx, 0.7f, 0.5f, 0.5f);

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dstX(-boundary->scale.x / 2.0f, boundary->scale.x / 2.0f);
	std::uniform_real_distribution<float> dstY(-boundary->scale.y / 2.0f, boundary->scale.y / 2.0f);
	std::uniform_real_distribution<float> dstZ(-boundary->scale.z / 2.0f, boundary->scale.z / 2.0f);

	matches = std::vector<std::unique_ptr<Sphere>>(0);
	matchVolume = 4.0f * PI * matchRadius * matchRadius;
	while (std::size(matches) < nMatches)
	{
		float x = dstX(rng);
		float y = dstY(rng);
		float z = dstZ(rng);
		std::unique_ptr<Sphere> s = std::make_unique<Sphere>(gfx, x, y, z);
		s->SetScale(0.25f * matchRadius);
		s->SetColor(gfx, 0.9f, 0.4f, 0.4f);
		matches.push_back(std::move(s));
	}
	std::sort(matches.begin(), matches.end(),
		[](const auto& a, const auto& b)
		{
			return a->position.y > b->position.y;
		}
	);
}