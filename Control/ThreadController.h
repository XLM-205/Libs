/*-------DOCUMENTATION PENDING-------
*
*	Moon Wiz Studios (c) - --/--/----
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version ---
*/

#ifndef _H_MWTHREADCONTROLLER
#define _H_MWTHREADCONTROLLER

#include <thread>
#include <future>
#include "DataTypes.h"
#include "CustomTypes-SA.h"

enum ThreadStatus
{
	BUSY,	//Current thread is in use and CANNOT be given a task
	FREE	//Current thread is free, and CAN be given a task
};

enum ThreadType
{
	DEPENDENT,		//Thread bound to main thread (join)
	INDEPENDENT		//Thread independent (detached)
};

//Thread Call Modes	(WIP) -> Using detach, and it's risks
//#define MWTC_MODE_DEPENDENT	0		//Will need to call "mwtcEndThreadTask" to release thread
//#define MWTC_MODE_INDEPENDENT	1		//Thread requested will be

//Helper: http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine

template <class ret, class par>
class ThreadBase
{
private:
	bool m_isRunning;			//Becomes true if the thread is running
	bool m_isDependent;			//If false, the thread runs indepently from the main thread, and don't need to (can't) be joined
	std::thread *m_thread;		//Actual Thread

	void*(*m_func)(void*);		//Function

public:
	ThreadBase() : m_thread() { }

	~ThreadBase() 
	{
		if (m_thread)
		{
			if (m_isDependent)
			{
				m_thread.join();
			}
			delete m_thread;
		}
	}

	int ID(void)
	{
		if (m_thread)
		{
			return m_thread.get_id()._Id;
		}
		return 0;
	}

	bool isRunning(void)
	{
		return m_isRunning;
	}

	void setThread(std::thread *trd)
	{
		Clear();
		m_thread = trd;
	}

	void Clear()
	{
		if (m_thread)
		{
			if (m_isDependent)
			{
				m_thread.join();
			}
			delete m_thread;
		}
	}

	ret Run(par &arg)
	{
		m_isRunning = true;
		return m_func(arg);
	}
	ret Run(par &arg, int count)
	{
		m_isRunning = true;
		return m_funcArr(arg, count);
	}
};

template<>
inline void ThreadBase<void, void*>::Run(void* &arg)
{
	m_func(arg);
}



#endif;	//Header