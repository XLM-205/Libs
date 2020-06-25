/*-------Moon Wiz OS Message Box Handler-------
*	--Handles message box across multiple OSes
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

#ifndef _H_MWMESSAGEBOX_HANDLER_
#define _H_MWMESSAGEBOX_HANDLER_
//Moon Wiz Message Box Handler -> A.K.A MWMBH

#include "Igniter.h"

#ifdef MWI_OS_WINDOWS
	#include <WinUser.h>
	enum MessageBoxButtons
	{
		MBB_OK = MB_OK,
		MBB_OK_CANCEL = MB_OKCANCEL,
		MBB_ABORT_RETRY_IGNORE = MB_ABORTRETRYIGNORE,
		MBB_YES_NO_CANCEL = MB_YESNOCANCEL,
		MBB_YES_NO = MB_YESNO,
		MBB_RETRY_CANCEL = MB_RETRYCANCEL
	};

	enum MessageBoxIcons
	{
		MBI_WARNING = MB_ICONWARNING,
		MBI_INFO = MB_ICONINFORMATION,
		MBI_ERROR = MB_ICONERROR
	};
	
	enum MessageBoxResponses
	{
		MBR_OK = IDOK,
		MBR_CANCEL = IDCANCEL,
		MBR_ABORT = IDABORT,
		MBR_RETRY = IDRETRY,
		MBR_IGNORE = IDIGNORE,
		MBR_YES = IDYES,
		MBR_NO = IDNO
	};
#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#endif

namespace MWMessageBoxHandler
{
	#ifdef MWI_OS_WINDOWS
		inline int Message(const char* Text, const char* Title, MessageBoxButtons Buttons, MessageBoxIcons Icon)
		{
			return MessageBoxA(NULL, Text, Title, Buttons | Icon);
		}
		inline bool Reponse(const char* Text, const char* Title, MessageBoxButtons Buttons, MessageBoxIcons Icon, MessageBoxResponses ExpectedResponse)
		{
			return MessageBoxA(NULL, Text, Title, Buttons | Icon) == ExpectedResponse ? true : false;
		}
		inline int Message(const char* Text, const char* Title)
		{
			return MessageBoxA(NULL, Text, Title, MessageBoxButtons::MBB_OK | MessageBoxIcons::MBI_INFO);
		}
		inline bool Reponse(const char* Text, const char* Title, MessageBoxResponses ExpectedResponse)
		{
			return MessageBoxA(NULL, Text, Title, MessageBoxButtons::MBB_YES_NO_CANCEL | MessageBoxIcons::MBI_INFO) == ExpectedResponse ? true : false;
		}
	#elif defined MWI_OS_LINUX

	#elif defined MWI_OS_OSX

	#endif
}

#endif;	//MWMBH