#pragma once

#include <windows.h>

class WindowsImpl
{
public:
	WindowsImpl(int width, int height, HINSTANCE hInstance, int nCmdShow);
	~WindowsImpl();

	void Message();
	bool IsWindowClosed();

	static bool is_close_window;
private:
	void InitWindow();

	static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	HWND m_hwnd;
	HINSTANCE m_hInstance;
	int nCmdShow;
	MSG msg{};
	int m_width, m_height;
};
