#pragma once

#include "Drawable.h"

template<typename T>
class DrawableBase : public Drawable
{
public:
	bool IsStaticallyInitialized() const noexcept
	{
		return !pStaticBinds.empty();
	}
	void AddStaticBind(std::unique_ptr<Bindable> pBind) noexcept(!IS_DEBUG)
	{
		assert("IndexBuffer must be bound using the AddStaticIndexBuffer function" && typeid(*pBind) != typeid(IndexBuffer));
		pStaticBinds.push_back(std::move(pBind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> pBind) noexcept(!IS_DEBUG)
	{
		assert("IndexBuffer already set" && pIndexBuffer == nullptr);
		pIndexBuffer = pBind.get();
		pStaticBinds.push_back(std::move(pBind));
	}
	void SetIndexBufferFromStatic() noexcept(!IS_DEBUG)
	{
		assert("IndexBuffer already set" && pIndexBuffer == nullptr);
		for (const auto& b : pStaticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
		assert("No static IndexBuffer found" && false);
	}
	virtual void Draw(Graphics& gfx) const override
	{
		assert(pIndexBuffer != nullptr && "No Index Buffer bound");

		for (auto& b : pBinds)
		{
			b->Bind(gfx);
		}
		for (auto& b : pStaticBinds)
		{
			b->Bind(gfx);
		}
		pIndexBuffer->Bind(gfx);
		gfx.DrawIndexed(pIndexBuffer->GetCount());
	}
private:
	static std::vector<std::unique_ptr<Bindable>> pStaticBinds;
};

template<typename T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::pStaticBinds;