#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(HWND hWnd)
{
	// Configure Device and Swap Chain
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0;

	// Create Device and Swap Chain
	UINT swapCreateFlags = 0;
#ifdef _DEBUG
	swapCreateFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		swapCreateFlags,
		NULL, 0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		NULL,
		&pContext
	);

	// Create Render Target View
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	pSwapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf()));
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView);

	// Bind Render Target View
	pContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);

	// Configure Viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = 800.0f;
	vp.Height = 600.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);
}

void Graphics::BeginFrame(float r, float g, float b)
{
	const float color[4] = { r,g,b,1.0f };
	pContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);
	pContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
}

void Graphics::EndFrame()
{
	pSwapChain->Present(1u, 0);
}