/*-------Moon Wiz Systems Pre-loader-------
*	--Detects and included the required OS and libraries
*
*	Copyright (c) 2015 Moon Wiz Studios
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.87
*/

#ifndef _H_IGNITER_
#define _H_IGNITER_
//a.k.a Moon Wiz Igniter (MWI)
//Pre-loader with configurations for every Moon Wiz compatible technologies, also working as an Abstraction Layer

#define IGNITER_VERSION	"0.87"	//The global preloader version

//#define MWI_ENFORCE				//If defined, checks for both OS and plataform must succeed, otherwise they will trhow errors

//#define MWI_DONT_INCLUDE_OS_BASE	//If defined, the base libraries for each OS (like <"Windows.h"> will NOT be included)
//#define MWI_OVERRIDE_OS_WINDOWS	//Initialize to a Windows Plataform
//#define MWI_OVERRIDE_OS_LINUX		//Initialize to a Linux Plataform
//#define MWI_OVERRIDE_OS_OSX		//Initialize to a Mac OSX Plataform

//#define MWI_OVERRIDE_PLTF_X86		//Initialize to a 32 bits (x86) Plataform
//#define MWI_OVERRIDE_PLTF_X64		//Initialize to a 64 bits Plataform

//System OS (Defined by the compiler)
#if defined (WIN32) || defined(_WIN32) || defined(__WIN32__) || defined (MWI_OVERRIDE_OS_WINDOWS)
	#define MWI_OS_WINDOWS
	#define MWI_UNVSAL_WINDOW_HANDLE HWND
	#define MWI_UNVSAL_APP_INSTANCE HINSTANCE
	#pragma message ("[MWI][INFO] -> Windows Detected")
	#ifndef MWI_DONT_INCLUDE_OS_BASE
		#pragma message ("[MWI][INFO] -> Windows Base included")	
		#include <Windows.h>
	#endif
//Other OS's ( !! WIP !! )
#elif defined LINUX || defined (MWI_OVERRIDE_OS_LINUX)
	#define MWI_OS_LINUX
	#define MWI_UNVSAL_WINDOW_HANDLE
	#pragma message ("[MWI][INFO] -> Linux Detected")
#elif defined OSX ||defined (MWI_OVERRIDE_OS_OSX)
	#define MWI_OS_OSX
	#define MWI_UNVSAL_WINDOW_HANDLE
	#pragma message ("[MWI][INFO] -> OSX Detected")
#else
	#ifdef MWI_ENFORCE
		#error [MWI][ERROR] -> Could not determine the host Operation System (If you want to continue anyway, undef 'MWI_ENFORCE')
	#else
		#pragma message ("[MWI][WARNING] -> Could not determine the host Operation System")
		#define MWI_OS_UNKNOWN						//Could not determine the host system. Maybe the source code isn't updated?	
	#endif
#endif

//System "bit mode"
#if defined X86 || defined (_M_IX86) || defined (_M_IX86_FP) 
	#pragma message ("[MWI][INFO] -> Compiling in 32 bits mode")
	#define MWI_PLTF_X86
#elif defined X64 || defined (_M_X64)
	#pragma message ("[MWI][INFO] -> Compiling in 64 bits mode")
	#define MWI_PLTF_X64
#else
	#ifdef MWI_ENFORCE
		#error [MWI][ERROR] -> Could not determine the system plataform (Maybe the compiler is not set-up?) (If you want to continue anyway, undef 'MWI_ENFORCE')
	#else
		#pragma message ("[MWI][WARNING] -> Could not determine the system plataform (Maybe the compiler is not set-up?)")
		#define MWI_PLTF_UNKNOWN
	#endif
#endif

#pragma message ("[MWI][INFO] -> READY")

// Copy & Paste template (for lazy people (aka, me))
//#ifdef MWI_OS_WINDOWS
//
//#elif defined MWI_OS_LINUX
//
//#elif defined MWI_OS_OSX
//
//#endif

#endif;	//Moon Wiz Igniter Header