/*-------Thread Handler-------
*	WIP
*	--WIP
*
*
*	Moon Wiz Studios (c) - 11/04/2019
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.0.1
*/

#ifndef _H_MWTH_
#define _H_MWTH_

#include <pthread.h>		//Threads using POSIX! (That's LINUX's)
#include <sched.h>
#include <semaphore.h>

class ThreadWorker
{
private:
	pthread_t m_ThreadID;
	int m_Status;
	void*(*m_Func)(void*);

public:
	ThreadWorker() { };
	ThreadWorker(void *input)
	{
		Run(input);
	}
	ThreadWorker(void *input, void*(*function)(void*))
	{
		Run(input, function);
	}

	void setFunction(void*(*function)(void*))
	{
		m_Func = function;
	}

	int Run(void *input)
	{
		if(input && m_Func)
		{
			m_Status = pthread_create(&m_ThreadID, NULL, m_Func, input);
		}
		else 
		{
			m_Status = -1;
		}
		return m_Status;
	}
	int Run(void *input, void*(*function)(void*))
	{
		m_Func = function;
		return Run(input);
	}
	void* RunReturn(void *input)
	{
		if(!Run(input))
		{
			void *out;
			pthread_join(m_ThreadID, &out);
			return out;
		}
		return nullptr;
	}
	void* RunReturn(void *input, void*(*function)(void*))
	{
		if(!Run(input, function))
		{
			void *out;
			pthread_join(m_ThreadID, &out);
			return out;
		}
		return nullptr;
	}
};

#endif//_H_MWTH_
