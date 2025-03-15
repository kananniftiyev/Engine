#pragma once

#include <stdexcept>
#include <windows.h>


class WindowsImpl
{
public:
	WindowsImpl(int width, int height, HINSTANCE hInstance, int nCmdShow);
	~WindowsImpl();

	void Message();
	bool IsWindowClosed();
	void ToggleFullscreen();
	void ToggleCursor();

	HWND GetHwnd() { return m_hwnd; }

	static bool is_close_window;
private:
	void InitWindow();

	static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	// TODO: Better var names.
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	int nCmdShow;
	MSG msg{};
	int m_width, m_height;
	bool m_fullscreen = false;

	const LPCSTR class_name = "Hello";
};
