#pragma once

#include <DirectXMath.h>

#include "ConstantBuffers.h"
#include "Drawable.h"

class TransformCBuf : public Bindable
{
public:
	TransformCBuf(Graphics& gfx, const Drawable& parent);
public:
	void Bind(Graphics& gfx) const override;
private:
	struct Transform
	{
		DirectX::XMMATRIX tf;
	};
private:
	std::unique_ptr<VertexConstantBuffer<Transform>> pvcb;
	const Drawable& parent;
};