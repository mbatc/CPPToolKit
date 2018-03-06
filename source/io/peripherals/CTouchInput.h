#pragma once

#include <Windows.h>
#include <vector>

struct STOUCHINFO
{
	STOUCHINFO()
	{
		ID = 0;
		touch_order = -1;
		x = 0;
		y = 0;
		prev_x = 0;
		prev_y = 0;
		bool pressed = false;
		bool released = false;
	}

	DWORD ID;
	int touch_order;
	float x;
	float y;
	float prev_x;
	float prev_y;

	bool pressed;
	bool released;
};

class CTouchInput;

class CTouchInputState
{
	friend CTouchInput;
public:
	CTouchInputState();
	void OnTouch(WPARAM wParam, LPARAM lParam);
	void Update();
private:
	STOUCHINFO* GetInput(DWORD id);
	const STOUCHINFO* GetInput(int touch_order);

	std::vector<STOUCHINFO>	m_pInputs;
	int						m_nSequenceTouchCount;
};

class CTouchInput
{
public:
	CTouchInput(){}

	void SetState(CTouchInputState* s) { state = s; }

	bool IsDown(int index = 0) { return (bool)state->GetInput(index); }
	bool IsPressed(int index = 0) { return state->GetInput(index) ? state->GetInput(index)->pressed : false; }
	bool IsReleased(int index = 0) { return state->GetInput(index) ? state->GetInput(index)->released : false; }
	float GetTouchX(int index = 0) { return state->GetInput(index) ? state->GetInput(index)->x : -1; }
	float GetTouchY(int index = 0) { return state->GetInput(index) ? state->GetInput(index)->y : -1; }
	float GetTouchPrevX(int index = 0) { return state->GetInput(index) ? state->GetInput(index)->prev_x : -1; }
	float GetTouchPrevY(int index = 0) { return state->GetInput(index) ? state->GetInput(index)->prev_y : -1; }
	int GetTouchCount() { return state->m_nSequenceTouchCount; }
private:
	CTouchInputState* state;
};