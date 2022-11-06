#include "Drawable.h"

#include <cassert>

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	pBinds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuf)
{
	assert(pIndexBuffer == nullptr && "Index Buffer already bound");
	pIndexBuffer = std::unique_ptr<IndexBuffer>(std::move(indexBuf));
}

void Drawable::Draw(Graphics& gfx)
{
	assert(pIndexBuffer != nullptr && "No Index Buffer bound");

	for (auto& b : pBinds)
	{
		b->Bind(gfx);
	}

	pIndexBuffer->Bind(gfx);
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}