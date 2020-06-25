/*-------Moon Wiz Atlas File Parser-------
*	- WIP
*
*
*	Moon Wiz Studios (c) - --/--/----
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.5
*/

#include "..\Control\MWAHandler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

MWAtlasHandler::MWAtlasHandler(char* File)
{
	m_isAtlasBuilt = parseMWA(File);
}

MWAtlasHandler::~MWAtlasHandler(void)
{
	purgeData();
}

void MWAtlasHandler::allocInit(uint16 Size)
{
	if (!m_Files)
	{
		m_Files = new String128[Size];
	}
	if (!m_isCharacterTable)
	{
		m_isCharacterTable = new bool[Size];
	}
	if (!m_SequenceTotal)
	{
		m_SequenceTotal = new uint16[Size];
	}
	if (!m_SelectionTotal)
	{
		m_SelectionTotal = new uint16[Size];
	}
	if (!m_GlobalSpeed)
	{
		m_GlobalSpeed = new int16[Size];
	}
	if (!m_Sequences)
	{
		m_Sequences = new Seq*[Size];
		for (int i = 0; i < Size; i++)
		{
			m_Sequences[i] = nullptr;
		}
	}
}

void MWAtlasHandler::errorDuplication(char* Symbol, uint8 Line)
{
	String64 Error;					//Used if the parser have found any errors
	sprintf(Error.getString(), "ERROR! DUPLICATED '%s' SYMBOL AT LINE %d", Symbol, Line);
	setFileError(&Error, MWAErrorCodes::MWAH_ERROR_BAD_FORMAT);
	purgeData();
}

//Reading -------------------------------------------------------------------------------

bool MWAtlasHandler::parseMWA(char* File)
{
	if (!m_isAtlasBuilt)	//Just parse a file if the Atlas wasn't build
	{
		if (File == NULL || File == "")
		{
			flstStateWr = "ERROR! NO PATH GIVEN";
			flstStateType = CST_TYPE_ERROR;
			return false;
		}
		openFile(File, "r");
		uint16 Comparator = strlen(File);
		if (flstFileData == NULL)
		{
			flstStateWr = "ERROR! NO FILE FOUND";
			flstStateType = CST_TYPE_ERROR;
			closeFile();
			return false;
		}
		else if (!CharOperations::isEqual(&File[Comparator - 3], "mwa", 3))
		{
			flstStateWr = "ERROR! INVALID/UNREADBLE FILE";
			flstStateType = CST_TYPE_ERROR;
			closeFile();
			return false;
		}
		flstStateWr = "FILE SUCCESSFULLY READ";
		flstStateType = CST_TYPE_SUCCESS;

		uint16 FileIndex = 0;			//Point to the "m_Files" based on order of appearance
		uint16 DescIndex = 0;
		uint16 SeqIndex = 0;			//Point to the correct sequence between each sheet. MUST BE RESET AT 'F' SYMBOL READING
		uint16 SelIndex = 0;			//Point to the correct selection between sequences. MUST BE RESET AT 'I' SYMBOL READING
		uint16 SeqID = 0, Selections = 0, Speed = 0, GenericInput = 0;
		uint16 Size = 0;
		uint16 FileLine = 0;
		float UVAX = 0, UVAY = 0;
		float UVBX = 0, UVBY = 0;
		char SeqName[32] = { '\0' };
		char FileIn[512] = { '\0' };

		//Special fscanf parameters:
		// %i   - Signed int
		// %u   - Unsigned int
		// %hi  - Signed SHORT int
		// %hu  - Unsigned SHORT int
		// %hhi - Signed char (int8)
		// %hhu - Unsigned char (uint8)

		//BUG FIX REPORT:
		//	- LAST BUG (24/06/2016): 'STACK CORRUPTION between "SeqID", "Selections", "Speed" and "GenericInput"'
		//	- CAUSE: Values read are of format 'uint16'/'int16' (SHORT (un)signed int (2 Bytes)). fscanf read for '%d' (Normal int (4 Bytes))
		//			 Because of that, the memory would change abruptly from 2 bytes to 4 bytes, thus corrupting the stack
		//	- SOLUTION:	Change fscanf parameters to %hu/%hi, to read SHORT formatted int

		for (FileLine = 0; strcmp("END", FileIn); FileLine++)	//Only stop when finding the "END" symbol
		{
			fscanf(flstFileData, "%s", FileIn);
			switch (FileIn[0])
			{
			case '!':	//Comment line. Skip it
					fscanf(flstFileData, " %512[^\n]", FileIn);
				break;
			case 'F':	//File path line
				if (FileIn[1] == 'T')	//If found, there is more than one sheet in this file
				{
					if (!m_SheetsAmount)
					{
						fscanf(flstFileData, "%hu", &m_SheetsAmount);
						allocInit(m_SheetsAmount);
					}
					else //If we fall here, this means that the 'FT' symbol was duplicated
					{
						errorDuplication("FT", FileLine);
						return false;
					}
				}
				else
				{
					if (m_SheetsAmount)	//If != 0, this means that we already hit a 'FT' block, therefore, we just need to record the file paths
					{
						if (FileIndex >= m_SheetsAmount)	// WARNING! BAD FORMATED MWA! HALT READING
						{
							errorDuplication(FileIn, FileLine);
							return false;
						}
						fscanf(flstFileData, "%s", FileIn);
						m_Files[FileIndex] = FileIn;
					}
					else
					{
						m_SheetsAmount = 1;
						allocInit(1);
						fscanf(flstFileData, "%s", FileIn);
						m_Files[0] = FileIn;
					}
					FileIndex++;
				}
				SeqIndex = 0;
				break;
			case 'D':	//Description
				if (FileIn[1] == 'T')	//If found, there is more than one description in this file
				{
					fscanf(flstFileData, "%hu", &m_DescriptionsAmount);
					m_Desc = new char*[m_DescriptionsAmount];
					for (int i = 0; i < m_DescriptionsAmount; i++)
					{
						m_Desc[i] = nullptr;
					}
				}
				else
				{
					fscanf(flstFileData, " %512[^\n]", FileIn);
					Size = strlen(FileIn) + 1;
					if (m_DescriptionsAmount)
					{
						if (DescIndex >= m_DescriptionsAmount)
						{
							errorDuplication(FileIn, FileLine);
							return false;
						}
						m_Desc[DescIndex] = new char[Size];
						memcpy(m_Desc[DescIndex], FileIn, Size);
						DescIndex++;
					}
					else
					{
						m_DescriptionsAmount = 1;
						if (m_Desc)
						{
							m_Desc = new char*[1];
							m_Desc[0] = new char[Size];
							memcpy(m_Desc[0], FileIn, Size);
						}
					}
				}
				break;
			case 'T':	//Sprite Sheet Type. [0] - Classic | [1] - Character Table
				GenericInput = 0;
				fscanf(flstFileData, "%hu", &GenericInput);
				m_isCharacterTable[FileIndex - 1] = GenericInput;
				break;
			case 'S':	//Sprite Sheet Sequences/Selection totals OR sequence Speed
				fscanf(flstFileData, "%hu", &GenericInput);
				if (FileIn[1] == 'Q')			//Sequence
				{
					m_SequenceTotal[FileIndex - 1] = GenericInput;
					m_Sequences[FileIndex - 1] = new Seq[GenericInput];
				}
				else if (FileIn[1] == 'L')		//Selection
				{
					m_SelectionTotal[FileIndex - 1] = GenericInput;
				}
				else if (FileIn[1] == 'P')		//Speed
				{
					if (m_GlobalSpeed[FileIndex - 1] == -1)
					{
						Speed = GenericInput;
						m_Sequences[FileIndex - 1][SeqIndex].setSpeed(Speed);
					}
				}
				break;
			case 'G':	//Sprite Sheet Global Speed. [-1] - Indepent Speed | [0] - Static | [>0] - Global Speed
				fscanf(flstFileData, "%hi", &m_GlobalSpeed[FileIndex - 1]);
				break;
			case 'I':	//ID reading
				fscanf(flstFileData, "%hu %hu %32[^\n]", &SeqID, &Selections, SeqName);
				if (m_GlobalSpeed[FileIndex - 1] >= 0)	//Just set the speed, if the 'm_GlobalSpeed' != -1, since it's already handled with 'SP' symbol, upwards
				{
					Speed = m_GlobalSpeed[FileIndex - 1];
				}
				m_Sequences[FileIndex - 1][SeqIndex].buildSequence(SeqID, Speed, Selections, &String32(SeqName));
				SeqIndex++;
				SelIndex = 0;
				break;
			case 'C':	//Background Color reading
				uint16 In[4];	//Used as uint16 instead of uint8 because, for some reason, when reading %hhu (unsigned char) it tries to read unsigned int (uint32), thus corrupting the stack
				fscanf(flstFileData, "%hu %hu %hu %hu", &In[0], &In[1], &In[2], &In[3]);
				m_Sequences[FileIndex - 1][SeqIndex - 1].setBGColors(In[0], In[1], In[2], In[3]);
				break;
			case 'U':	//UVX/UVY reading
				if (m_isCharacterTable[FileIndex - 1])
				{
					fscanf(flstFileData, "%f %f %hu", &UVAX, &UVAY, &GenericInput);
					m_Sequences[FileIndex - 1][SeqIndex - 1].setIDChar((char)GenericInput);
				}
				else
				{
					fscanf(flstFileData, "%f %f", &UVAX, &UVAY);
				}
				fscanf(flstFileData, "%s", FileIn);			//Dumping next 'UV' symbol, since it MUST come in pairs
				fscanf(flstFileData, "%f %f", &UVBX, &UVBY);
				m_Sequences[FileIndex - 1][SeqIndex - 1].setUVA(&mwVec2f(UVAX, UVAY), SelIndex);
				m_Sequences[FileIndex - 1][SeqIndex - 1].setUVB(&mwVec2f(UVBX, UVBY), SelIndex);
				FileLine++;
				SelIndex++;
				break;
			}
		}
		closeFile();

		//TODO: Security Prevention

		return true;
	}
}

void MWAtlasHandler::swapMWA(char* NewFile)
{
	purgeData();
	m_isAtlasBuilt = parseMWA(NewFile);
}

//Saving --------------------------------------------------------------------------------

//Reset ---------------------------------------------------------------------------------

void MWAtlasHandler::purgeData(void)
{
	if (m_Sequences)
	{
		for (int i = 0; i < m_SheetsAmount; i++)
		{
			if (m_Sequences[i])
			{
				delete[] m_Sequences[i];
				m_Sequences[i] = nullptr;
			}
		}
		delete[] m_Sequences;
		m_Sequences = nullptr;
	}
	if (m_Desc)
	{
		for (int i = 0; i < m_DescriptionsAmount; i++)
		{
			if (m_Desc[i])
			{
				delete[] m_Desc[i];
				m_Desc[i] = nullptr;
			}
		}
		delete[] m_Desc;
		m_Desc = nullptr;
	}
	if (m_Files)
	{
		delete[] m_Files;
		m_Files = nullptr;
	}
	if (m_isCharacterTable)
	{
		delete[] m_isCharacterTable;
		m_isCharacterTable = nullptr;
	}
	if (m_SequenceTotal)
	{
		delete[] m_SequenceTotal;
		m_SequenceTotal = nullptr;
	}
	if (m_SelectionTotal)
	{
		delete[] m_SelectionTotal;
		m_SelectionTotal = nullptr;
	}
	if (m_GlobalSpeed)
	{
		delete[] m_GlobalSpeed;
		m_GlobalSpeed = nullptr;
	}
	m_isAtlasBuilt = false;
}

inline void MWAtlasHandler::resetValues(void)
{
	m_isAtlasBuilt = false;
	m_isCharacterTable = false;
	m_SheetsAmount = 0;
	m_DescriptionsAmount = 0;
}
