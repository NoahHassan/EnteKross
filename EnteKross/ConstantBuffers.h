#pragma once

#include "Bindable.h"
#include "GraphicsThrowMacros.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics& gfx)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC dc = {};
		dc.ByteWidth = sizeof(C);
		dc.Usage = D3D11_USAGE_DYNAMIC;
		dc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		dc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		dc.MiscFlags = 0u;
		dc.StructureByteStride = 0u;

		ENTE_GFX_CHECK_EXCEPTION(GetDevice(gfx)->CreateBuffer(&dc, 0u, &pBuffer));
	}
	ConstantBuffer(Graphics& gfx, const C& consts)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC dc = {};
		dc.ByteWidth = sizeof(consts);
		dc.Usage = D3D11_USAGE_DYNAMIC;
		dc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		dc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		dc.MiscFlags = 0u;
		dc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &consts;

		ENTE_GFX_CHECK_EXCEPTION(GetDevice(gfx)->CreateBuffer(&dc, &sd, &pBuffer));
	}
public:
	void Update(Graphics& gfx, const C& consts)
	{
		HRESULT hr;

		D3D11_MAPPED_SUBRESOURCE msr;
		ENTE_GFX_CHECK_EXCEPTION(GetContext(gfx)->Map(
			pBuffer.Get(),
			0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr
		));
		std::memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(pBuffer.Get(), 0u);
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	virtual void Bind(Graphics& gfx) const override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	virtual void Bind(Graphics& gfx) const override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pBuffer.GetAddressOf());
	}
};