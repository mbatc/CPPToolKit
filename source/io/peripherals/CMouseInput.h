#pragma once
class CMouseInput;

#ifdef _WIN32
	#include <Windows.h>
	#define SetCursorPosA(x,y) SetCursorPos(x,y)
#elif _WIN64
	#include <Windows.h>
	#define SetCursorPosA(x,y) SetCursorPos(x,y)
#elif _LINUX
	#include <stdio.h> 
	#include <X11/X.h> 
	#include <X11/Xlib.h> 
	#include <X11/Xutil.h>

	void SetCursorPosLinux(Display* disp,Window* wnd, x, y);
	#define SetCursorPosA(disp,wnd,x,y) SetCursorPosLinux(disp, wnd, KeyReleaseMask)
#endif

class CMouseInputState
{
	friend CMouseInput;
public:
	CMouseInputState() 
	:
	m_mouseX(-1),
	m_mouseY(-1),
	m_mouseXPrev(-1),
	m_mouseYPrev(-1)
	{}
	~CMouseInputState() {}

	void OnLeftMousePressed();
	void OnLeftMouseRelesed();

	void OnRightMousePressed();
	void OnRightMouseReleased();

	void SetMouse(int x, int y);
	void SetMousePrev(int x, int y);

	void OnVerticalScroll(int scroll);
	void OnHorizontalScroll(int scroll);

	void Update();

private:
	bool m_leftMouseDown;
	bool m_leftMouseReleased;
	bool m_leftMousePressed;
	int m_nLeftFramesDown;
	int m_nLeftFramesUp;

	bool m_rightMouseDown;
	bool m_rightMouseReleased;
	bool m_rightMousePressed;
	int m_nRightFramesDown;
	int m_nRightFramesUp;

	int m_mouseX, m_mouseY;
	int m_mouseXPrev, m_mouseYPrev;

	int m_mouse_scroll_y, m_mouse_scroll_x;
};

class CMouseInput
{
public:
	CMouseInput() {}
	~CMouseInput() {}

	void SetMouseState(CMouseInputState* state);

	bool IsLeftMouseDown() { return m_mServ->m_leftMouseDown; }
	bool IsLeftMousePressed() { return m_mServ->m_leftMousePressed; }
	bool IsLeftMouseReleased() { return m_mServ->m_leftMouseReleased; }

	bool IsRightMouseDown() { return m_mServ->m_rightMouseDown; }
	bool IsRightMousePressed() { return m_mServ->m_rightMousePressed; }
	bool IsRightMouseReleased() { return m_mServ->m_rightMouseReleased; }

	int GetMouseX() { return m_mServ->m_mouseX; }
	int GetMouseY() { return m_mServ->m_mouseY; }
	int GetMousePrevX() { if (m_mServ->m_mouseXPrev != -1) return m_mServ->m_mouseXPrev; else return GetMouseX(); }
	int GetMousePrevY() { if (m_mServ->m_mouseXPrev != -1) return m_mServ->m_mouseYPrev; else return GetMouseY(); }

	int GetMouseScrollX() { return m_mServ->m_mouse_scroll_x; }
	int GetMouseScrollY() { return m_mServ->m_mouse_scroll_y; }

	void SetPosition(int x, int y);
	void SetMouse(int x, int y) { m_mServ->SetMouse(x, y); m_mServ->SetMousePrev(x, y); }

private:
	CMouseInputState* m_mServ;
};