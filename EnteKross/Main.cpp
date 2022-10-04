#include "Window.h"

#include <sstream>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	try
	{
		Window window(800, 600, L"Poopie");

		while (true)
		{
			Sleep(1);
			if (const auto ecode = Window::ProcessMessages())
			{
				return *ecode;
			}
		}
	}
	catch (KrossException& ex)
	{
		MessageBoxA(NULL, ex.what(), ex.GetType(), MB_ICONERROR | MB_OK);
	}
	catch (std::exception& ex)
	{
		MessageBoxA(NULL, ex.what(), "Standard Exception", MB_ICONERROR | MB_OK);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unhandled exception", "Error", MB_ICONERROR | MB_OK);
	}

	return 0;
}