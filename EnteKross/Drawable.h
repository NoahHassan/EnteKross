#pragma once

#include <DirectXMath.h>
#include <memory>
#include <vector>

#include "Bindable.h"
#include "IndexBuffer.h"

class Drawable
{
public:
	Drawable() = default;
public:
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuf);
public:
	void Draw(Graphics& gfx);
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
private:
	std::vector<std::unique_ptr<Bindable>> pBinds;
	std::unique_ptr<IndexBuffer> pIndexBuffer;
};