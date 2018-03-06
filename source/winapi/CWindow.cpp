#include "CWindow.h"
#include "../tools/CLog.h"

//CSystem* PUMP::g_system = NULL;
//
//LRESULT CALLBACK WndProcPump(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	if(PUMP::g_system)
//		return PUMP::g_system->SystemWndProc(hWnd,msg,wParam,lParam);
//	MessageBox(NULL,"g_system Not Initialized Correctly!","Error!",
//		MB_OK | MB_ICONEXCLAMATION );
//	return false;
//}

CWindow::CWindow()
	:
	m_hWnd( 0 ),
	m_clientWidth(0),
	m_clientHeight(0),
	m_windowClassName(nullptr),
	m_windowName(nullptr)
{}

CWindow::~CWindow()
{
	m_windowName.clear();
	m_windowClassName.clear();
}

bool CWindow::Init(HINSTANCE hInstance, unsigned int clientWidth, unsigned int clientHeight, LRESULT(CALLBACK *func)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam))
{
	Log(this, DEBUGLOG_LEVEL_INFO, "Initialising window (ptr=%p)", this);
	m_hInstance = hInstance;

	ZeroMemory(&wcex,sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = func;


	//wcex.lpfnWndProc = WndProcPump;
	wcex.lpszClassName = m_windowClassName.c_str();
	wcex.lpszMenuName = nullptr;
	wcex.style = 0;

	if (!RegisterClassEx(&wcex))
	{
		Log(this, DEBUGLOG_LEVEL_FATAL, "Failed to register window class: %s", m_windowClassName);
		return false;
	}
	Log(this, DEBUGLOG_LEVEL_INFO, "Window class registered: %s ", m_windowClassName);
	

	m_clientWidth = clientWidth;
	m_clientHeight = clientHeight;

	m_windowRect.bottom = clientHeight;
	m_windowRect.right = clientWidth;
	m_windowRect.left = 0;
	m_windowRect.top = 0;

	AdjustWindowRect(&m_windowRect, m_windowStyle, false);

	return true;
}

bool CWindow::CWndCreateWindow()
{
	int posX, posY, width, height;

	width = m_windowRect.right - m_windowRect.left;
	height = m_windowRect.bottom - m_windowRect.top;

	posX = (GetSystemMetrics(SM_CXSCREEN)/2) - width/2;
	posY = (GetSystemMetrics(SM_CYSCREEN)/2) - height/2;

	m_hWnd = CreateWindow(m_windowClassName.c_str(),m_windowName.c_str(),m_windowStyle, posX, posY,
		width, height, NULL, NULL, wcex.hInstance, NULL);
	if (!RegisterTouchWindow(m_hWnd, NULL))
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "CWindow: Touch Input not Supported...\n");
	}

	if(!m_hWnd)
		return false;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

void CWindow::SetHeight(unsigned int height)
{
	m_clientHeight = height;
}

void CWindow::SetWidth(unsigned int width)
{
	m_clientWidth = width;
}

void CWindow::SetWindowClassName(const char* name)
{
	m_windowClassName = name;
}

void CWindow::SetWindowName(const char* name)
{
	m_windowName = name;
}

void CWindow::SetWindowStyle(DWORD style)
{
	m_windowStyle = style;
}

HWND CWindow::GetHandle()
{
	return m_hWnd;
}

UINT CWindow::GetHeight()
{
	RECT r;
	GetClientRect(m_hWnd, &r);
	m_clientHeight = r.bottom - r.top;
	return r.bottom - r.top;
}

UINT CWindow::GetWidth()
{
	RECT r;
	GetClientRect(m_hWnd,&r);
	m_clientWidth = r.right - r.left;
	return r.right - r.left;
}