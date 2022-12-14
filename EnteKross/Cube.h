#pragma once

#include "Graphics.h"
#include "DrawableBase.h"

class Cube : public DrawableBase<Cube>
{
public:
	Cube(Graphics& gfx, float x, float y, float z);
	Cube(Graphics& gfx);
public:
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
public:
	struct
	{
		float posX = 0.0f;
		float posY = 0.0f;
		float posZ = 0.0f;
	} position;
};