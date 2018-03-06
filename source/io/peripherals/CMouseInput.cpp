#include "CMouseInput.h"

#ifdef _LINUX
void SetCursorPosLinux(Display* disp, Window* wnd, int x, int y)
{
	XSelectInput(disp, wnd, x, y);
	XWarpPointer(disp, None, wnd, 0, 0, 0, 0, x, y);
	XFlush(disp);
}
#endif

void CMouseInputState::OnLeftMousePressed()
{
	m_leftMouseDown = true;
	if (m_nLeftFramesDown == 0)
	{
		m_leftMousePressed = true;
	}

	m_nLeftFramesUp = 0;
}

void CMouseInputState::OnLeftMouseRelesed()
{
	m_leftMouseDown = false;
	if (m_nLeftFramesUp == 0)
	{
		m_leftMouseReleased = true;
	}

	m_nLeftFramesDown = 0;
}

void CMouseInputState::OnRightMousePressed()
{
	m_rightMouseDown = true;
	if (m_nRightFramesDown == 0)
	{
		m_rightMousePressed = true;
	}

	m_nRightFramesUp = 0;
}

void CMouseInputState::OnRightMouseReleased()
{
	m_rightMouseDown = false;
	if (m_nRightFramesUp == 0)
	{
		m_rightMouseReleased = true;
	}

	m_nRightFramesDown = 0;
}

void CMouseInputState::SetMouse(int x, int y)
{
	m_mouseX = x;
	m_mouseY = y;
}

void CMouseInputState::SetMousePrev(int x, int y)
{
	m_mouseXPrev = x;
	m_mouseYPrev = y;
}

void CMouseInputState::OnVerticalScroll(int scroll)
{
	m_mouse_scroll_y = scroll/120;
}

void CMouseInputState::OnHorizontalScroll(int scroll)
{
	m_mouse_scroll_x = scroll/120;
}

void CMouseInputState::Update()
{
	m_leftMousePressed = false;
	m_leftMouseReleased = false;
	m_rightMousePressed = false;
	m_rightMouseReleased = false;

	if (m_leftMouseDown)
		m_nLeftFramesDown++;
	else
		m_nLeftFramesUp++;

	if (m_rightMouseDown)
		m_nRightFramesDown++;
	else
		m_nRightFramesUp++;

	SetMousePrev(m_mouseX, m_mouseY);
	m_mouse_scroll_y = 0;
	m_mouse_scroll_x = 0;
}

void CMouseInput::SetMouseState(CMouseInputState * state)
{
	m_mServ = state;
}

void CMouseInput::SetPosition(int x, int y)
{
	SetCursorPosA(x, y);
	SetMouse(x, y);
}
