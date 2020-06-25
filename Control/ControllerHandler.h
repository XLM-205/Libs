/*-------Xbox Controller Handler-------
*	Handles XBOX Controller inputs
*	--Independent control support, force feedback and battery reading
*
*
*	Copyright (c) 2015 Moon Wiz Studios
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 1.5
*/

#ifndef _H_MWCONTROLLERHANDLER_
#define _H_MWCONTROLLERHANDLER_
//Moon Wiz Controller Handler -> A.K.A MWCH
//Flag for controller debugging
//#define MWCH_DEBUG

#define WIN32_LEAN_AND_MEAN	//Doesn't include some headers from the Win32 header (faster build / less header size)

#include <Windows.h>
#include <Xinput.h>
#include "..\Maths\Vec2d.h"

#ifdef MWCH_DEBUG
	#include "CustomTypes-SA.h"
#endif

//Controller Mappings
enum XButton
{
	DUP =		XINPUT_GAMEPAD_DPAD_UP,			//Digital Up
	DDOWN =		XINPUT_GAMEPAD_DPAD_DOWN,		//Digital Down
	DLEFT =		XINPUT_GAMEPAD_DPAD_LEFT,		//Digital Left
	DRIGHT =	XINPUT_GAMEPAD_DPAD_RIGHT,		//Digital Right
	START =		XINPUT_GAMEPAD_START,			//Start
	BACK =		XINPUT_GAMEPAD_BACK,			//Back/Select
	LS =		XINPUT_GAMEPAD_LEFT_THUMB,		//Left Stick/Analog Button (L3)
	RS =		XINPUT_GAMEPAD_RIGHT_THUMB,		//Right Stick/Analog Button (R3)
	LB =		XINPUT_GAMEPAD_LEFT_SHOULDER,	//Left Bumper (L1)
	RB =		XINPUT_GAMEPAD_RIGHT_SHOULDER,	//Right Bumper (R1)
	GUIDE =		0x0400,							//'Central' Button
	SYNC =		0x0800,							//Wireless sync button
	A =			XINPUT_GAMEPAD_A,				//A Button (Cross)
	B =			XINPUT_GAMEPAD_B,				//B Button (Circle) 
	X =			XINPUT_GAMEPAD_X,				//X Button (Square)
	Y =			XINPUT_GAMEPAD_Y				//Y Button (Triangle)
};

class DeadZones
{
private:
	Vec2<uint16> LSDeadZone;			//The dead zone for the left stick (default is XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	Vec2<uint16> RSDeadZone;			//The dead zone for the right stick (default is XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	uint8 LTDeadZone;					//LT Dead Zone
	uint8 RTDeadZone;					//RT Dead Zone

public:
	DeadZones()
	{
		LSDeadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		RSDeadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		LTDeadZone = RTDeadZone = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	}
	DeadZones(uint16 LSX, uint16 LSY, uint16 RSX, uint16 RSY, uint8 LT, uint8 RT)
	{
		LSDeadZone.setValues((LSX > CST_INT16_MAX ? CST_INT16_MAX : LSX), (LSY > CST_INT16_MAX ? CST_INT16_MAX : LSY));
		RSDeadZone.setValues((RSX > CST_INT16_MAX ? CST_INT16_MAX : RSX), (RSY > CST_INT16_MAX ? CST_INT16_MAX : RSY));
		LTDeadZone = LT;
		RTDeadZone = RT;
	}

	Vec2<uint16> getLSDZ(void)
	{
		return LSDeadZone;
	}
	uint8 getLTDZ(void)
	{
		return LTDeadZone;
	}
	Vec2<uint16> getRSDZ(void)
	{
		return RSDeadZone;
	}
	uint8 getRTDZ(void)
	{
		return RTDeadZone;
	}

	void setLSDZ(const uint16 lsdz)
	{
		LSDeadZone = lsdz > CST_INT16_MAX ? CST_INT16_MAX : lsdz;
	}
	void setLTDZ(const uint8 ltdz)
	{
		LTDeadZone = ltdz;
	}
	void setRSDZ(const uint16 rsdz)
	{
		RSDeadZone = rsdz > CST_INT16_MAX ? CST_INT16_MAX : rsdz;
	}
	void setRTDZ(const uint8 rtdz)
	{
		RTDeadZone = rtdz;
	}

	void setLSDZ_X(const int16 lsdz)
	{
		LSDeadZone._X = lsdz > CST_INT16_MAX ? CST_INT16_MAX : lsdz;
	}
	void setLSDZ_Y(const int16 lsdz)
	{
		LSDeadZone._Y = lsdz > CST_INT16_MAX ? CST_INT16_MAX : lsdz;
		if(LSDeadZone._Y > CST_INT16_MAX)
		{
			LSDeadZone._Y = CST_INT16_MAX;
		}
	}
	void setRSDZ_X(const int16 rsdz)
	{
		RSDeadZone._X = rsdz > CST_INT16_MAX ? CST_INT16_MAX : rsdz;
	}
	void setRSDZ_Y(const int16 rsdz)
	{
		RSDeadZone._Y = rsdz > CST_INT16_MAX ? CST_INT16_MAX : rsdz;
	}
};

class AnalogicData
{
private:
	DeadZones m_DZ;		//Represent the deadzones for all analog input (sticks and triggers)
	mwVec2d LS;			//Left Stick RAW
	mwVec2d RS;			//Right Stick RAW
	double LT;			//Left Trigger
	double RT;			//Right Trigger

public:
	AnalogicData()
	{
		LS = 0;
		RS = 0;
		LT = RT = 0;
	}

	mwVec2d getStickLeft(void)
	{
		return LS;
	}
	mwVec2d getStickRight(void)
	{
		return RS;
	}
	double getTriggerLeft(void)
	{
		return LT;
	}
	double getTriggerRight(void)
	{
		return RT;
	}

	AnalogicData getRaw(const XINPUT_GAMEPAD &ctrl)
	{
		double MoveMag = 0.0;
		AnalogicData Raw;
		Raw.LS._X = ctrl.sThumbLX;
		Raw.LS._Y = ctrl.sThumbLY;
		Raw.RS._X = ctrl.sThumbRX;
		Raw.RS._Y = ctrl.sThumbRY;
		Raw.LT = ctrl.bLeftTrigger;
		Raw.RT = ctrl.bRightTrigger;
		MoveMag = Raw.LS.getLength();
		if (MoveMag > CST_INT16_MAX)
		{
			MoveMag = CST_INT16_MAX;
		}
		Raw.LS /= MoveMag;
		MoveMag = MoveMag / CST_INT16_MAX;
		Raw.LS *= MoveMag;

		MoveMag = Raw.RS.getLength();
		if (MoveMag > CST_INT16_MAX)
		{
			MoveMag = CST_INT16_MAX;
		}
		Raw.RS /= MoveMag;
		MoveMag = MoveMag / CST_INT16_MAX;
		Raw.RS *= MoveMag;
		Raw.LT = (float)(ctrl.bLeftTrigger) / CST_UINT8_MAX;
		Raw.RT = (float)(ctrl.bRightTrigger) / CST_UINT8_MAX;
		return Raw;
	}

	//Get the raw value, update analog data and put it in scale
	void updateValuesCircular(const XINPUT_GAMEPAD &ctrl)
	{
		//Sticks
		double MoveMag = 0.0;
		LS._X = ctrl.sThumbLX;
		LS._Y = ctrl.sThumbLY;
		RS._X = ctrl.sThumbRX;
		RS._Y = ctrl.sThumbRY;
		MoveMag = LS.getLength();		//The real distance the stick moved
		if (MoveMag > CST_INT16_MAX)
		{
			MoveMag = CST_INT16_MAX;
		}

		if (MoveMag > m_DZ.getLSDZ()._X)
		{
			LS /= MoveMag;
			MoveMag = (MoveMag - m_DZ.getLSDZ()._X) / (CST_INT16_MAX - m_DZ.getLSDZ()._X);
			LS *= MoveMag;
		}
		else
		{
			LS = 0;
		}

		MoveMag = RS.getLength();
		if (MoveMag > CST_INT16_MAX)
		{
			MoveMag = CST_INT16_MAX;
		}
		if (MoveMag > m_DZ.getRSDZ()._X)
		{
			RS /= MoveMag;
			MoveMag = (MoveMag - m_DZ.getRSDZ()._X) / (CST_INT16_MAX - m_DZ.getRSDZ()._X);
			RS *= MoveMag;
		}
		else
		{
			RS = 0;
		}

		//Triggers
		if (ctrl.bLeftTrigger < m_DZ.getLTDZ())
		{
			LT = 0;
		}
		else
		{
			LT = (float)(ctrl.bLeftTrigger - m_DZ.getLTDZ()) / (CST_UINT8_MAX - m_DZ.getLTDZ());
		}
		if (ctrl.bRightTrigger < m_DZ.getRTDZ())
		{
			RT = 0;
		}
		else
		{
			RT = (float)(ctrl.bRightTrigger - m_DZ.getRTDZ()) / (CST_UINT8_MAX - m_DZ.getRTDZ());
		}
	}
	void updateValuesCustom(const XINPUT_GAMEPAD &ctrl)
	{
		//Sticks
		if (ctrl.sThumbLX > -m_DZ.getLSDZ()._X && ctrl.sThumbLX < m_DZ.getLSDZ()._X)
		{
			LS._X = 0;
		}
		else
		{
			if (ctrl.sThumbLX < 0)
			{
				LS._X = (float)(ctrl.sThumbLX + m_DZ.getLSDZ()._X) / (CST_INT16_MAX - m_DZ.getLSDZ()._X);
			}
			else
			{
				LS._X = (float)(ctrl.sThumbLX - m_DZ.getLSDZ()._X) / (CST_INT16_MAX - m_DZ.getLSDZ()._X);
			}
		}
		if (ctrl.sThumbLY > -m_DZ.getLSDZ()._Y && ctrl.sThumbLY < m_DZ.getLSDZ()._Y)
		{
			LS._Y = 0;
		}
		else
		{
			if (ctrl.sThumbLY < 0)
			{
				LS._Y = (float)(ctrl.sThumbLY + m_DZ.getLSDZ()._Y) / (CST_INT16_MAX - m_DZ.getLSDZ()._Y);
			}
			else
			{
				LS._Y = (float)(ctrl.sThumbLY - m_DZ.getLSDZ()._Y) / (CST_INT16_MAX - m_DZ.getLSDZ()._Y);
			}
		}
		if (ctrl.sThumbRX > -m_DZ.getRSDZ()._X && ctrl.sThumbRX < m_DZ.getRSDZ()._X)
		{
			RS._X = 0;
		}
		else
		{
			if (ctrl.sThumbRX < 0)
			{
				RS._X = (float)(ctrl.sThumbRX + m_DZ.getRSDZ()._X) / (CST_INT16_MAX - m_DZ.getRSDZ()._X);
			}
			else
			{
				RS._X = (float)(ctrl.sThumbRX - m_DZ.getRSDZ()._X) / (CST_INT16_MAX - m_DZ.getRSDZ()._X);
			}
		}
		if (ctrl.sThumbRY > -m_DZ.getRSDZ()._Y && ctrl.sThumbRY < m_DZ.getRSDZ()._Y)
		{
			RS._Y = 0;
		}
		else
		{
			if (ctrl.sThumbRY < 0)
			{
				RS._Y = (float)(ctrl.sThumbRY + m_DZ.getRSDZ()._Y) / (CST_INT16_MAX - m_DZ.getRSDZ()._Y);
			}
			else
			{
				RS._Y = (float)(ctrl.sThumbRY - m_DZ.getRSDZ()._Y) / (CST_INT16_MAX - m_DZ.getRSDZ()._Y);
			}
		}

		//Triggers
		if (ctrl.bLeftTrigger < m_DZ.getLTDZ())
		{
			LT = 0;
		}
		else
		{
			LT = (float)(ctrl.bLeftTrigger - m_DZ.getLTDZ()) / (CST_UINT8_MAX - m_DZ.getLTDZ());
		}
		if (ctrl.bRightTrigger < m_DZ.getRTDZ())
		{
			RT = 0;
		}
		else
		{
			RT = (float)(ctrl.bRightTrigger - m_DZ.getRTDZ()) / (CST_UINT8_MAX - m_DZ.getRTDZ());
		}
	}

	DeadZones* getDeadzones(void)
	{
		return &m_DZ;
	}
};

class XBOXController
{
protected:
	XINPUT_STATE m_ControllerState;			//The controller state (Analogs, triggers, buttons, you name it)
	AnalogicData m_Analogs;					//Represent all analogic data already clamped (Triggers and Sticks)
	int32 m_LastPacket;						//Packet counter (When the controller detects a change, this value goes up by 1)
	int32 m_ControllerNmb;					//The controller Number (ID)
	//bool m_isSuspended = false;			// !! [DEPRECATED] !! If true, the controller is Suspended (Doesn't pool data from it, and revert vibration back to off)
	bool m_isDeadZoneCircular = true;		//Defines if the deadzone will behave like a circle or a square
	bool m_shutdownOnAppClose = false;		//If true, when this instance is destroyed, the controller is also turned off

	int32 updateState(void);				//Polls this controller data and return the controller state (connected or not)
	int8 buttonBitIndex(XButton button);	//Return the bit index relative to the button given	

public:
	XBOXController(uint8 PlayerNmb);
	~XBOXController();

	bool isAnyButtonDown(void);										//Return true if any button is down (triggers and analogs EXCLUDED)
	bool isAnyStickMove(void);										//Return true if any stick have moved outside it's dead zone
	bool isAnyTriggerPress(void);									//Return true if any trigger have been pressed over it's threshold (deadzone)
	bool isButtonDown(XButton Button);								//Return true if a button or a combination of, is/are down (triggers and analogs EXCLUDED)
	bool isButtonUp(XButton Button);								//Return true if a button or a combination of, is/are up (triggers and analogs EXCLUDED)
	bool isConnected(void);											//Return true if this controller is connected (also pools the controller state!)
	//bool isControllerSuspended(void);								// !! [DEPRECATED] !! Return true if this controller is suspended
	bool isDeadzoneCircular(void);									//Return true if the deadzone mode is circular (default)
	bool isActive(void);											//Return true if any action (button press/analog/trigger) is detected (analog/trigger must pass the deadzone first)
	bool isIdle(void);												//Oposite of 'isActive()'
	AnalogicData* getAnalogic(void);								//Return the analog data
	AnalogicData getAnalogicRaw(void);								//Return the unfiltered analog data (RAW)
	uint16 getButtonPresses(void);									//Return the combination of button presses (triggers and analogs EXCLUDED)
	uint8 getControllerID(void);									//Return the controllerNmb
	DeadZones* getDeadZones(void);									//Return the deadzone data
	void setDeadZoneCircular(bool isCircular);						//Sets the tipe of dead zone (Circular (X and Y dependent) or squared (X and Y independent))
	void setStickLeftDeadZone(uint16 DeadZone);						//Sets the LS Dead Zone for both X and Y
	void setStickLeftDeadZone_X(uint16 DeadZone);					//Sets the LS X Dead Zone
	void setStickLeftDeadZone_Y(uint16 DeadZone);					//Sets the LS Y Dead Zone
	void setTriggerLeftDeadZone(uint8 DeadZone);					//Sets the LT Dead Zone
	void setStickRightDeadZone(uint16 DeadZone);					//Sets the RS Dead Zone for both X and Y
	void setStickRightDeadZone_X(uint16 DeadZone);					//Sets the RS X Dead Zone
	void setStickRightDeadZone_Y(uint16 DeadZone);					//Sets the RS Y Dead Zone
	void setTriggerRightDeadZone(uint8 DeadZone);					//Sets the RT Dead Zone
	//void setResume(void);											// !! [DEPRECATED] !! Sets the controller state to Stand-By
	//void setSuspend(void);										// !! [DEPRECATED] !!Sets the controller state to Suspended
	void shutdown(void);											//Shutdown the controller
	void shutdownOnQuit(bool stdwn);								//Sets the controller "auto-shutdown" on destruction
	//void switchState(void);										// !! [DEPRECATED] !!Switches the controller state to Suspended or Stand-By
	void setVibration(const uint16 LMotor, const uint16 RMotor);	//Sets this controller vibration
	void resetDeadZones(void);										//Resets all Deadzones
	void resetStickLeftDeadZone(void);								//Reset the LS Deadzone
	void resetTriggerLeftDeadZone(void);							//Reset the LT Deadzone
	void resetStickRightDeadZone(void);								//Reset the RS Deadzone
	void resetTriggerRightDeadZone(void);							//Reset the RT Deadzone
	void resetSticksDeadZone(void);									//Reset both LS and RS DeadZones
	void resetTriggersDeadZone(void);								//Reset both LT and RT DeadZones

	//Static functions
	static bool isXInputExReady(void);								//Return true if the PC supports extra functionality on the controller (guide button state read / shutdown controller)

#ifdef MWCH_DEBUG
	int32 d_packetsReceived(void) { return m_ControllerState.dwPacketNumber; };
	void d_ButtonStatus(void);
	BaseString d_StringButtonStatus(void);
#endif
};

class XBOXControllerEx : public XBOXController
{
private:
	BYTE BatteryTp = BATTERY_DEVTYPE_GAMEPAD;		//...or BATTERY_DEVTYPE_HEADSET to pull headset battery
	XINPUT_BATTERY_INFORMATION ControllerBattery;

public:
	XBOXControllerEx(uint8 PlayerNmb);
	~XBOXControllerEx();

	XINPUT_BATTERY_INFORMATION getBatteryData(void);
	const char* getBatteryType(void);
	int getBatteryTypeCode(void);
	const char* getBatteryPower(void);
	int getBatteryPowerCode(void);
};

#endif;