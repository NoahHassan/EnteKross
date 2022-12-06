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
	void SetScale(float sx, float sy, float sz) noexcept;
	void SetColor(Graphics& gfx, float r, float g, float b);
private:
	struct RGBColor
	{
		float r;
		float g;
		float b;
		float a;
	};
private:
	RGBColor color = { 1.0f,1.0f,1.0f,1.0f };
	struct
	{
		float x = 1.0f;
		float y = 1.0f;
		float z = 1.0f;
	} scale;
public:
	struct
	{
		float posX = 0.0f;
		float posY = 0.0f;
		float posZ = 0.0f;
	} position;
};