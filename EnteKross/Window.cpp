#include "Window.h"

#include <sstream>
#include <WinUser.h>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInstance(GetModuleHandle(NULL))
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgCreate;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hCursor = NULL;
	wc.hIcon = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GetName();
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(GetName(), GetInstance());
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInstance;
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return pClassName;
}

Window::Window(unsigned int width, unsigned int height, const wchar_t* name)
	:
	width(width),
	height(height),
	name(name)
{
	RECT rt = {};
	rt.left = 100;
	rt.top = 100;
	rt.right = rt.left + width;
	rt.bottom = rt.top + height;

	if (AdjustWindowRect(&rt, WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, FALSE) == 0)
	{
		ENTE_WND_THROW_LAST_EXCEPTION();
	}

	hWnd = CreateWindow
	(
		WindowClass::GetName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rt.right - rt.left,
		rt.bottom - rt.top,
		NULL,
		NULL,
		WindowClass::GetInstance(),
		this
	);

	if (hWnd == 0)
	{
		ENTE_WND_THROW_LAST_EXCEPTION();
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT Window::HandleMsgCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		// Get Window Instance from the data passed to the CreateWindow function
		const CREATESTRUCT* const cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const pWindowInstance = static_cast<Window*>(cs->lpCreateParams);

		// Bind Window Instance to user data
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindowInstance));

		// Retarget Windows Procedure to HandleMsgThunk
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(pWindowInstance->HandleMsgThunk));

		return pWindowInstance->HandleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Get Window class instance from user data
	Window* const pWindowInstance = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Handle Messages via member function
	return pWindowInstance->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
	{
		quitDialog = true;
		int boxID = MessageBoxEx(
			NULL,
			L"Do you want to close the application?",
			L"Exit Application", 
			MB_ICONINFORMATION | MB_YESNO,
			0x409
		);

		if (boxID == IDYES)
		{
			PostQuitMessage(0);
		}
		quitDialog = false;
		return 0;
	}
	// Mouse input
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lParam);
		if (0 <= pt.x && pt.x < width && 0 <= pt.y && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInsideWindow())
			{
				if (!quitDialog)
					SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseExit();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
		mouse.OnLeftPress();
		break;
	case WM_LBUTTONUP:
		mouse.OnLeftRelease();
		break;
	case WM_RBUTTONDOWN:
		mouse.OnRightPress();
		break;
	case WM_RBUTTONUP:
		mouse.OnRightRelease();
		break;
	case WM_MBUTTONDOWN:
		mouse.OnMiddlePress();
		break;
	case WM_MBUTTONUP:
		mouse.OnMiddleRelease();
		break;
	case WM_MOUSEWHEEL:
		mouse.OnWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	// Keyboard input
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000) || keyboard.AutoRepeatEnabled())
		{
			keyboard.OnKeyDown(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		keyboard.OnKeyRelease(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg = {};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return (int)msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowTextA(hWnd, title.c_str()) == 0)
	{
		ENTE_WND_THROW_LAST_EXCEPTION();
	}
}

Graphics& Window::Gfx() const noexcept
{
	return *pGfx;
}

Window::Exception::Exception(int line, std::string file, HRESULT hr) noexcept
	:
	KrossException(line, file),
	hr(hr)
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream err;
	err << GetType() <<
		"\nLine: " << GetLine() <<
		"\nFile: " << GetFile() <<
		"\nDescription: " << GetErrorDescription() << std::endl;

	whatBuffer = err.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "EnteKross Window Exception";
}

std::string Window::Exception::GetErrorDescription() const noexcept
{
	return TranslateErrorCode(hr);
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMessageBuf = nullptr;
	if (FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, hr, 0x409,
		reinterpret_cast<LPSTR>(&pMessageBuf), 0, NULL))
	{
		std::string errStr = pMessageBuf;
		LocalFree(pMessageBuf);
		return errStr;
	}
	else
	{
		return "Unidentified error";
	}
}