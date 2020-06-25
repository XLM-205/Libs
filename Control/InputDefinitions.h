/*-------Moon Wiz Input Definitions-------
*	--Provides definitions and key mask to the main input handler library
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

#ifndef _H_MWINPUT_DEF_
#define _H_MWINPUT_DEF_

#include "Igniter.h"

#ifdef MWI_OS_WINDOWS
enum MessageTypes
{
	MWIH_MESSAGE_SYSKEYDOWN = WM_SYSKEYDOWN,		//Keyboard (system)key press
	MWIH_MESSAGE_KEYDOWN = WM_KEYDOWN,				//Keyboard key press
	MWIH_MESSAGE_SYSKEYUP = WM_SYSKEYUP,			//Keyboard (system)key release
	MWIH_MESSAGE_KEYUP = WM_KEYUP,					//Keyboard key release
	MWIH_MESSAGE_MOUSEMOVE = WM_MOUSEMOVE,			//Mouse moviment
	MWIH_MESSAGE_MOUSEWHEEL = WM_MOUSEWHEEL,		//Mouse Wheel moviment
	MWIH_MESSAGE_LMOUSEDOWN = WM_LBUTTONDOWN,		//Left mouse button press
	MWIH_MESSAGE_LMOUSEUP = WM_LBUTTONUP,			//Left mouse button release
	MWIH_MESSAGE_RMOUSEDOWN = WM_RBUTTONDOWN,		//Right mouse button press
	MWIH_MESSAGE_RMOUSEUP = WM_RBUTTONUP,			//Right mouse button release
	MWIH_MESSAGE_MMOUSEDOWN = WM_MBUTTONDOWN,		//Middle mouse button press
	MWIH_MESSAGE_MMOUSEUP = WM_MBUTTONUP,			//Middle mouse button release
	MWIH_MESSAGE_MOUSEMOVECTRL = WM_NCMOUSEMOVE,	//Mouse moviment on window's control
	//Special Messages
	MWIH_MESSAGE_PAINT = WM_PAINT,					//Windows specific for painting windows
	MWIH_MESSAGE_CLOSE = WM_CLOSE,					//Window received a request to close
	MWIH_MESSAGE_SIZE = WM_SIZE,					//Window is being resized
	MWIH_MESSAGE_AFK = (WM_USER + 1)				//No message received ("Away From Keyboard")

};

enum KeyCodes
{
	MWIH_KEY_LEFT = VK_LEFT,
	MWIH_KEY_RIGHT = VK_RIGHT,
	MWIH_KEY_UP = VK_UP,
	MWIH_KEY_DOWN = VK_DOWN
};

#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#endif

#endif;	//Moon Wiz Input Definitions