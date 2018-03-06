#pragma once

//#include <Windows.h>
#include <string.h>

struct KeyState
{
	bool isDown;
	bool isPressed;
	bool isReleased;

	int nFramesDown;
	int nFramesUp;
};

class CKeyboardInput;

class CKeyboardInputState
{
	friend CKeyboardInput;

public:
	CKeyboardInputState() 
	{
		memset(keyState,0, sizeof(KeyState) * 256);
		in_caps = false;
		memset(m_inputBuffer,0, sizeof(m_inputBuffer));
		m_bufLen = 0;
	}

	void OnKeyPressed(unsigned int keyCode);
	void OnKeyReleased(unsigned int keyCode);
	void OnCharacterPressed(char c);
	void Update();

private:
	KeyState keyState[256];

	void AddToBuffer(char keyCode);
	bool in_caps;

	char m_inputBuffer[64];
	int m_bufLen;
};

class CKeyboardInput
{
public:
	CKeyboardInput() {}
	~CKeyboardInput() {}

	void SetKeyboardState(CKeyboardInputState* state) { keyboard = state; }

	bool IsDown(unsigned int keyCode) { return keyboard->keyState[keyCode].isDown; }
	bool IsPressed(unsigned int keyCode) { return keyboard->keyState[keyCode].isPressed; }
	bool IsReleased(unsigned int keyCode) { return keyboard->keyState[keyCode].isReleased; }
	const char* InputBuffer() { return keyboard->m_inputBuffer; }

private:
	CKeyboardInputState* keyboard;
};