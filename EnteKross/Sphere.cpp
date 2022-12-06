#include "Sphere.h"

#include "Geometry.h"
#include "BindableBase.h"

Sphere::Sphere(Graphics& gfx, float x, float y, float z)
	:
	position({ x,y,z })
{
	if (!IsStaticallyInitialized())
	{
		Geometry g = Geometry::MakeIcosphere();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, g.vertices));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, g.indices));
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"SolidPS.cso"));

		auto pVs = std::make_unique<VertexShader>(gfx, L"SolidVS.cso");
		auto pByteCode = pVs->GetShaderByteCode();
		AddStaticBind(std::move(pVs));

		AddStaticBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, layout, pByteCode));
	}
	else
	{
		SetIndexBufferFromStatic();
	}

	AddBind(std::make_unique<PixelConstantBuffer<RGBColor>>(gfx, color));
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

Sphere::Sphere(Graphics& gfx)
	:
	Sphere(gfx, 0.0f, 0.0f, 0.0f)
{}

DirectX::XMMATRIX Sphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixScaling(scale, scale, scale) * DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

void Sphere::SetScale(float scale) noexcept
{
	this->scale = scale;
}

void Sphere::SetColor(Graphics& gfx, float r, float g, float b)
{
	color = { r,g,b,1.0f };
	QueryBindable<PixelConstantBuffer<RGBColor>>()->Update(gfx, color);
}