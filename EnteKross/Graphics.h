#pragma once

#include "WindowsKross.h"
#include "KrossException.h"
#include <d3d11.h>
#include <wrl.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator =(const Graphics&) = delete;
public:
	void BeginFrame(float r, float g, float b);
	void EndFrame();
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pRenderTargetView;
};