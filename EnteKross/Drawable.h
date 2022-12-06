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
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuf) noexcept(!IS_DEBUG);
public:
	virtual void Draw(Graphics& gfx) const;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
protected:
	std::vector<std::unique_ptr<Bindable>> pBinds;
	const IndexBuffer* pIndexBuffer = nullptr;
};