#include "platform/WindowsImpl.hpp"
#include <Windows.h>

bool WindowsImpl::is_close_window = false;

WindowsImpl::WindowsImpl(int width, int height, HINSTANCE hInstance, int nCmdShow)
{
	m_width = width;
	m_height = height;
	m_hInstance = hInstance;
	this->nCmdShow = nCmdShow;
	InitWindow();
}

WindowsImpl::~WindowsImpl() {

}

void WindowsImpl::InitWindow() {
	WNDCLASSEX wcx{};
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW |
		CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = m_hInstance;
	wcx.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL,
		IDC_ARROW);
	wcx.lpszMenuName = "MainMenu";
	wcx.lpszClassName = "MainWClass";
	wcx.hIconSm = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	RegisterClassEx(&wcx);

	m_hwnd = CreateWindowExA(
		0, "MainWClass", "MainMenu", WS_OVERLAPPEDWINDOW, 0, 0, m_width, m_height, nullptr, nullptr, m_hInstance, nullptr
	);

	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);
}

LRESULT WindowsImpl::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		is_close_window = true;
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void WindowsImpl::Message() {
	while (PeekMessageA(&msg, m_hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

bool WindowsImpl::IsWindowClosed()
{
	return !WindowsImpl::is_close_window;
}
