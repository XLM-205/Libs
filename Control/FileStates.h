/*-------File State Handler-------
*	??
*	--??
*
*
*	Moon Wiz Studios (c) - 22/06/2016
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.9
*/

#ifndef _MWFS_H_
#define _MWFS_H_

#include <string.h>
#include <stdio.h>
#include "CustomTypes-SA.h"
#include "DataTypes.h"

enum FileStatus
{
	ERROR_NO_PATH = -2,	//No or blank path given
	ERROR_NO_FILE,		//No file found with given path
	SUCCESS = 1			//File successfully read
};

class FileStates
{
protected:
	BaseString *m_state = new BaseString(64);		//State of 'm_fileP', written
	BaseString *m_name = new BaseString(64);		//File name, WITH format
	BaseString *m_format = new BaseString(8);		//Format, WITHOUT file name
	BaseString *m_nameFull = new BaseString(256);	//Full file path
	uint32 m_Lines = 0;			//The total number of lines
	int16 m_stateID = 0;		//Defaults to 0, or, no state read. It's value depends, but generaly, 1 means that the m_fileP was loaded successfuly
	uint16 m_stateIDType = 0;	//Defaults to 0, or, none type specified. Range: 0~4 for [None], [Sucess], [Warning], [Error], [Exception]
	FILE *m_fileP = nullptr;	//The file pointer								
	bool m_eof = false;			//Becomes true if the last read reached the end of file

	int fileCheck(const char *Filename, const char* Mode)
	{
		if (Filename == NULL || Filename[0] == '\0')
		{
			m_state->setString("ERROR! NO PATH GIVEN");
			m_stateIDType = CST_TYPE_ERROR;
			return FileStatus::ERROR_NO_PATH;
		}
		m_fileP = fopen(Filename, Mode);
		if (m_fileP)
		{
			m_state->setString("FILE SUCCESSFULLY READ");
			m_stateIDType = CST_TYPE_SUCCESS;
			return FileStatus::SUCCESS;
		}
		m_state->setString("ERROR! NO FILE FOUND");
		m_stateIDType = CST_TYPE_ERROR;
		m_fileP = nullptr;
		return FileStatus::ERROR_NO_FILE;
	}
	int setFileError(BaseString* Error, uint16 Code)
	{
		m_state->setString(Error->getString());
		m_stateIDType = CST_TYPE_ERROR;
		m_stateID = Code;
		return m_stateID;
	}
	void grabInfo(void)
	{
		int index = m_nameFull->indexOfLast('/');
		if (index == -1)
		{
			index = m_nameFull->indexOfLast('\\');
		}
		m_name->setString(m_nameFull->indexedPointer(index+1));
		m_format->setString(m_name->indexedPointer(m_name->indexOfFirst('.')));
	}

public:
	FileStates() { }
	FileStates(const char *FileName, const char *ReadMode)
	{
		openFile(FileName, ReadMode);
	}
	~FileStates(void)
	{
		closeFile();
		if(m_state)
		{
			delete m_state;
		}
		if(m_name)
		{
			delete m_name;
		}
		if (m_nameFull)
		{
			delete m_nameFull;
		}
		if (m_format)
		{
			delete m_format;
		}
	}

	inline int getFileStateType(void){ return m_stateIDType; };			//Return the state type
	inline int getFileStateCurrent(void){ return m_stateID; };			//Return the m_fileP state
	inline BaseString* getFileStateWr(void){ return m_state; };		//Return the m_fileP state, written
	inline BaseString* getFileStateName(void) { return m_name; };		//Return the m_fileP name
	inline FileStates& getFileStateDataPointer(void) { return *this; };

	inline FILE* swapFile(const char* Filename, const char* ReadMode)
	{
		if (m_fileP)
		{
			closeFile();
		}
		openFile(Filename, ReadMode);
	}
	inline void closeFile(void)
	{
		if (m_fileP)
		{
			fclose(m_fileP);
			m_fileP = nullptr;
			m_Lines = 0;
			m_eof = false;
		}
	}
	FILE* openFile(const char* Filename, const char* ReadMode)		//Tries to open a file. If successful, keep the file open and return the pointer to it
	{
		if (m_fileP)
		{
			closeFile();
		}
		m_nameFull->setString(Filename);
		m_stateID = fileCheck(m_nameFull->getString(), ReadMode);
		if (m_stateID == FileStatus::ERROR_NO_FILE)	//File not found (maybe path is broken?)
		{
			m_nameFull->Replace('\\', '/');		//Try to fix it and check again
			m_stateID = fileCheck(m_nameFull->getString(), ReadMode);
		}
		if (m_stateID < 0)							//Failed again. Return null
		{
			m_fileP = nullptr;
			m_nameFull->Clear();
			m_name->setString("UNKW");
			return nullptr;
		}
		grabInfo();	//Everything went well! Grab all the remaining info
		return m_fileP;
		//m_stateID = fileCheck(Filename, ReadMode);
		//if (m_stateID != MWFS_ERROR_NO_PATH)					//Input path is not valid or non existent
		//{
		//	char FilePath[128];
		//	sprintf(FilePath, Filename);
		//	if (m_stateID == MWFS_ERROR_NO_FILE)				//Try to fix the path in case of "/" or single "\" in the input string
		//	{
		//		Filename = filePathFixer(FilePath);
		//		m_stateID = fileCheck(FilePath, ReadMode);		//Checks again to see if the path was fixed
		//		if (m_stateID == MWFS_ERROR_NO_FILE)			//Object failed the fix -> Impossible to read
		//		{
		//			m_name->setString("UNKW");
		//		}
		//	}
		//	else if (m_stateID == MWFS_SUCCESS)
		//	{
		//		return m_fileP;
		//	}
		//}
		//return nullptr;
	}

	uint32 Lines(void)
	{
		return m_Lines;
	}

	//Resets the pointer to the start of the file
	void resetFile(void)
	{
		if (m_fileP)
		{
			rewind(m_fileP);
			m_eof = false;
		}
	}
	uint32 countLines(void)
	{
		if (m_fileP)
		{
			uint64 fIndex = ftell(m_fileP);
			fseek(m_fileP, 0, SEEK_SET);
			while (!endOfFile())
			{
				char buf[512], c;
				fscanf(m_fileP, "%511[^\n]%c", buf, &c);
				if (c == '\n')
				{
					m_Lines++;
				}
				else
				{
					fseek(m_fileP, -1, SEEK_CUR);
				}
			}
			fseek(m_fileP, fIndex, SEEK_SET);
		}
		return m_Lines;
	}
	bool endOfFile(void)
	{
		/*if (m_fileP)
		{
			char c = fgetc(m_fileP);
			if (c != EOF)
			{
				int code = fseek(m_fileP, -1L, SEEK_CUR);
				c = fgetc(m_fileP);
				return false;
			}
		}
		return true;*/
		return m_eof;
	}
	bool fileSuccess(void)
	{
		return m_stateID == FileStatus::SUCCESS;
	}

	//Read a Line assuming a base buffer of 256 bytes and ignoring LineFeed ('\n')
	char* readLine(void)
	{
		return readLine(256, false);
	}
	char* readLine(int bufSize, bool includeLineFeed)
	{
		if (m_fileP)								//Only read stuff if we opened a file, duh!
		{
			int i = 0, j = 0, stop = bufSize - 1;
			char *buf = new char[bufSize], c = '\0';
			for ( ; i < stop; i++)
			{
				if ((c = fgetc(m_fileP)) == EOF)	//Take c = fgetc() and IF c == EOF, break it	
				{
					m_eof = true;
					if (!i)							//If i == 0, then we didn't read nothing! Empty buffer and return null
					{
						delete[] buf;
						return nullptr;
					}
					break;
				}
				else if (c == '\n')					//We found the LineFeed, return buffer
				{
					if (includeLineFeed)
					{
						buf[j++] = c;
					}
					else if (!i)					//If we don't consider the line feed, and i == 0, then this is a empty line
					{
						delete[] buf;
						return nullptr;
					}
					buf[j] = '\0';
					return buf;
				}
				buf[j++] = c;
			}
			buf[j] = '\0';
			return buf;
		}
		return nullptr;
	}
	//Reading a single word (using '%s') and assuming a buffer size of 32
	char* readWord(void)
	{
		return readWord(32);
	}
	char* readWord(int bufSize)
	{
		if (m_fileP)
		{
			char *buf = new char[bufSize]();
			fscanf(m_fileP, "%s", buf);
			return buf;
		}
		return nullptr;
	}
	//Reads an integer, returning CST_INT32_MIN (-2147483648) on failure
	int readInteger(void)
	{
		if (m_fileP)
		{
			int temp = 0;
			fscanf(m_fileP, "%d", &temp);
			return temp;
		}
		return CST_INT32_MIN;
	}

	char* read(int cAmount)
	{
		if (m_fileP)
		{
			int i = 0;
			char *buf = new char[cAmount], c = '\0';
			for (; i < cAmount; i++)
			{
				if ((c = fgetc(m_fileP)) == EOF)
				{
					m_eof = true;
					if (!i)
					{
						delete[] buf;
						return nullptr;
					}
					break;
				}
				buf[i] = c;
			}
			if (!(i - cAmount))
			{
				i--;
			}
			buf[i] = '\0';
			return buf;
		}
		return nullptr;
	}

	inline void copyFileStateData(FileStates &FileStateObject)			//Copy data from an FileStates object from another class
	{
		m_state = FileStateObject.m_state;
		m_name = FileStateObject.m_name;
		m_Lines = FileStateObject.m_Lines;
		m_stateID = FileStateObject.m_stateID;
		m_stateIDType = FileStateObject.m_stateIDType;
	}
	inline FILE* getFilePointer(void)
	{
		return m_fileP;
	}
	int isValidExtension(const char *FilenameInput, const char **ValidExtension, uint8 Extensions)
	{
		int aux = CharOperations::indexOfLast(FilenameInput, '.') + 1;
		/*for (int i = 0; FilenameInput[i]; i++)
		{
			if (FilenameInput[i] == '.')
			{
				aux = i;
			}
		}
		++aux;*/
		for (int i = 0; i < Extensions; i++)
		{
			uint8 ExtLen = strlen(ValidExtension[i]);
			if (CharOperations::isEqual((char*)&FilenameInput[aux], (char*)ValidExtension[i], ExtLen))
			{
				return i;	//Detected a valid extension. Return the index of it
			}
		}
		return -1;	//Could not detect a valid extension. Return -1
	}

};

#endif	//Moon Wiz File State