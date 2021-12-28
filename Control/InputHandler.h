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

#ifndef _H_MWINPUT_HANDLER_
#define _H_MWINPUT_HANDLER_

// !! This header have it's .cpp part

#include "InputDefinitions.h"
#include "..\Maths\Vec2d.h"

enum MouseHandlerValue
{
	MWI_MWHEEL_UP = -1,
	MWI_MWHEEL_STOP,
	MWI_MWHEEL_DOWN,
	MWI_MKEY_NONE = 0,
	MWI_MKEY_LEFT,
	MWI_MKEY_RIGHT,
	MWI_MKEY_MIDDLE,
	MWI_MSTATE_NONE = 0,
	MWI_MSTATE_DOWN,
	MWI_MSTATE_UP
};

namespace MWInputHandler
{
	bool isIdle(void);
	bool isKeyboardIdle(void);
	bool isKeyPressed(uint8 Key);
	bool isMouseIdle(void);
	bool isMouseLeftDown(void);
	bool isMouseRightDown(void);
	bool isMouseMiddleDown(void);
	bool isMouseAnyDown(void);
	int8 getMouseWheelStatus(void);
	uint16 getMouseXPos(void);
	uint16 getMouseYPos(void);
	Vec2<int16> getMousePos(void);
	mwVec2d getMouseCentralized(const uint16 WindowWidth, const uint16 WindowHeigth, double FullOrtho);
	mwVec2d getMouseCentralizedRatio(const uint16 WindowWidth, const uint16 WindowHeigth, double FullOrtho, double FullOrthoXRatio);
	uint32 getSpecialMessage(void);
	int64 getSpecialParameter(void);
	uint32 getPurgeSpecialMessage(void);
	int64 getPurgeSpecialParameter(void);
	void setInputWindowHandle(MWI_UNVSAL_WINDOW_HANDLE Focus);
	void setKeyState(uint8 Key, bool Down);
	void purgeSpecialMessage(void);
	void purgeSpecialParameter(void);

	uint32 getLowWord(int64 In);
	uint32 getHighWord(int64 In);

	bool init(void);
	void poolInput(void);
	bool dispatchInput(void);		//Returns true if a message was dispatched and false if none was found //Windows exclusive ??
	void d_KeyboardInput(void);

#ifdef MWI_OS_WINDOWS

	void __stdcall poolCustomKeyMouseHandler(void(__cdecl *Handler)(uint32 Message, uint16 Key, Vec2<int16> Mouse));

	//Main interceptor		---------------------------------------------------------------------
	LRESULT __stdcall InputInterceptor(HWND hWnd, uint32 Msg, WPARAM wParam, LPARAM lParam);

	//Callback with a close request
	//> Required Structure: int <func>(void)
	void __stdcall setExternalHandleClosing(int(*Handler)(void));
	//Callback with a Window Resize message
	//> Required Structure: void <func>(uint16, uint16)
	void __stdcall setExternalHandleResize(void(*Handler)(uint16 Width, uint16 Heigth));

	//Callback with a Keyboard Key Press (Down)
	//> Required Structure: void <func>(uint8)
	void __stdcall setExternalHandleKeyDown(void(*Handler)(uint8 Key));
	//Callback with a Keyboard Key Release (Up)
	//> Required Structure: void <func>(uint8)
	void __stdcall setExternalHandleKeyUp(void(*Handler)(uint8 Key));

	//'MPos' Values: [-1, -1] If no mouse movement updates where captured and any value >= 0 if it where
	//'MWheelState' Values: [-1] - Scroll Up | [0] - Stopped | [1] Scroll Down
	//'MKey' values: [0] - No mouse button press | [1] - Left Mouse Button | [2] - Right Mouse Button | [3] - Middle Mouse Button
	//'MState' values: [0] - No state change | [1] - Press (Down) | [2] - Release (Up)
	//> Required Structure: void <func>(Vec2<int16>, int8, uint8, uint8)
	void __stdcall setExternalHandleMouse(void(*Handler)(Vec2<int16> MPos, int8 MWheelState, uint8 MKey, uint8 MState));

	//Special callbacks. To enable them, uncomment in the .cpp, here and the 'InputInterceptor' below
	////Callback with a mouse Left Click. Sends the mouse position
	//void __stdcall setExternalHandleMouseLeftClick(void(*Handler)(Vec2<int16> MPos));
	////Callback with a mouse Left Click (continous call while UP). Sends the mouse position
	//void __stdcall setExternalHandleMouseLeftUp(void(*Handler)(Vec2<int16> MPos));
	////Callback with a mouse Left Click (continous call while DOWN). Sends the mouse position
	//void __stdcall setExternalHandleMouseLeftDown(void(*Handler)(Vec2<int16> MPos));

	////Callback with a mouse Right Click. Sends the mouse position
	//void __stdcall setExternalHandleMouseRightClick(void(*Handler)(Vec2<int16> MPos));
	////Callback with a mouse Right Click (continous call while UP). Sends the mouse position
	//void __stdcall setExternalHandleMouseRightUp(void(*Handler)(Vec2<int16> MPos));
	////Callback with a mouse Right Click (continous call while DOWN). Sends the mouse position
	//void __stdcall setExternalHandleMouseRightDown(void(*Handler)(Vec2<int16> MPos));

	////Callback with a mouse Wheel Click. Sends the mouse position
	//void __stdcall setExternalHandleMouseWheelClick(void(*Handler)(Vec2<int16> MPos));
	////Callback with a mouse Wheel Click (continous call while UP). Sends the mouse position
	//void __stdcall setExternalHandleMouseWheelUp(void(*Handler)(Vec2<int16> MPos));
	////Callback with a mouse Wheel Click (continous call while DOWN). Sends the mouse position
	//void __stdcall setExternalHandleMouseWheelDown(void(*Handler)(Vec2<int16> MPos));

	////Callback with a mouse Wheel Movement. Sends the mouse position
	//void __stdcall setExternalHandleMouseWheelMove(void(*Handler)(Vec2<int16> MPos, int8 MWheelState));
	////Callback with a mouse Movement. Sends the mouse position
	//void __stdcall setExternalHandleMouseMovement(void(*Handler)(Vec2<int16> MPos));
	
#elif defined MWI_OS_LINUX
	
#elif defined MWI_OS_OSX
	
#endif

}

#endif;	//Moon Wiz Input Handler