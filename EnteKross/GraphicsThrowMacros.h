#pragma once

#define ENTE_GFX_CHECK_EXCEPTION(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr)
#define ENTE_GFX_THROW_EXCEPTION(hr) throw Graphics::HrException(__LINE__, __FILE__, hr)
#define ENTE_GFX_THROW_DEVICE_REMOVED_EXCEPTION(hr) throw Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)