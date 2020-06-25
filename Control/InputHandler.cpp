/*-------Moon Wiz Input Handler-------
*	--Handles keyboard and Mouse input from the system
*
*	Copyright (c) 2015 Moon Wiz Studios
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version --.--
*/

#include "InputHandler.h"
#include <iostream>

//Holds every possible key press
bool KeyPressed[256] = { 0 };			//Keyboard state
bool IsMouseLeftDown = false;			//Becomes true if the left mouse button is pressed
bool IsMouseRightDown = false;			//Becomes true if the right mouse button is pressed
bool IsMouseMiddleDown = false;			//Becomes true if the middle mouse button is pressed
bool IsIdle = true;						//Becomes true if no input is detected (either from mouse, keyboard or any supported input device)
bool IsMouseMoving = false;				//Becomes true if the mouse is moving
bool IsKeyPressed = false;				//Becomes true if ANY key is being pressed
int8 MouseWheel = 0;					// [-1] BACKWARDS | [0] Idle | [1] FOWARD
Vec2<int16> MousePos;					//The mouse position on the screen
uint32 UntreatedMessages = 0;			//Holds the last intercepted message sent from the OS, not including key presses nor mouse movement
int64 SpecialParameter = 0;				//Reserved parameter. Will be updated when any special message not handled was sent

//OS Specific Controllers
#ifdef MWI_OS_WINDOWS
#include <Windowsx.h>

#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#else
#pragma message ("[MWIH][WARNING] -> Could not determine the host system. Cannot read input")
#endif //OS Specific Controllers

bool MWInputHandler::isIdle(void)
{
	return IsIdle;
}
bool MWInputHandler::isKeyboardIdle(void)
{
	return IsKeyPressed;
}
bool MWInputHandler::isKeyPressed(uint8 Key)
{
	return KeyPressed[Key];
}
bool MWInputHandler::isMouseIdle(void)
{
	return IsMouseMoving;
}
bool MWInputHandler::isMouseLeftDown(void)
{
	return IsMouseLeftDown;
}
bool MWInputHandler::isMouseRightDown(void)
{
	return IsMouseRightDown;
}
bool MWInputHandler::isMouseMiddleDown(void)
{
	return IsMouseMiddleDown;
}
bool MWInputHandler::isMouseAnyDown(void)
{
	if (!IsMouseLeftDown)
	{
		if (!IsMouseRightDown)
		{
			if (!IsMouseMiddleDown)
			{
				return false;
			}
		}
	}
	return true;
}
int8 MWInputHandler::getMouseWheelStatus(void)
{
	return MouseWheel;
}
uint16 MWInputHandler::getMouseXPos(void)
{
	return MousePos._X;
}
uint16 MWInputHandler::getMouseYPos(void)
{
	return MousePos._Y;
}
Vec2<int16> MWInputHandler::getMousePos(void)
{
	return MousePos;
}
mwVec2d MWInputHandler::getMouseCentralized(const uint16 WWidth, const uint16 WHeigth, const double FullOrtho)
{
	mwVec2d TheoricCenter(((double)MousePos._X / WWidth) - 0.5, ((double)MousePos._Y / WHeigth) - 0.5);
	mwVec2d RealCenter(0, 0);

	RealCenter._X = TheoricCenter._X * FullOrtho;
	RealCenter._Y = -TheoricCenter._Y * FullOrtho;

	return RealCenter;
}
mwVec2d MWInputHandler::getMouseCentralizedRatio(uint16 WWidth, uint16 WHeigth, double FullOrtho, double FullOrthoXRatio)
{
	mwVec2d TheoricCenter(((double)MousePos._X / WWidth) - 0.5, ((double)MousePos._Y / WHeigth) - 0.5);
	mwVec2d RealCenter(0, 0);

	RealCenter._X = TheoricCenter._X * FullOrthoXRatio;
	RealCenter._Y = -TheoricCenter._Y * FullOrtho;

	return RealCenter;
}
uint32 MWInputHandler::getSpecialMessage(void)
{
	return UntreatedMessages;
}
int64 MWInputHandler::getSpecialParameter(void)
{
	return SpecialParameter;
}
uint32 MWInputHandler::getPurgeSpecialMessage(void)
{
	uint32 Temp = UntreatedMessages;
	UntreatedMessages = 0;
	return Temp;
}
int64 MWInputHandler::getPurgeSpecialParameter(void)
{
	int64 Temp = SpecialParameter;
	SpecialParameter = 0;
	return Temp;
}
void MWInputHandler::purgeSpecialMessage(void)
{
	UntreatedMessages = 0;
}
void MWInputHandler::purgeSpecialParameter(void)
{
	SpecialParameter = 0;
}
void MWInputHandler::setKeyState(uint8 Key, bool Down)
{
	KeyPressed[Key] = Down;
}

uint32 MWInputHandler::getHighWord(int64 In)
{
	//Code reformated from 'GET_X_LPARAM'
	/*int Out = 0;
	Out = (uint16)(((uint64)In) & 0xffff);
	return Out;*/
	return In & 0xFFFF;
}
uint32 MWInputHandler::getLowWord(int64 In)
{
	/*int Out = 0;
	Out = (uint16)((((uint64)In) >> 16) & 0xffff);
	return Out;*/
	return (In >> 16) & 0xFFFF;
}

//Function callbacks
int(*CloseHandler)(void) = nullptr;												//Closing Callback
void(*ResizeHandler)(uint16 Width, uint16 Heigth) = nullptr;					//Window Rezise Callback

void(*KeyPressHandler)(uint8 Key) = nullptr;													//Keyboard Key Press (Down) Callback. Returns the key pressed
void(*KeyReleaseHandler)(uint8 Key) = nullptr;													//Keyboard Key Release (Up) Callback. Returns the key released

//Secondary handler functions take priority over pure 'MouseHandler'. If the respective callback is set, the primary callback WILL NOT be called 
void(*MouseHandler)(Vec2<int16> MPos, int8 MWheelState, uint8 MKey, uint8 MState) = nullptr;	//Mouse State Change (Move, button, wheel) Callback

//OS Specific Input
#ifdef MWI_OS_WINDOWS

//More info on Raw Input:
//http://codereal.blogspot.com.br/2009/11/raw-input-introduction-part-1-keyboard.html

RAWINPUTDEVICE KeyboardIn;
MSG		InMsg;					//Where we store the intercepted messages
HWND	CurrHandle = NULL;		//The Handle to the current WiNDow
uint8	MessageStatus = 0;		//Holds the Message Status of the current intercepted message ([>0] Input | [0] 'WM_QUIT' | [-1] Error)
uint32	RawInputBufferSize = 0;	//The buffer size for a complete input (Raw)
uint32	PeekFlags = PM_REMOVE | PM_QS_INPUT;

bool MWInputHandler::init(void)
{
	KeyboardIn.usUsagePage = 0x01;
	KeyboardIn.usUsage = 0x06;				//Code for 'Keyboard'
	KeyboardIn.dwFlags = RIDEV_NOLEGACY;
	KeyboardIn.hwndTarget = 0;

	return RegisterRawInputDevices(&KeyboardIn, 1, sizeof(KeyboardIn));
}

bool MWInputHandler::dispatchInput(void)
{
	if (PeekMessage(&InMsg, CurrHandle, 0, 0, PeekFlags))
	{
		//Required for a pure window system
		TranslateMessage(&InMsg);
		DispatchMessage(&InMsg);
		return true;
	}
	return false;
}
void MWInputHandler::setInputWindowHandle(MWI_UNVSAL_WINDOW_HANDLE Focus)
{
	CurrHandle = Focus;
	KeyboardIn.hwndTarget = Focus;
}
void __stdcall MWInputHandler::setExternalHandleClosing(int(*Handler)(void))
{
	CloseHandler = Handler;
}
void __stdcall MWInputHandler::setExternalHandleResize(void(*Handler)(uint16 Width, uint16 Heigth))
{
	ResizeHandler = Handler;
}
void __stdcall MWInputHandler::setExternalHandleMouse(void(*Handler)(Vec2<int16> MPos, int8 MWheelState, uint8 MKey, uint8 MState))
{
	MouseHandler = Handler;
}
void __stdcall MWInputHandler::setExternalHandleKeyDown(void(*Handler)(uint8 Key))
{
	KeyPressHandler = Handler;
}
void __stdcall MWInputHandler::setExternalHandleKeyUp(void(*Handler)(uint8 Key))
{
	KeyReleaseHandler = Handler;
}

void __stdcall MWInputHandler::poolCustomKeyMouseHandler(void(__cdecl *Handler)(uint32 Message, uint16 Key, Vec2<int16> Mouse))
{
	static uint16 StickyVKey = 0;
	if (PeekMessage(&InMsg, CurrHandle, 0, 0, PeekFlags))
	{
		GetRawInputData((HRAWINPUT)InMsg.lParam, RID_INPUT, NULL, &RawInputBufferSize, sizeof(RAWINPUTHEADER));
		uint8 *lpb = new uint8[RawInputBufferSize];
		if (!lpb)	//We couldn't alloc memory (dafuq?!). Abort
		{
			printf("[MWIH] -> ALLOC ERROR\n");
			return;
		}
		int Status = GetRawInputData((HRAWINPUT)InMsg.lParam, RID_INPUT, lpb, &RawInputBufferSize, sizeof(RAWINPUTHEADER));
		if (Status != RawInputBufferSize && Status != -1)		//If [-1] this means the function failed to read from the keyboard (Maybe no input received)
		{
			//Error: Function does not return correct size. Abort
			delete[] lpb;
			lpb = nullptr;
			printf("[MWIH] -> INPUT CATCH ERROR\n");
			return;
		}
		RAWINPUT *inRaw = (RAWINPUT*)lpb;
		if (inRaw->header.dwType == RIM_TYPEKEYBOARD)	//Keyboard events
		{
			//HINT! We receive a VKey of 255 before and after each keyboard update (key up and down)
			if (inRaw->data.keyboard.VKey == 255)		//Invalid virtual key! Abort
			{
				delete[] lpb;
				lpb = nullptr;
				printf("[MWIH] -> INVALID KEY ERROR\n");
				return;
			}
			StickyVKey = inRaw->data.keyboard.VKey;
			Handler(inRaw->data.keyboard.Message, StickyVKey, MousePos);
		}
		else											//Mouse events
		{
			StickyVKey = 0;
			MousePos._X = GET_X_LPARAM(InMsg.lParam);
			MousePos._Y = GET_Y_LPARAM(InMsg.lParam);
			Handler(InMsg.message, StickyVKey, MousePos);
		}
		delete[] lpb;
		lpb = nullptr;
	}
	else
	{
		Handler(MWIH_MESSAGE_AFK, 0, MousePos);
	}
}

LRESULT __stdcall MWInputHandler::InputInterceptor(HWND hWnd, uint32 Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_KEYDOWN:
		KeyPressed[wParam] = true;
		if (KeyPressHandler)
		{
			KeyPressHandler(wParam);
		}
		break;
	case WM_KEYUP:
		KeyPressed[wParam] = false;
		if (KeyReleaseHandler)
		{
			KeyReleaseHandler(wParam);
		}
		break;
	case WM_MOUSEMOVE:
		//Getting X and Y values of the mouse on the active part of the window (excluding OS window controls)
		//Follows the same rules as GLUT's mouse functions: X increments from the LEFT to the RIGHT, and Y increments from the TOP to the BOTTOM. Both starts from (0, 0)
		MousePos._X = MWInputHandler::getHighWord(lParam);
		MousePos._Y = MWInputHandler::getLowWord(lParam);
		MouseHandler(MousePos, MWI_MWHEEL_STOP, MWI_MKEY_NONE, MWI_MSTATE_NONE);
		break;
		//case WM_NCMOUSEMOVE:
		//	std::cout << "Mouse On Controls Moviment\n";
		//	break;
	case WM_MOUSEWHEEL:
		if (((int8)MWInputHandler::getLowWord(wParam)) >= 0)	//If the value of delta is HIGHER than 0, then the user is scrolling AWAY from the mouse (UP)
		{
			MouseWheel = 1;
		}
		else													//Therefore, if the value is LOWER than 0, the the user is scrolling TOWARS him
		{
			MouseWheel = -1;
		}
		MouseHandler(MousePos, MouseWheel, MWI_MKEY_NONE, MWI_MSTATE_NONE);
		break;
	case WM_LBUTTONDOWN:
		MouseHandler(MousePos, MWI_MWHEEL_STOP, MWI_MKEY_LEFT, MWI_MSTATE_DOWN);
		IsMouseLeftDown = true;
		break;
	case WM_LBUTTONUP:
		MouseHandler(MousePos, MWI_MWHEEL_STOP, MWI_MKEY_LEFT, MWI_MSTATE_UP);
		IsMouseLeftDown = false;
		break;
	case WM_RBUTTONDOWN:
		MouseHandler(MousePos, MWI_MWHEEL_STOP, MWI_MKEY_RIGHT, MWI_MSTATE_DOWN);
		IsMouseRightDown = true;
		break;
	case WM_RBUTTONUP:
		MouseHandler(MousePos, MWI_MWHEEL_STOP, MWI_MKEY_RIGHT, MWI_MSTATE_UP);
		IsMouseRightDown = false;
		break;
	case WM_MBUTTONDOWN:
		MouseHandler(MousePos, MWI_MWHEEL_STOP, MWI_MKEY_MIDDLE, MWI_MSTATE_DOWN);
		IsMouseMiddleDown = true;
		break;
	case WM_MBUTTONUP:
		MouseHandler(MousePos, MWI_MWHEEL_STOP, MWI_MKEY_MIDDLE, MWI_MSTATE_UP);
		IsMouseMiddleDown = false;
		break;
	case WM_CLOSE:			//Called when the window received a 'close' signal
		if (CloseHandler)
		{
			return CloseHandler();
		}
		else
		{
			UntreatedMessages = MWIH_MESSAGE_CLOSE;
		}
		break;
	case WM_SIZE:		//Called when the window is resizing
		if (ResizeHandler)
		{
			ResizeHandler(MWInputHandler::getHighWord(lParam), MWInputHandler::getLowWord(lParam));
		}
		else
		{
			UntreatedMessages = MWIH_MESSAGE_SIZE;
			SpecialParameter = lParam;
		}
	break;
	default:
		//std::cout << "Untreated Message Received\n";
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#endif //OS Specific Input