#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <memory>
#include <stdexcept>
#include <windows.h>


class WindowsImpl
{
public:
	/// <summary>
	/// Ctor for WindowsImpl Class. 
	/// </summary>
	/// <param name="width">Width of window.</param>
	/// <param name="height">Height of window.</param>
	/// <param name="hInstance">HInstance from APIEntry.</param>
	/// <param name="nCmdShow">nCmdShow from APIEntry. </param>
	WindowsImpl(int width, int height, HINSTANCE hInstance, int nCmdShow);
	~WindowsImpl();

	/// <summary>
	/// This function handles Translating and Dispatching messages for win32.
	/// </summary>
	void Message();

	/// <summary>
	/// It returns status of should windows closed or not.
	/// </summary>
	/// <returns>Status of Window close state.</returns>
	bool IsWindowClosed();

	/// <summary>
	/// It toggles between fullscreen and windowed.
	/// </summary>
	void ToggleFullscreen();

	/// <summary>
	/// It toggles between hidding and showing curosor.
	/// </summary>
	void ToggleCursor();

	HWND GetHwnd() { return m_hwnd; }
	MSG& GetMsg() { return msg; }

	static bool is_window_closed;
private:
	/// <summary>
	/// Helper func for Initiliaze Window.
	/// </summary>
	void InitWindow();

	static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND m_hwnd;
	HINSTANCE m_hInstance;
	int nCmdShow;
	MSG msg{};
	int m_width, m_height;
	bool m_is_fullscreen = false;

	const LPCSTR class_name = "Hello";

};
