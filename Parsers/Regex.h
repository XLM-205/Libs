/*-------Regex-------
*	??
*	--??
*
*
*	Moon Wiz Studios (c) - 21/10/2019
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.9
*/

#ifndef _H_MWREGEX_
#define _H_MWREGEX_

#include <Control/CustomTypes-SA.h>
#include <Control/DataTypes.h>

class Regex
{
private:
	class RegRule
	{
	private:
		BaseString *m_ruleID;

	public:
		RegRule(const char *name)
		{
			m_ruleID = new BaseString(name);
		}
		~RegRule()
		{
			if (m_ruleID)
			{
				delete m_ruleID;
			}
		}
	};

public:
};

#endif
