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

#ifndef _H_MWSYSQUERY_
#define _H_MWSYSQUERY_
//Moon Wiz System Query a.k.a MWSQ

#include "Igniter.h"

#ifndef _H_CSTTYPE_
typedef unsigned char uint8;
typedef unsigned long long uint64;
#endif

//If this library is included, then the class data should be deleted by the main program

class SysMem
{
private:
	uint64 PhysicRAMTotal;
	uint64 CurrentRAMLoad;	//The amount of memory being used by this application

public:
	SysMem();

	static uint64 getCurrentLoad(void);
	static uint64 getTotalPhysicalRAM(void);
	void updateMemoryUsage(void);

};

//Same as SysMem, but with the hability to count paging files, cache and available RAM during runtime (WIP)
class SysMemEx : protected SysMem
{
protected:
	uint64 CacheAvailable		= 0;
	uint64 PagingAvailable		= 0;
	uint64 PhysicRAMAvailable	= 0;
public:
	SysMemEx();

};

class SysCPU
{
private:
	uint8 TotalThreads			= 1;
	bool isMultiThreadSupport	= false;

public:
	SysCPU();

	//Return the total of available threads and updates "mwtcThreadsAvailable" value
	static int QueryAvailableThreads(void);
	//Return the processor architecture (type) and updates the "mwtcProcessorType" value
	static const char* QueryProcessorArchitecture(void);
	static bool isMultiThreadSuppported(void);
};

#endif;	//Library