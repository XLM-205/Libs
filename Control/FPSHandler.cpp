/*-------FPS "LU" Handler-------
*	Low Usage frame counter
*	--Frame per second calculation, display, logic and graphic frame-rate control and dynamic time-step control 
*
*
*	Moon Wiz Studios (c) - 10/01/2015
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 1.0.1
*/

//TODO: Delta for frame drop calculations

#include <GL\glut.h>
#include <sstream>
#include <time.h>

#include "FPSHandler.h"

#define FPS_LIMITER_LOW_THSL	10						//Threshold to decrease step
#define FPS_LIMITER_HIGH_THSL	20						//Threshold to increase step

uint8   DiferenceInstance	= 0;						//Alternates between 0 - 1. Used in debug mode
uint8	ClockStep			= 0;						//
uint16  CPUCap				= FPS_CAP_DEFAULT;			//Logic speed limit
uint32  FPSCurrent			= 0;						//The current FPS
uint32  FPSTiLoTotal		= 0;						//Total time spent (in ms) of the process, logic counterpart
uint32  FPSTiGpTotal		= 0;						//Total time spent (in ms) of the process, graphic counterpart
uint32  FrameCounter		= 0;						//
uint16  GpCap				= FPS_CAP_NONE;				//Graphics speed limit
uint16  GraphicClock		= 0;						//Counter for frame-rate handling	
uint16  GraphicStepCounter	= 0;						//How many times in 1 sec the graphic executed last frame?
uint16  GraphicStep			= 0;						//Specifies when the graphic should be executed
uint8   FPSDrawType			= 0;						//Draw type used to show current FPS
uint16	GraphicRendered		= 0;						//Keep track of how many frames was drawn
uint8	LimiterType			= FPS_LIMIT_MATCH;			//Limiter mode for Logic / Graphics handling
uint16	LogicRendered		= 0;						//Keep track of how many frames was computed
uint16  LogicStepCounter	= 0;						//How many times in 1 sec the logic executed last frame?
uint16  LogicClock			= 0;						//Counter for frame-rate handling
uint16  LogicStep			= 0;						//Specifies when the logic should be executed
uint16	PhysicsRendered		= 0;						//Keep track of how many frames was computed
uint16  PhysicsStepCounter	= 0;						//How many times in 1 sec the physics executed last frame?
uint16  PhysicsClock		= 0;						//Counter for frame-rate handling
uint16  PhysicsStep			= 0;						//Specifies when the physics should be executed
uint32  ResolutionClock		= 0;						//Get the time in ms for rate handling
uint32  TimeCounter			= 0;						//
GLfloat PosX				= 0;						//Position X for the counter
GLfloat PosY				= 0;						//Position Y for the counter
GLfloat PosZ				= 0;						//Position Z for the counter
GLfloat R					= 0;						//Red Color for the counter
GLfloat G					= 0;						//Green Color for the counter
GLfloat B					= 0;						//Blue Color for the counter
String8 FrameCountType		= 0;
MWGLContext *RefWindow = nullptr;
int CallDiference[2] = { 0, 0 };						//Used in debug mode
bool DrawFPS				= false;					//Draw FPS? true for yes, false for no
bool StopUpdate				= false;					//Become active if "FPS_CAP_STOP" is called in "fpsChangeLogicLimit". Stops updating logic (pause effect)
bool ShowActualFPS			= true;						//True - Show actual frames drawn (Main Graphic calls) | False - Show the amount of handler calls
bool FixTimeStep			= false;					//Try to fix the time step?
bool DebugModeActive		= false;					//If true, each second the library will print on the console data about it's actions
bool TimeStepDelta			= false;					//If true, the time step will be delta-time variable. If false (default), it will update at a fixed time step
bool ExternalWindow			= false;					//If true, then we are drawing in a externally created window (GLUT and similiars)
static char *Text;
static char *FPSDraw;

//Debug functions
static int d_timedUpdates = 0;

#ifdef WIN32	//Platform lock, since __stdcall is only for windows, but the multi platform one is WAY SLOWER
void __stdcall fpsAllowDraw(void(__cdecl *MainGraphicFunction)());
void __stdcall fpsAllowLogic(void(__cdecl *MainLogicFunction)());
void __stdcall fpsAllowPhysics(void(__cdecl *PhysicsFUnction)());
#else
void fpsAllowDraw(std::function<void(void)> MainGraphicFunction);
void fpsAllowLogic(std::function<void(void)> MainLogicFunction);
void fpsAllowPhysics(std::function<void(void)> PhysicsFunction);
#endif;

bool fpsAllowDraw(void);
bool fpsAllowLogic(void);
void fpsChangeLogicLimit(uint16 LogicCap);
void fpsCounterStart(void);
void fpsCounterDraw(uint32 DrawFrames, uint8 Type);
void fpsLockSet(uint32 Limit);
void fpsLimiter(void);									//Changes the cap's step to possibly, increase rendered frames
void fpsMainSet(GLfloat X, GLfloat Y, GLfloat Z, GLfloat Red, GLfloat Green, GLfloat Blue, char *Message, uint8 DisplayType, uint8 DrawMode, uint8 ExtraMode);
void fpsSetLimit(uint16 LogicCap, uint16 GraphicsCap, uint8 LimiterMode);
void fpsCounterReset(void);
void fpsTextSize(void);
void fpsSwitchDebug(void);
//void fpsTimeRegister(int TimeState, uint8 ProcessType);
//void fpsTimeReset(void);

fpsTimedEvent::fpsTimedEvent(uint16 Cap, uint8 Mode)
{
	sprintf(EventName, "Default");
	switch (Mode)
	{
	case FPS_TIMED_UPDATE_TIMED:
		is_timed = true;
		EventStep = Cap;
		EventSpeed = Cap;
		break;
	case FPS_TIMED_UPDATE_PER_SECOND:
	default:
		if (Cap <= 0)
		{
			EventStep = 1000 / FPS_CAP_DEFAULT;
			EventSpeed = FPS_CAP_DEFAULT;
		}
		else
		{
			EventStep = 1000 / Cap;
			EventSpeed = Cap;
		}
		break;
	}
	is_stopped = false;
}

fpsTimedEvent::fpsTimedEvent(uint16 Cap, uint8 Mode, char Name[32])
{
	sprintf(EventName, Name);
	switch (Mode)
	{
	case FPS_TIMED_UPDATE_TIMED:
		is_timed = true;
		EventStep = Cap;
		EventSpeed = Cap;
		break;
	case FPS_TIMED_UPDATE_PER_SECOND:
	default:
		if (Cap <= 0)
		{
			EventStep = 1000 / FPS_CAP_DEFAULT;
			EventSpeed = FPS_CAP_DEFAULT;
		}
		else
		{
			EventStep = 1000 / Cap;
			EventSpeed = Cap;
		}
		break;
	}
	is_stopped = false;
}

void fpsTimedEvent::setUpdateRate(uint16 Rate)
{
	if (is_timed)
	{
		EventStep = Rate;
		EventSpeed = Rate;
	}
	else
	{
		if (Rate <= 0)
		{
			EventStep = 1000 / FPS_CAP_DEFAULT;
			EventSpeed = FPS_CAP_DEFAULT;
		}
		else
		{
			EventStep = 1000 / Rate;
			EventSpeed = Rate;
		}
	}
}

void fpsTimedEvent::setUpdateMode(uint8 Mode)
{
	switch (Mode)
	{
	case FPS_TIMED_UPDATE_TIMED:
		is_timed = true;
		break;
	case FPS_TIMED_UPDATE_PER_SECOND:
	default:
		is_timed = false;
		break;
	}
}

void fpsTimedEvent::stopUpdate()
{
	is_stopped = true;
}

void fpsTimedEvent::startUpdate()
{
	is_stopped = false;
}

void fpsTimedEvent::toggleUpdate()
{
	is_stopped = false ? true : !is_stopped;
}

void fpsTimedEvent::switchDebugMode()
{
	DebugMode = !DebugMode;
}

void fpsTimedEvent::setEventName(char Name[32])
{
	sprintf(EventName, Name);
}

#ifdef WIN32
void __stdcall fpsTimedEvent::ExecuteEvent(void(__cdecl *Function)())
{
	if (is_timed)
	{
		int32 TimedCounter = clock();
		if (TimedCounter - TimedTimer >= EventStep)	//">=" so small errors like "101" (for EventStep == 100) are accepted
		{
			TimedTimer = TimedCounter;
			EventRendered++;
			Function();
		}
		else if (!TimedEngaged)
		{
			TimedTimer = TimedCounter;
			EventRendered++;
			TimedEngaged = true;
			Function();
		}
	}
	else
	{
		EventClock = clock() - ResClock - (EventRendered * EventStep);
		if ((EventClock >= EventStep) && !is_stopped)
		{
			EventRendered++;
			Function();
		}
	}

	//-----------------Independent Time Counter-----------------------
	uint32 timeComp = clock() + 1000;
	FrameCounter++;
	if (TimeCount < clock())
	{
		ResClock = clock();
		TimeCount = timeComp;

		LastUpdateRenderTotal = EventRendered;
		if (DebugMode)
		{
			if (is_timed)
			{
				printf("[D]Timed 'Timed' Event: '%s' -> Frames Updated: [%d] updates\n", EventName, EventRendered);
			}
			else
			{
				printf("[D]Timed 'Per/Second' Event: '%s' -> Frames Updated: [%d] updates\n", EventName, EventRendered);
			}
		}
		EventRendered = 0;
	}
	//-------------------------------------------------------------
}
#else
void fpsTimedEvent::ExecuteEvent(std::function<void(void)> Function)
{
	if (is_timed)
	{
		int32 TimedCounter = clock();
		if (TimedCounter - TimedTimer >= EventStep)
		{
			TimedTimer = TimedCounter;
			EventRendered++;
			Function();
		}
		else if (!TimedEngaged)
		{
			TimedTimer = TimedCounter;
			EventRendered++;
			TimedEngaged = true;
			Function();
		}
	}
	else
	{
		EventClock = clock() - ResClock - (EventRendered * EventStep);
		if ((EventClock >= EventStep) && !is_stopped)
		{
			EventRendered++;
			Function();
		}
	}

	//-----------------Independent Time Counter-----------------------
	uint32 timeComp = clock() + 1000;
	FrameCounter++;
	if (TimeCount < clock())
	{
		ResClock = clock();
		TimeCount = timeComp;

		LastUpdateRenderTotal = EventRendered;
		if (DebugMode)
		{
			printf("[D]Timed Event: '%s' -> Frames Updated: [%d] updates\n", EventName, EventRendered);
		}
		EventRendered = 0;
	}
	//-------------------------------------------------------------
}
#endif

bool fpsTimedEvent::OnCycle(void)
{
	if (is_timed)
	{
		int32 TimedCounter = clock();
		if (TimedCounter - TimedTimer >= EventStep)	//">=" so small errors like "101" (for EventStep == 100) are accepted
		{
			TimedTimer = TimedCounter;
			EventRendered++;
			Cycler = !Cycler;
		}
		else if (!TimedEngaged)
		{
			TimedTimer = TimedCounter;
			EventRendered++;
			TimedEngaged = true;
			Cycler = !Cycler;
		}
	}
	else
	{
		EventClock = clock() - ResClock - (EventRendered * EventStep);
		if ((EventClock >= EventStep) && !is_stopped)
		{
			EventRendered++;
			Cycler = !Cycler;
		}
	}

	//-----------------Independent Time Counter-----------------------
	uint32 timeComp = clock() + 1000;
	FrameCounter++;
	if (TimeCount < clock())
	{
		ResClock = clock();
		TimeCount = timeComp;

		LastUpdateRenderTotal = EventRendered;
		if (DebugMode)
		{
			if (is_timed)
			{
				printf("[D]Timed 'Timed' Event: '%s' -> Frames Updated: [%d] updates\n", EventName, EventRendered);
			}
			else
			{
				printf("[D]Timed 'Per/Second' Event: '%s' -> Frames Updated: [%d] updates\n", EventName, EventRendered);
			}
		}
		EventRendered = 0;
	}
	return Cycler;
}

bool fpsTimedEvent::OnTime(void)
{
	if (is_timed)
	{
		int32 TimedCounter = clock();
		if (TimedCounter - TimedTimer >= EventStep)	//">=" so small errors like "101" (for EventStep == 100) are accepted
		{
			TimedTimer = TimedCounter;
			EventRendered++;
			Cycler = !Cycler;
		}
		else if (!TimedEngaged)
		{
			TimedTimer = TimedCounter;
			EventRendered++;
			TimedEngaged = true;
			Cycler = !Cycler;
		}
	}
	else
	{
		EventClock = clock() - ResClock - (EventRendered * EventStep);
		if ((EventClock >= EventStep) && !is_stopped)
		{
			EventRendered++;
			Cycler = !Cycler;
		}
	}

	//-----------------Independent Time Counter-----------------------
	uint32 timeComp = clock() + 1000;
	FrameCounter++;
	if (TimeCount < clock())
	{
		ResClock = clock();
		TimeCount = timeComp;

		LastUpdateRenderTotal = EventRendered;
		if (DebugMode)
		{
			if (is_timed)
			{
				printf("[D]Timed 'Timed' Event: '%s' -> Frames Updated: [%d] updates\n", EventName, EventRendered);
			}
			else
			{
				printf("[D]Timed 'Per/Second' Event: '%s' -> Frames Updated: [%d] updates\n", EventName, EventRendered);
			}
		}
		if (EventRendered != 0)
		{
			EventRendered = 0;
			return true;
		}
	}
	if (EventRendered != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void fpsSwitchDebug(void)
{
	DebugModeActive = !DebugModeActive;
}

bool fpsAllowLogic(void)
{
	LogicClock = clock() - ResolutionClock - (LogicRendered * LogicStep);
	if ((LogicClock >= LogicStep) && !StopUpdate)		//If the time in ms is over the LogicStep, then update a frame
	{
		LogicRendered++;
		return true;
	}
	return false;
}

bool fpsAllowLogicChild(void)
{
	LogicClock = clock() - ResolutionClock - (LogicRendered * LogicStep);
	if ((LogicClock >= LogicStep) && !StopUpdate)		//If the time in ms is over the LogicStep, then update a frame
	{
		return true;
	}
	return false;
}

bool fpsAllowDraw(void)
{
	GraphicClock = clock() - ResolutionClock - (GraphicRendered * GraphicStep);
	if ((GraphicClock >= GraphicStep) && !StopUpdate)
	{
		GraphicRendered++;
		return true;
	}
	return false;
}

bool fpsAllowDrawChild(void)
{
	GraphicClock = clock() - ResolutionClock - (GraphicRendered * GraphicStep);
	if ((GraphicClock >= GraphicStep) && !StopUpdate)
	{
		return true;
	}
	return false;
}

#ifdef WIN32
void __stdcall fpsAllowDraw(void(__cdecl *MainGraphicFunction)())
{
	GraphicClock = clock() - ResolutionClock - (GraphicRendered * GraphicStep);
	if ((GraphicClock >= GraphicStep) && !StopUpdate)
	{
		GraphicRendered++;
		MainGraphicFunction();
	}
}

void __stdcall fpsAllowDrawChild(void(__cdecl *MainGraphicFunction)())
{
	GraphicClock = clock() - ResolutionClock - (GraphicRendered * GraphicStep);
	if ((GraphicClock >= GraphicStep) && !StopUpdate)
	{
		MainGraphicFunction();
	}
}

void __stdcall fpsAllowLogic(void(__cdecl *MainLogicFunction)())
{
	LogicClock = clock() - ResolutionClock - (LogicRendered * LogicStep);
	if ((LogicClock >= LogicStep) && !StopUpdate)
	{
		LogicRendered++;
		MainLogicFunction();
	}
}

void __stdcall fpsAllowLogicChild(void(__cdecl *MainLogicFunction)())
{
	LogicClock = clock() - ResolutionClock - (LogicRendered * LogicStep);
	if ((LogicClock >= LogicStep) && !StopUpdate)
	{
		MainLogicFunction();
	}
}

void __stdcall fpsAllowPhysics(void(__cdecl *PhysicsFunction)())
{
	PhysicsClock = clock() - ResolutionClock - (PhysicsRendered * PhysicsStep);
	if ((PhysicsClock >= PhysicsStep) && !StopUpdate)
	{
		PhysicsRendered++;
		PhysicsFunction();
	}
}

void __stdcall fpsAllowPhysicsChild(void(__cdecl *PhysicsFunction)())
{
	PhysicsClock = clock() - ResolutionClock - (PhysicsRendered * PhysicsStep);
	if ((PhysicsClock >= PhysicsStep) && !StopUpdate)
	{
		PhysicsFunction();
	}
}
#else
void fpsAllowDraw(std::function<void(void)> MainGraphicFunction)
{
	GraphicClock = clock() - ResolutionClock - (GraphicRendered * GraphicStep);
	if ((GraphicClock >= GraphicStep) && !StopUpdate)
	{
		GraphicRendered++;
		MainGraphicFunction();
	}
}

void fpsAllowDrawChild(std::function<void(void)> MainGraphicFunction)
{
	GraphicClock = clock() - ResolutionClock - (GraphicRendered * GraphicStep);
	if ((GraphicClock >= GraphicStep) && !StopUpdate)
	{
		MainGraphicFunction();
	}
}

void fpsAllowLogic(std::function<void(void)> MainLogicFunction)
{
	LogicClock = clock() - ResolutionClock - (LogicRendered * LogicStep);
	if ((LogicClock >= LogicStep) && !StopUpdate)		//If the time in ms is over the LogicStep, then, update a frame
	{
		LogicRendered++;
		MainLogicFunction();
	}
}

void fpsAllowLogicChild(std::function<void(void)> MainLogicFunction)
{
	LogicClock = clock() - ResolutionClock - (LogicRendered * LogicStep);
	if ((LogicClock >= LogicStep) && !StopUpdate)		//If the time in ms is over the LogicStep, then, update a frame
	{
		MainLogicFunction();
	}
}

#endif;

void fpsCounterDraw(uint32 DrawFrames)
{
	int length;
	if (FPSDrawType == FPS_TYPE_WINDOW)
	{
		sprintf(FPSDraw, "%s | FPS - %d %s", Text, DrawFrames, FrameCountType.getString());
		if (ExternalWindow)
		{
			RefWindow->setTitle(FPSDraw);
		}
	}
	else if (FPSDrawType == FPS_TYPE_SCREEN)
	{
		sprintf(FPSDraw, "%s%d %s", Text, DrawFrames, FrameCountType.getString());
		length = strlen(FPSDraw);
		glColor3f(R, G, B);
		glRasterPos3f(PosX, PosY, PosZ);
		for (int i = 0; i < length; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, FPSDraw[i]);
		}
	}
	else
	{
		FPSDraw = "FPS INVALID RENDER TYPE";
		length = strlen(FPSDraw);
		glColor3f(1, 0, 0);
		glRasterPos3f(0, 0, 0);
		for (int i = 0; i < length; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, FPSDraw[i]);
		}
	}
}

void fpsCounterStart(void)
{
	//Using time() is not precise, but save some CPU (practically, 100% more CPU, but, time-step is 50% slower)
	uint32 timeComp = clock() + 1000;
	FrameCounter++;
	if (TimeCounter < clock())
	{
		TimeCounter = ResolutionClock = clock();		//1 second passed! Save this value for frame handling
		TimeCounter += 1000;
		if (!ShowActualFPS)
		{
			FPSCurrent = FrameCounter;
		}
		else
		{
			FPSCurrent = GraphicRendered;
		}
		if (FPSDrawType == FPS_TYPE_WINDOW && DrawFPS)
		{
			fpsCounterDraw(FPSCurrent);
			if (DiferenceInstance == 0)
			{
				CallDiference[0] = FrameCounter;
				DiferenceInstance = 1;
			}
			else
			{
				CallDiference[1] = FrameCounter;
				DiferenceInstance = 0;
			}
			if (DebugModeActive)
			{ 
				static char *UpdMethod = TimeStepDelta ? "Delta" : "Fixed";
				static uint32 ResolutionOld = 0;
				float Second = (float)clock() * 0.001, Minute = (float)clock() / 60000, Hour = (float)clock() / 3600000;
				printf("Program Time: \t\t\t[%d] ms | %.2f s | %.2f m | %.2f h\nCalls on last iteration: \t[%d] calls\nCall difference: \t\t[%d] calls\nResolution Delay: \t\t[%d] ms\nGL Frame Rate: \t\t\t[%d] Frames/s\nCPU Lock: \t\t\t[%d] Frames/s\nGL Lock: \t\t\t[%d] Frames/s\nFPS Update Type:\t\t[%s]\n--------------End of FPS-LU Cycle--------------\n", clock(), Second, Minute, Hour, FrameCounter, CallDiference[0] - CallDiference[1], ResolutionClock - ResolutionOld, GraphicRendered, CPUCap, GpCap, UpdMethod);
				ResolutionOld = ResolutionClock + 1000;
			}
		}
		FrameCounter = 0;
		if (FixTimeStep)
		{
			fpsLimiter();
		}
		fpsCounterReset();
	}
	if (FPSDrawType == FPS_TYPE_SCREEN && DrawFPS)
	{
		fpsCounterDraw(FPSCurrent);
	}
}

#ifndef FPS_EXT_WINDOW
void fpsMainSet(GLfloat X, GLfloat Y, GLfloat Z, GLfloat Red, GLfloat Green, GLfloat Blue, char *Message, uint8 DisplayType, uint8 DrawMode, uint8 ExtraMode, MWGLContext *TGTWindow)
{
	PosX = X;
	PosY = Y;
	PosZ = Z;
	R = Red;
	G = Green;
	B = Blue;
	Text = Message;
	FPSDrawType = DisplayType;
	ExternalWindow = true;
	if (DrawMode == FPS_DRAW)
	{
		//We gonna draw the FPS? Yes. Then malloc some memory for the counter
		DrawFPS = true;
		fpsTextSize();
	}
	else
	{
		DrawFPS = false;
	}
	if (ExtraMode == FPS_COUNT_ACTUAL_REND || ExtraMode == NULL)
	{
		ShowActualFPS = true;
		FrameCountType = "FRAMES";
	}
	else
	{
		ShowActualFPS = false;
		FrameCountType = "CALLS";
	}
	RefWindow = TGTWindow;
}
#endif

void fpsMainSet(GLfloat X, GLfloat Y, GLfloat Z, GLfloat Red, GLfloat Green, GLfloat Blue, char *Message, uint8 DisplayType, uint8 DrawMode, uint8 ExtraMode)
{
	PosX = X;
	PosY = Y;
	PosZ = Z;
	R = Red;
	G = Green;
	B = Blue;
	Text = Message;
	FPSDrawType = DisplayType;
	if (DrawMode == FPS_DRAW)
	{
		//We gonna draw the FPS? Yes. Then malloc some memory for the counter
		DrawFPS = true;
		fpsTextSize();
	}
	else
	{
		DrawFPS = false;
	}
	if (ExtraMode == FPS_COUNT_ACTUAL_REND || ExtraMode == NULL)
	{
		ShowActualFPS = true;
		FrameCountType = "FRAMES";
	}
	else
	{
		ShowActualFPS = false;
		FrameCountType = "CALLS";
	}
}

void fpsSetLimit(uint16 LogicCap, uint16 GraphicsCap, uint8 LimiterMode)
{
	CPUCap = LogicCap;
	GpCap = GraphicsCap;
	LimiterType = LimiterMode;
	if (LimiterType == FPS_LIMIT_NO)
	{
		FixTimeStep = false;
	}
	else
	{
		FixTimeStep = true;
	}
	if (CPUCap <= 0)
	{
		CPUCap = FPS_CAP_DEFAULT;
		LogicStep = 1000 / FPS_CAP_DEFAULT;
	}
	else
	{
		LogicStep = 1000 / CPUCap;
	}
	if (GpCap <= 0)
	{
		GpCap = FPS_CAP_NONE;
		GraphicStep = 0;			//If not set, then draw frames as fast as possible
	}
	else
	{
		GraphicStep = 1000 / GpCap;
	}
}

void fpsSetCountMode(uint8 CountMode)
{
	if (CountMode == FPS_COUNT_CALL_AMOUNT)
	{
		ShowActualFPS = false;
		FrameCountType = "CALLS";
	}
	else
	{
		ShowActualFPS = true;
		FrameCountType = "FRAMES";
	}
}

void fpsSwitchCountMode(void)
{
	ShowActualFPS = !ShowActualFPS;
	if (ShowActualFPS)
	{
		FrameCountType = "FRAMES";
	}
	else
	{
		FrameCountType = "CALLS";
	}
}

void fpsLimiter(void)
{
	uint16 NewStepTemp = 0;
	if (LimiterType == FPS_LIMIT_SLOW_LOGIC)											//Slow logic to increase graphic. Will cause slow response
	{
		if (GraphicRendered < GpCap - FPS_LIMITER_LOW_THSL && GraphicRendered > 0)		//Houston, we are drawing less frames than required! Decrease the time step!!
		{
			NewStepTemp = GpCap / GraphicRendered;
			GraphicStep /= NewStepTemp;
			if (LogicStep * NewStepTemp > 65535)		//uint16 Overflow Lock
			{
				LogicStep = 65535;
			}
			else
			{
				LogicStep *= NewStepTemp;
			}
		}
	}
	else if (LimiterType == FPS_LIMIT_SLOW_GRAPHIC)										//Slow graphic to increase logic. May cause shuttering
	{
		if (LogicRendered < CPUCap - FPS_LIMITER_LOW_THSL && LogicRendered > 0)
		{
			NewStepTemp = CPUCap / LogicRendered;
			LogicRendered /= NewStepTemp;
			if (GraphicStep * NewStepTemp > 65535)		//uint16 Overflow Lock
			{
				GraphicStep = 65535;
			}
			else
			{
				GraphicStep *= NewStepTemp;
			}
		}
	}
}

void fpsCounterReset(void)
{
	LogicStepCounter = 0;
	GraphicStepCounter = 0;
	GraphicRendered = 0;
	LogicRendered = 0;
}

void fpsTextSize(void)
{
	int i = 0;
	for (i = 0; Text[i] != '\0'; i++)
	{

	}
	i += CST_DIGITS_INT32;
	if (FPSDrawType == FPS_TYPE_SCREEN)
	{
		FPSDraw = (char*)malloc(sizeof(char) * i);
	}
	else
	{
		FPSDraw = (char*)malloc(sizeof(char) * (i + 8));
	}
}

//void fpsTimeRegister(uint8 TimeState, uint8 ProcessType)
//{
//	uint32  FPSTiStart = 0;						//Start time (in ms) of the process
//	uint32  FPSTiStop = 0;						//End time (in ms) of the process
//	if (ProcessType == FPS_PROCESS_IS_LOGIC)
//	{
//		if (TimeState == FPS_TIME_START)
//		{
//			FPSTiStart = clock();
//		}
//		else
//		{
//			FPSTiStop = clock();
//			FPSTiLoTotal = (FPSTiStop - FPSTiStart);
//			LogicStepCounter++;
//		}
//	}
//	else
//	{
//		if (TimeState == FPS_TIME_START)
//		{
//			FPSTiStart = clock();
//		}
//		else
//		{
//			FPSTiStop = clock();
//			FPSTiGpTotal = (FPSTiStop - FPSTiStart);
//			GraphicStepCounter++;
//		}
//	}
//}

//void fpsTimeReset(uint8 Timer)
//{
//	if (Timer == FPS_PROCESS_IS_LOGIC)
//	{
//		FPSTiLoTotal = 0;
//	}
//	else
//	{
//		FPSTiGpTotal = 0;
//	}
//}

int fpsGetFPS(void)
{
	return FPSCurrent;
}

void fpsChangeLogicLimit(int16 LogicCap)
{
	if (LogicCap <= FPS_CAP_STOP)
	{
		if (LogicCap <= 0)
		{
			CPUCap = FPS_CAP_DEFAULT;
			LogicStep = 1000 / FPS_CAP_DEFAULT;
		}
		StopUpdate = true;
	}
	else
	{
		CPUCap = LogicCap;
		LogicStep = 1000 / CPUCap;
		StopUpdate = false;
	}
}