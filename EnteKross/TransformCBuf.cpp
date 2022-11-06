#include "TransformCBuf.h"

TransformCBuf::TransformCBuf(Graphics& gfx, const Drawable& parent)
	:
	parent(parent)
{
	pvcb = std::make_unique<VertexConstantBuffer<Transform>>(gfx);
}

void TransformCBuf::Bind(Graphics& gfx) const
{
	DirectX::XMMATRIX modelTransform = parent.GetTransformXM();

	Transform tf =
	{
		DirectX::XMMatrixTranspose(
			modelTransform * gfx.GetProjectionXM()
		)
	};

	pvcb->Update(gfx, tf);
	pvcb->Bind(gfx);
}