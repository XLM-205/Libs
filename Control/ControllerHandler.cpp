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
*	Version 1.8.9
*/
#include "ControllerHandler.h"

#pragma comment(lib, "xinput.lib")

#ifdef MWCH_DEBUG
#include <stdio.h>
#endif

bool AutoLoadXinputExtras(void);

//typedefing a function pointer:
//In this case, the function "XInputGetStateExFunc" will return a DWORD (uint32), and request "DWORD dwUserIndex, XINPUT_STATE *pState" as parameters
typedef uint32(__stdcall *XInputGetStateExFunc)(DWORD dwUserIndex, XINPUT_STATE *pState);
typedef uint32(__stdcall *XInputPowerOffCtrlFunc)(DWORD dwUserIndex);

XInputGetStateExFunc XInputPoolState = nullptr;
XInputPowerOffCtrlFunc XInputShutdown = nullptr;

const bool XInputExReady = AutoLoadXinputExtras();	//If true, then the controller can be shutdown and accept guide button reading

bool AutoLoadXinputExtras(void)
{
	//Ordinals for special XInput functions.
	//(Taken from here: https://gist.github.com/robindegen/9446175)
	/*
	Ordinal 100:
	DWORD XInputGetStateEx(DWORD dwUserIndex, XINPUT_STATE *pState);
	Ordinal 101:
	DWORD XInputWaitForGuideButton(DWORD dwUserIndex, DWORD dwFlag, unKnown *pUnKnown);
	Ordinal 102:
	DWORD XInputCancelGuideButtonWait(DWORD dwUserIndex);
	Ordinal 103:
	DWORD XInputPowerOffController(DWORD dwUserIndex);
	*/
	HINSTANCE XInputEx = nullptr;
	if ((XInputEx = LoadLibrary("xinput1_4.dll")) || (XInputEx = LoadLibrary("xinput1_3.dll")))	//If true, we can find the EX function
	{
		XInputPoolState = (XInputGetStateExFunc)GetProcAddress(XInputEx, (LPCSTR)100);
		XInputShutdown = (XInputPowerOffCtrlFunc)GetProcAddress(XInputEx, (LPCSTR)103);
		if (XInputPoolState && XInputShutdown)
		{
			return true;	//We could find AND fetch the function! All OK
		}
		
	}
	//...but if we couldn't, abort all functions
	XInputPoolState = (XInputGetStateExFunc)XInputGetState;
	XInputShutdown = nullptr;
	return false;
}

XBOXController::XBOXController(uint8 Player)		//Define qual é o controle conectado (numero de 0 - 3)
{
	if (Player > 3)
	{
		m_ControllerNmb = 3;
	}
	else
	{
		m_ControllerNmb = Player;
	}
	m_LastPacket = 0;
}

XBOXController::~XBOXController(void)
{
	if (m_shutdownOnAppClose)
	{
		shutdown();
	}
}

AnalogicData* XBOXController::getAnalogic(void)
{
	return &m_Analogs;
}

AnalogicData XBOXController::getAnalogicRaw(void)
{
	return m_Analogs.getRaw(m_ControllerState.Gamepad);
}

uint16 XBOXController::getButtonPresses(void)
{
	return m_ControllerState.Gamepad.wButtons;
}

uint8 XBOXController::getControllerID(void)
{
	return m_ControllerNmb;
}

DeadZones* XBOXController::getDeadZones(void)
{
	return m_Analogs.getDeadzones();
}

bool XBOXController::isAnyButtonDown(void)
{
	return m_ControllerState.Gamepad.wButtons;
}

bool XBOXController::isAnyStickMove(void)
{
	return m_Analogs.getStickLeft() != 0 || m_Analogs.getStickRight() != 0;
}

bool XBOXController::isAnyTriggerPress(void)
{
	return m_Analogs.getTriggerLeft() || m_Analogs.getTriggerRight();
}

bool XBOXController::isActive(void)
{
	return isAnyButtonDown() || isAnyTriggerPress() || isAnyStickMove();
}

bool XBOXController::isIdle(void)
{
	return !isActive();
}


int8 XBOXController::buttonBitIndex(XButton Button)
{
	switch (Button)
	{
	case XButton::DUP:
		return 0;
	case XButton::DDOWN:
		return 1;
	case XButton::DLEFT:
		return 2;
	case XButton::DRIGHT:
		return 3;
	case XButton::START:
		return 4;
	case XButton::BACK:
		return 5;
	case XButton::LS:
		return 6;
	case XButton::RS:
		return 7;
	case XButton::LB:
		return 8;
	case XButton::RB:
		return 9;
	case XButton::GUIDE:
		return 10;
	case XButton::SYNC:
		return 11;
	case XButton::A:
		return 12;
	case XButton::B:
		return 13;
	case XButton::X:
		return 14;
	case XButton::Y:
		return 15;
	}
	return -1;	//If we reached here, the supplied button is invalid
}

bool XBOXController::isButtonDown(XButton Button)
{
	return BitOperations::IsBitSet<int16>(m_ControllerState.Gamepad.wButtons, buttonBitIndex(Button));
}

bool XBOXController::isButtonUp(XButton Button)
{
	return !BitOperations::IsBitSet<int16>(m_ControllerState.Gamepad.wButtons, buttonBitIndex(Button));
}

bool XBOXController::isConnected(void)		//Testa a conexão do controle
{
	//if (!m_isSuspended)
	if (updateState() == ERROR_SUCCESS)	//ERROR_SUCESS = Não houve erros
	{
		return true;
	}
	return false;
}

//bool XBOXController::isControllerSuspended(void)
//{
//	return m_isSuspended;
//}

bool XBOXController::isDeadzoneCircular(void)
{
	return m_isDeadZoneCircular;
}

void XBOXController::setDeadZoneCircular(bool isCircular)
{
	m_isDeadZoneCircular = isCircular;
}

void XBOXController::setStickLeftDeadZone(uint16 LSDZ)
{
	m_Analogs.getDeadzones()->setLSDZ(LSDZ);
}

void XBOXController::setStickLeftDeadZone_X(uint16 LSDZ)
{
	m_Analogs.getDeadzones()->setLSDZ_X(LSDZ);
}

void XBOXController::setStickLeftDeadZone_Y(uint16 LSDZ)
{
	m_Analogs.getDeadzones()->setLSDZ_Y(LSDZ);
}

void XBOXController::setTriggerLeftDeadZone(uint8 LTDZ)
{
	m_Analogs.getDeadzones()->setLTDZ(LTDZ);
}

void XBOXController::setStickRightDeadZone(uint16 RSDZ)
{
	m_Analogs.getDeadzones()->setRSDZ(RSDZ);
}

void XBOXController::setStickRightDeadZone_X(uint16 RSDZ)
{
	m_Analogs.getDeadzones()->setRSDZ_X(RSDZ);
}

void XBOXController::setStickRightDeadZone_Y(uint16 RSDZ)
{
	m_Analogs.getDeadzones()->setRSDZ_X(RSDZ);
}

void XBOXController::setTriggerRightDeadZone(uint8 RTDZ)
{
	m_Analogs.getDeadzones()->setRTDZ(RTDZ);
}

//void XBOXController::setResume(void)
//{
//	if (m_isSuspended)
//	{
//		m_isSuspended = false;
//		//Getting fresh data
//		updateState();
//		XInputEnable(true);
//	}
//}

//void XBOXController::setSuspend(void)
//{
//	if (!m_isSuspended)
//	{
//		m_isSuspended = true;
//		memset(&m_ControllerState, 0, sizeof(XINPUT_STATE));
//		setVibration(0, 0);
//		XInputEnable(false);
//	}
//}

void XBOXController::setVibration(const uint16 LMotor,const uint16 RMotor)
{
	XINPUT_VIBRATION Vibrate;
	memset(&m_ControllerState, 0, sizeof(XINPUT_STATE));
	Vibrate.wLeftMotorSpeed = LMotor;
	Vibrate.wRightMotorSpeed = RMotor;
	//Vibra o controle
	XInputSetState(m_ControllerNmb, &Vibrate);
	updateState();
}

void XBOXController::shutdown(void)
{
	if (XInputExReady)
	{
		XInputShutdown(m_ControllerNmb);
	}
}

void XBOXController::shutdownOnQuit(bool stdwn)
{
	m_shutdownOnAppClose = stdwn;
}

//void XBOXController::switchState(void)
//{
//	if (m_isSuspended)
//	{
//		setResume();
//	}
//	else
//	{
//		setSuspend();
//	}
//}

void XBOXController::resetDeadZones(void)
{
	resetSticksDeadZone();
	resetTriggersDeadZone();
}

void XBOXController::resetStickLeftDeadZone(void)
{
	m_Analogs.getDeadzones()->setLSDZ(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
}

void XBOXController::resetTriggerLeftDeadZone(void)
{
	m_Analogs.getDeadzones()->setLTDZ(XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

void XBOXController::resetStickRightDeadZone(void)
{
	m_Analogs.getDeadzones()->setRSDZ(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

void XBOXController::resetTriggerRightDeadZone(void)
{
	m_Analogs.getDeadzones()->setRTDZ(XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

void XBOXController::resetSticksDeadZone(void)
{
	resetStickLeftDeadZone();
	resetStickRightDeadZone();
}

void XBOXController::resetTriggersDeadZone(void)
{
	resetTriggerLeftDeadZone();
	resetTriggerRightDeadZone();
}

int32 XBOXController::updateState(void)	//Criando uma classe usando como padrão a estrutura XINPUT_STATE, e armazena os dados
{
	int32 Status = 0;
	//Zera a memoria para o ponto onde esta o estado do controle
	memset(&m_ControllerState, 0, sizeof(XINPUT_STATE));
	//Atualiza o estado do controle
	Status = XInputPoolState(m_ControllerNmb, &m_ControllerState);
	if (m_isDeadZoneCircular)
	{
		m_Analogs.updateValuesCircular(m_ControllerState.Gamepad);
	}
	else
	{
		m_Analogs.updateValuesCustom(m_ControllerState.Gamepad);
	}
	return Status;
}

//Static Functions
bool XBOXController::isXInputExReady(void)
{
	return XInputExReady;
}

//Debug Functions
#ifdef MWCH_DEBUG
void XBOXController::d_ButtonStatus(void)
{
	const char bttns[16][3] = { {"^"},{"v"}, {"<"}, {">"}, {"ST"}, {"BK"}, {"LS"}, {"RS"},
							    {"LB"}, {"RB"},  {"GU"}, {"SY"}, {"A"}, {"B"}, {"X"}, {"Y"} };
	for (int i = 0; i < 16; i++)
	{
		if (BitOperations::IsBitSet<int16>(m_ControllerState.Gamepad.wButtons, i))
		{
			printf("[%s]", bttns[i]);
		}
		else
		{
			printf(" %s ", bttns[i]);
		}
	}
	printf("\nLT: [%3hhu]\n", m_ControllerState.Gamepad.bLeftTrigger);
	printf("RT: [%3hhu]\n", m_ControllerState.Gamepad.bRightTrigger);
	printf("LS.X: [%6d] LS.Y: [%6d]\n", m_ControllerState.Gamepad.sThumbLX, m_ControllerState.Gamepad.sThumbLY);
	printf("RS.X: [%6d] RS.Y: [%6d]\n", m_ControllerState.Gamepad.sThumbRX, m_ControllerState.Gamepad.sThumbRY);
}

BaseString XBOXController::d_StringButtonStatus(void)
{
	BaseString sText(128);
	const char bttns[16][3] = { {"^"},{"v"}, {"<"}, {">"}, {"ST"}, {"BK"}, {"LS"}, {"RS"},
							{"LB"}, {"RB"},  {"GU"}, {"SY"}, {"A"}, {"B"}, {"X"}, {"Y"} };
	for (int i = 0; i < 16; i++)
	{
		if (BitOperations::IsBitSet<int16>(m_ControllerState.Gamepad.wButtons, i))
		{
			sText.Add('[');
			sText.Add(bttns[i]);
			sText.Add(']');
		}
		else
		{
			sText.Add(bttns[i]);
		}
	}
	return sText;
}
#endif

//XBOXControllerEx Definitions ----------------------------------------------------------------------------------------------------------

XBOXControllerEx::XBOXControllerEx(uint8 Player) : XBOXController(Player) { }

XBOXControllerEx::~XBOXControllerEx() { }

XINPUT_BATTERY_INFORMATION XBOXControllerEx::getBatteryData(void)
{
	memset(&ControllerBattery, 0, sizeof(XINPUT_BATTERY_INFORMATION));
	XInputGetBatteryInformation(m_ControllerNmb, BatteryTp, &ControllerBattery);
	return ControllerBattery;
}

//TODO: Way to get data from 'ControllerBattery' without having to frequently call 'getBatteryData()' to update it
//		(?) Possibly a packet timer ?

const char* XBOXControllerEx::getBatteryPower(void)
{
	switch (getBatteryData().BatteryLevel)
	{
	case BATTERY_LEVEL_EMPTY:
		return "  0%";
	case BATTERY_LEVEL_LOW:
		return " 25%";
	case BATTERY_LEVEL_MEDIUM:
		return " 75%";
	case BATTERY_LEVEL_FULL:
		return "100%";
	case BATTERY_LIFE_UNKNOWN:
	default:
		return "???%";
	}
}

int XBOXControllerEx::getBatteryPowerCode(void)
{
	return getBatteryData().BatteryLevel;
}

const char* XBOXControllerEx::getBatteryType(void)
{
	switch (getBatteryData().BatteryType)
	{
	case BATTERY_TYPE_ALKALINE:
		return "ALKN";
	case BATTERY_TYPE_NIMH:
		return "NiMh";
	case BATTERY_TYPE_WIRED:
		return "Wire";
	case BATTERY_TYPE_UNKNOWN:
	case BATTERY_TYPE_DISCONNECTED:
	default:
		return "UNKW";
	}
}

int XBOXControllerEx::getBatteryTypeCode(void)
{
	return getBatteryData().BatteryType;
}