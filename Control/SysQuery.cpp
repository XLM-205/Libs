/*-------PENDING-------
*	PENDING
*	--PENDING
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
*	Version 0.01
*/

#include "SysQuery.h"

#ifdef MWI_OS_WINDOWS
#include <Windows.h>
#include <WinBase.h>
#include <Psapi.h>
#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#else

#endif

//--------------------------------------------------------------------------------------------------------------------------
//														Memory Pooling
//--------------------------------------------------------------------------------------------------------------------------

//	SysMem		------------------------------------------------------------------------------------------------------------

SysMem::SysMem()
{
#ifdef MWI_OS_WINDOWS
	PROCESS_MEMORY_COUNTERS Mem;
	GetPhysicallyInstalledSystemMemory(&PhysicRAMTotal);
	GetProcessMemoryInfo(GetCurrentProcess(), &Mem, sizeof(PROCESS_MEMORY_COUNTERS));
	CurrentRAMLoad = Mem.WorkingSetSize;
#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#else

#endif

#ifdef MWSQ_DEBUG
	std::cout << "Total Physical RAM: " << PhysicRAMTotal / 1024 << " MB" << std::endl;
#endif
}

void SysMem::updateMemoryUsage(void)
{
#ifdef MWI_OS_WINDOWS
	PROCESS_MEMORY_COUNTERS Mem;
	GetProcessMemoryInfo(GetCurrentProcess(), &Mem, sizeof(PROCESS_MEMORY_COUNTERS));
	CurrentRAMLoad = Mem.WorkingSetSize;
#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#else

#endif
}

//	Statics (SysMem)

uint64 SysMem::getCurrentLoad(void)
{
#ifdef MWI_OS_WINDOWS
	PROCESS_MEMORY_COUNTERS Mem;
	GetProcessMemoryInfo(GetCurrentProcess(), &Mem, sizeof(PROCESS_MEMORY_COUNTERS));
	return Mem.WorkingSetSize;
#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#else

#endif
}

uint64 SysMem::getTotalPhysicalRAM(void)
{
	uint64 Temp;
	GetPhysicallyInstalledSystemMemory(&Temp);
	return Temp;

}

//	SysMemEx	------------------------------------------------------------------------------------------------------------

SysMemEx::SysMemEx()
{
	//CLASS IN DEVELOPMENT
	delete this;
}

//	SysCPU		------------------------------------------------------------------------------------------------------------

SysCPU::SysCPU()
{
	TotalThreads = QueryAvailableThreads();
	isMultiThreadSupport = isMultiThreadSuppported();
}

//	Statics (SysCPU)

#ifdef MWI_OS_WINDOWS		//Code running on windows system
//Return the total of available threads and updates "mwtcThreadsAvailable" value
int SysCPU::QueryAvailableThreads(void)
{
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	return SystemInfo.dwNumberOfProcessors;

}
//Return the processor architecture (type) and updates the "mwtcProcessorType" value
const char* SysCPU::QueryProcessorArchitecture(void)
{
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	switch (SystemInfo.wProcessorArchitecture)
	{
	case 9:
		return "X64";
	case 6:
		return "ARM";
	case 5:
		return "ITA";	//Intel Itanium-based processors. -Didn't knew about these either ' -'
	case 0:
		return "X86";	//The processor is "Intel based" (not Itanium)
	case 0xFFFF:
	default:
		return "UNK";	//Unknown processor. May fall here if you overclocked it, you dingas
	}
}
//return true if this processor is multi-threading ready
bool SysCPU::isMultiThreadSuppported(void)
{
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	if (SystemInfo.dwNumberOfProcessors > 1)
	{
		return true;
	}
	return false;
}

#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#else

#define MWTC_THREAD_APPROX		//Thread reading MAY NOT BE 100% precise. USE WITH CAUTION
//Return the total of available threads and updates "mwtcThreadsAvailable" value
static int QueryAvailableThreads(void)
{
	uint8 Thrds = std::thread::hardware_concurrency();
	if (Thrds > 1)
	{
		mwtcIs_multiThreadSupported = true;
	}
	return Thrds;
}

//Return the processor architecture (type) and updates the "mwtcProcessorType" value
static char* QueryProcessorArchitecture(void)
{
	return "UNK";
}

//If system is unknow, use the std c++ get procedure (that, in this case, should only be used as a "hint")
#endif //Host System CPU enumeration