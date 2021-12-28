/*-------FPS "LU" Handler-------
*	Low Usage framerate counter
*	--Frame per second calculation, display, logic and graphic framerate control and dynamic time-step control 
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
*	Version 2.0.8
*/

//TODO: Delta for frame drop calculations

#ifndef FPS_HNDLR_H_
#define FPS_HNDLR_H_

#include "CustomTypes-SA.h"

#ifndef FPS_EXT_WINDOW
#include "..\Graphics\GLStarter.h"
#endif

#ifndef WIN32
#include <functional>
#endif

//Extra modes
#define FPS_NO_DRAW					0		//Don't draw the fps counter, anywhere
#define FPS_DRAW					1		//Draw the fps counter (FPS_TYPE.. dependent)
#define FPS_COUNT_ACTUAL_REND		2		//Counts the actual number of frames drawn (Graphics)
#define FPS_COUNT_CALL_AMOUNT		3		//DEBUG - Counts the number of times the handler was called
#define FPS_TIMED_UPDATE_TIMED		0		//Timed Event: Execute the event at every X miliseconds
#define FPS_TIMED_UPDATE_PER_SECOND	1		//Timed Event: Execute the event X times per second (Default)
#define FPS_TIMED_UPDATE_HOLD		2		//Timed Event: Execute the event upon "ExecuteEvent" call and hold it until X miliseconds have passed
//Speed Limit (Cap.)
#define FPS_CAP_DEFAULT				60		//Default FPS cap. If not set, use this instead
#define FPS_CAP_NONE				0		//Capless FPS
#define FPS_CAP_STOP			   -1		//Stop updating. Useful for pause states. Should only be called in "fpsChangeLogicLimit"
//Draw type: Window draws FPS on the window title. Screen, as rendered text on screen
#define FPS_TYPE_SCREEN				0		//Display FPS as screen text
#define FPS_TYPE_WINDOW				1		//Display FPS as window title
//Time handling macros
#define FPS_TIME_START				1		//Start the time recording
#define FPS_TIME_STOP				0		//Stop and records total time spent
//Define what type of process will be execute (for use with "fpsTimeRegister(...)")
#define FPS_PROCESS_IS_LOGIC		1		//The current process is the logic process
#define FPS_PROCESS_IS_GRAPHIC		2		//The current process is the graphic process
//Define the speed limit modes
#define FPS_LIMIT_MATCH				0		//Make the CPU limit and graphic limit match as much as possible
#define FPS_LIMIT_SLOW_LOGIC		1		//Priority on graphics: make the logic execute less times, to execute more graphic (Cap. dependent)
#define FPS_LIMIT_SLOW_GRAPHIC		2		//Priority on logic: make the graphic execute less times, to execute more logic (Cap. dependent)
#define FPS_LIMIT_NO				3		//NO priority. The handler will not try to fix the time steps (Recommended)
//Time Step Configuration
#define FPS_TIMESTEP_FIXED			true	//If selected, the controller will update every frame, logic and graphic, at a fixed time, related to the amount of frames per second requested (default)
#define FPS_TIMESTEP_VARIABLE		false	//If selected, the controller will update every frame, logic and graphic, at a variable time, using the delta time of it's last interaction to compute it's next update time

//Starts the FPS counter procedure
void fpsCounterStart(void);
//Allows the render engine to draw another frame
#ifdef WIN32
void __stdcall fpsAllowDraw(void(__cdecl *MainGraphicFunction)());
void __stdcall fpsAllowDrawChild(void(__cdecl *MainGraphicFunction)());//Don't increment total rendered (Should be used to run a "child" function ONLY)
#else
void fpsAllowDraw(std::function<void(void)> MainGraphicFunction);
void fpsAllowDrawChild(std::function<void(void)> MainGraphicFunction);	//Don't increment total rendered (Should be used to run a "child" function ONLY)(Side effects can occur)
#endif;
bool fpsAllowDraw(void);
bool fpsAllowDrawChild(void);	//Don't increment total rendered (Should be used to run a "child" function ONLY)(Side effects can occur)
//Allows the logic engine do to stuff
#ifdef WIN32
void __stdcall fpsAllowLogic(void(__cdecl *MainLogicFunction)());
void __stdcall fpsAllowLogicChild(void(__cdecl *MainLogicFunction)());	//Don't increment total rendered (Should be used to run a "child" function ONLY)(Side effects can occur)
#else
void fpsAllowLogic(std::function<void(void)> MainLogicFunction);
void fpsAllowLogicChild(std::function<void(void)> MainLogicFunction);	//Don't increment total rendered (Should be used to run a "child" function ONLY)(Side effects can occur)
#endif;
bool fpsAllowLogic(void);
bool fpsAllowLogicChild(void);	//Don't increment total rendered (Should be used to run a "child" function ONLY)(Side effects can occur)
//Allows the physics engine to do stuff
#ifdef WIN32
void __stdcall fpsAllowPhysics(void(__cdecl *PhysicsFunction)());
void __stdcall fpsAllowPhysicsChild(void(__cdecl *PhysicsFunction)());	//Don't increment total rendered (Should be used to run a "child" function ONLY)(Side effects can occur)
#else
void fpsAllowPhysics(std::function<void(void)> PhysicsFunction);
void fpsAllowPhysicsChild(std::function<void(void)> PhysicsFunction);	//Don't increment total rendered (Should be used to run a "child" function ONLY)(Side effects can occur)
#endif;
bool fpsAllowPhysics(void);
bool fpsAllowPhysicsChild(void);	//Don't increment total rendered (Should be used to run a "child" function ONLY)(Side effects can occur)

//Change the actual logic cap, if you want to speed up or down the application
void fpsChangeLogicLimit(int16 LogicCap);
//Gets the FPS count
int fpsGetFPS(void);
//Calculates total time spent during procedures
//void fpsTimeRegister(uint8 TimeState, uint8 ProcessType);
//Sets the position, color and message for the FPS counter
#ifndef FPS_EXT_WINDOW
	void fpsMainSet(float X, float Y, float Z, float Red, float Green, float Blue, char *Message, uint8 DisplayType, uint8 DrawMode, uint8 CountMode, MWGLContext *RefWindow);
#endif
void fpsMainSet(float X, float Y, float Z, float Red, float Green, float Blue, char *Message, uint8 DisplayType, uint8 DrawMode, uint8 CountMode);
//Sets the speed limit for the application
void fpsSetLimit(uint16 LogicCap, uint16 GraphicsCap, uint8 LimiterMode);
//Switch between debug and normal mode
void fpsSwitchDebug(void);
//Change current count mode. If invalid value supplied, defaults to 'FPS_COUNT_ACTUAL_REND'
void fpsSetCountMode(uint8 CountMode);
//Switch current count mode
void fpsSwitchCountMode(void);
//Free Memory allocated
void fpsClear(void);

class fpsTimedEvent
{
private:
	bool is_stopped = true;										//Becomes false when constructed. If true, prevent executing function call ("ExecuteEvent()")
	bool TimedEngaged = false;									//(Fix) if true, then the timed chain is enganged. If false (default) especial code will ensure the chain engagement
	bool DebugMode = false;										//If true, updates will be printed on the console
	bool Cycler = false;										//Will cycle between true and false ONLY when the event reachs it's EventStep
	uint8 is_timed = 0;										//If 0 (default) the event will be run X times per second, 1, will be executed after X miliseconds and 2, execute continously until X milisecodnds have passed
	uint16 EventSpeed = 0;
	uint16 EventClock = 0;
	uint16 EventStep = 0;
	uint16 EventRendered = 0;
	uint16 LastUpdateRenderTotal = 0;							//Stores the total of updates between seconds
	//Specifics: This members are class independent versions of the already used time counters
	uint16 ResClock = 0;
	uint32 TimeCount = 0;
	int32 TimedTimer = 0;
	char EventName[32];
public:
	fpsTimedEvent(uint16 Time, uint8 Mode);						//If no valid Time value as given (<=0), use the default FPS CAP ("FPS_CAP_DEFAULT")
	fpsTimedEvent(uint16 Time, uint8 Mode, char Name[32]);		//If no valid Time value as given (<=0), use the default FPS CAP ("FPS_CAP_DEFAULT") (Name constructor)
	void setUpdateRate(uint16 NewRate);							//Changes the update rate (Cap) of the timed event
	void setUpdateMode(uint8 NewMode);							//Changes the update mode between FPS_TIMED_UPDATE_TIMED or FPS_TIMED_UPDATE_PER_SECOND
	void switchMode();											//Switch between each mode. No input required
	void stopUpdate();											//Stops updating the event. Prevent the execution of "ExecuteEvent"
	void startUpdate();											//Starts updating the event
	void toggleUpdate();										//Toggles the timed event. If it's stopped, start it, if already started, stop it
	void switchDebugMode();										//Toggles between debug (console info printed) or normal mode
	void setEventName(char Name[32]);							//Set the current event name (max of 32 characters)
#ifdef WIN32
	void __stdcall ExecuteEvent(void(__cdecl *Function)());		//Executes the event, if the required time has passed, also return true, if an event was execute that frame, and false if isn't
#else
	void ExecuteEvent(std::function<void(void)> Function);		//Executes the event, if the required time has passed, also return true, if an event was execute that frame, and false if isn't
#endif;	
	bool OnCycle(void);											//When called, will switch it's state from true to false, when the event have reached it's step
	bool OnTime(void);											//When called, will return true if the event have reached it's step, and false when not
	int getEventUpdateSpeed(void) { return EventSpeed; };
	int getTotalRendered(void) { return LastUpdateRenderTotal; };
};

#endif;