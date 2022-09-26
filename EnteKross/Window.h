#pragma once

#include <optional>

#include "WindowsKross.h"

class Window
{
private:
	class WindowClass
	{
	public:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator =(const WindowClass&) = delete;
	public:
		static HINSTANCE GetInstance() noexcept;
		static const wchar_t* GetName() noexcept;
	private:
		static constexpr const wchar_t* pClassName = L"EnteKross Window Class";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};
public:
	Window(unsigned int width, unsigned int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator =(const Window&) = delete;
public:
	static LRESULT CALLBACK HandleMsgCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static std::optional<int> ProcessMessages() noexcept;
private:
	HWND hWnd;
	unsigned int width;
	unsigned int height;
	const wchar_t* name;
};