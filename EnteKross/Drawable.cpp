#include "Drawable.h"

#include <cassert>

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	pBinds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuf) noexcept(!IS_DEBUG)
{
	assert(pIndexBuffer == nullptr && "Index Buffer already bound");
	pIndexBuffer = indexBuf.get();
	AddBind(std::move(indexBuf));
}

void Drawable::Draw(Graphics& gfx) const
{
	assert(pIndexBuffer != nullptr && "No Index Buffer bound");

	for (auto& b : pBinds)
	{
		b->Bind(gfx);
	}

	pIndexBuffer->Bind(gfx);
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}