#pragma once

#include "Graphics.h"
#include <DirectXMath.h>

class Camera
{
public:
	Camera() = default;
	Camera(DirectX::XMFLOAT3 position) noexcept
		:
		position(position)
	{}
	Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 focus) noexcept
		:
		position(position),
		focus(focus)
	{}
public:
	void UpdatePosition(Graphics& gfx, DirectX::XMFLOAT3 position) noexcept
	{
		this->position = position;
		ApplyCameraXM(gfx);
	}
	void UpdatePosition(Graphics& gfx, float r, float theta, float phi) noexcept
	{

	}
	void UpdateFocus(Graphics& gfx, DirectX::XMFLOAT3 focus) noexcept
	{
		this->focus = focus;
		ApplyCameraXM(gfx);
	}
private:
	void ApplyCameraXM(Graphics& gfx) const noexcept
	{
		gfx.SetCameraXM(
			DirectX::XMMatrixLookAtRH(
				DirectX::XMLoadFloat3(&position),
				DirectX::XMLoadFloat3(&focus),
				DirectX::XMVECTOR{ 0.0f,1.0f,0.0f,0.0f }
			)
		);
	}
private:
	DirectX::XMFLOAT3 position	= { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 focus		= { 0.0f,0.0f,0.0f };
};