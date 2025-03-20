#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "platform/WindowsImpl.hpp"

bool WindowsImpl::is_window_closed = false;

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
	wcx.lpszMenuName = class_name;
	wcx.lpszClassName = class_name;
	wcx.hIconSm = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	RegisterClassEx(&wcx);

	m_hwnd = CreateWindowExA(
		0, class_name, class_name, WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX | WS_MAXIMIZEBOX), 0, 0, m_width, m_height, nullptr, nullptr, m_hInstance, nullptr
	);

	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WindowsImpl::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		is_window_closed = true;
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
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
	return !WindowsImpl::is_window_closed;
}

void WindowsImpl::ToggleFullscreen()
{
	DEVMODEA dm{};
	dm.dmSize = sizeof(dm);

	if (m_is_fullscreen)
	{
		SetWindowLong(m_hwnd, GWL_STYLE, (WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX | WS_MAXIMIZEBOX)) | WS_VISIBLE);
		SetWindowPos(m_hwnd, nullptr, 0, 0, 800, 600, SWP_NOZORDER | SWP_FRAMECHANGED);
		UpdateWindow(m_hwnd);

		m_is_fullscreen = false;
	}
	else {

		if (!EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm))
		{
			throw std::runtime_error("Could not get display settings");
			return;
		}

		m_width = dm.dmPelsWidth;
		m_height = dm.dmPelsHeight;

		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			throw std::runtime_error("Could not set full screen");
			return;
		}

		SetWindowLong(m_hwnd, GWL_STYLE, WS_POPUP);

		SetWindowPos(m_hwnd, HWND_TOP, 0, 0, dm.dmPelsWidth, dm.dmPelsHeight, SWP_FRAMECHANGED | SWP_NOZORDER);
		ShowWindow(m_hwnd, SW_MAXIMIZE);
		UpdateWindow(m_hwnd);

		m_is_fullscreen = true;
	}

}

void WindowsImpl::ToggleCursor()
{
	CURSORINFO pci{};
	pci.cbSize = sizeof(pci);
	GetCursorInfo(&pci);

	if (pci.flags == CURSOR_SHOWING)
	{
		ShowCursor(FALSE);
	}
	else {
		ShowCursor(TRUE);
	}
}
