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

enum class FileStatus
{
	FS_ERROR_NO_PATH = -2,	//No or blank path given
	FS_ERROR_NO_FILE,		//No file found with given
	FS_NONE,				//
	FS_SUCCESS				//Sucessfully read file
};

class FileStates
{
protected:
	BaseString *m_state = new BaseString(64);		//State of 'm_fileP', written
	BaseString *m_name = new BaseString(64);		//File name, WITH format
	BaseString *m_format = new BaseString(8);		//Format, WITHOUT file name
	BaseString *m_nameFull = new BaseString(256);	//Full file path
	FileStatus m_stateCode = FileStatus::FS_NONE;		//[-2] - No Path, [-1] - No File, [0] - No State Set (Default), [1] - Sucess
	uint32 m_Lines = 0;			//The total number of lines
	uint32 m_curLine = 0;		//Current line
	FILE *m_fileP = nullptr;	//The file pointer								
	//bool m_eof = false;			//Becomes true if the last read reached the end of file

	FileStatus fileCheck(const char *Filename, const char* Mode)
	{
		if (Filename == NULL || Filename[0] == '\0')
		{
			m_state->setString("ERROR! NO PATH GIVEN");
			m_stateCode = FileStatus::FS_ERROR_NO_PATH;
			return m_stateCode;
		}
		m_fileP = fopen(Filename, Mode);
		if (m_fileP)
		{
			m_state->setString("FILE SUCCESSFULLY READ");
			m_stateCode = FileStatus::FS_SUCCESS;
			return m_stateCode;
		}
		m_state->setString("ERROR! NO FILE FOUND");
		m_stateCode = FileStatus::FS_ERROR_NO_FILE;
		m_fileP = nullptr;
		return m_stateCode;
	}
	FileStatus setFileError(BaseString* Error, FileStatus Code)
	{
		m_state->setString(Error->getString());
		m_stateCode = Code;
		return m_stateCode;
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

	inline FileStatus getFileState(void){ return m_stateCode; };	//Return the m_fileP state code
	inline BaseString* getFileStateWr(void){ return m_state; };		//Return the m_fileP state, written
	inline BaseString* getFileStateName(void) { return m_name; };	//Return the m_fileP name
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
			//m_eof = false;
		}
	}
	FILE* openFile(const char* Filename, const char* ReadMode)		//Tries to open a file. If successful, keep the file open and return the pointer to it
	{
		if (m_fileP)
		{
			closeFile();
		}
		m_nameFull->setString(Filename);
		m_stateCode = fileCheck(m_nameFull->getString(), ReadMode);
		if (m_stateCode == FileStatus::FS_ERROR_NO_FILE)	//File not found (maybe path is broken?)
		{
			m_nameFull->Replace('\\', '/');		//Try to fix it and check again
			m_stateCode = fileCheck(m_nameFull->getString(), ReadMode);
		}
		if (m_stateCode != FileStatus::FS_SUCCESS)	//Failed again. Return null
		{
			m_fileP = nullptr;
			m_nameFull->Clear();
			m_name->setString("UNKW");
			return nullptr;
		}
		grabInfo();	//Everything went well! Grab all the remaining info
		return m_fileP;
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
			m_curLine = 0;
			//m_eof = false;
		}
	}
	uint32 countLines(void)
	{
		if (m_fileP)
		{
			long fIndex = ftell(m_fileP);
			fseek(m_fileP, 0, SEEK_SET);
			while (!endOfFile())
			{
				char buf[512], c;
				(void)fscanf(m_fileP, "%511[^\n]%c", buf, &c);
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
		if (m_fileP && (fgetc(m_fileP) != EOF))
		{
			fseek(m_fileP, -1L, SEEK_CUR);
			return false;
		}
		return true;
	}
	bool fileSuccess(void)
	{
		return m_stateCode == FileStatus::FS_SUCCESS;
	}

	//Resets the pointer and then skips 'line' lines
	void skipTo(int line)
	{
		if (line < m_curLine)					//If the target is ahead of the pointer, reset the pointer..
		{
			resetFile();
		}
		for (int i = m_curLine; i < line; i++)	//...but if not, skip the diference of them
		{
			char *del = readLine();
			if (del)
			{
				delete[] del;
			}
		}
	}

	//Read a Line assuming a base buffer of 2048 bytes and ignoring LineFeed ('\n')
	char* readLine(void)
	{
		return readLine(2048, false);
	}
	char* readLine(int bufSize, bool includeLineFeed)
	{
		if (m_fileP)								//Only read stuff if we opened a file, duh!
		{
			int i = 0, j = 0, stop = bufSize - 1;
			char *buf = new char[bufSize], c = '\0';
			for ( ; i < stop; i++)
			{
				if ((c = (char)fgetc(m_fileP)) == EOF)	//Take c = fgetc() and IF c == EOF, break it	
				{
					//m_eof = true;
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
					m_curLine++;
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
			(void)fscanf(m_fileP, "%s", buf);
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
			(void)fscanf(m_fileP, "%d", &temp);
			return temp;
		}
		return CST_INT32_MIN;
	}
	//Reads a double, returning -0.0 on failure
	double readDouble(void)
	{
		if(m_fileP)
		{
			double temp = 0;
			(void)fscanf(m_fileP, "%lf", &temp);
			return temp;
		}
		return -0.0;
	}

	char* read(int cAmount)
	{
		if (m_fileP)
		{
			int i = 0;
			char *buf = new char[cAmount], c = '\0';
			for (; i < cAmount; i++)
			{
				if ((c = (char)fgetc(m_fileP)) == EOF)
				{
					//m_eof = true;
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
		m_stateCode = FileStateObject.m_stateCode;
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
			uint32 ExtLen = strlen(ValidExtension[i]);
			if (CharOperations::isEqual((char*)&FilenameInput[aux], (char*)ValidExtension[i], ExtLen))
			{
				return i;	//Detected a valid extension. Return the index of it
			}
		}
		return -1;	//Could not detect a valid extension. Return -1
	}

};

#endif	//Moon Wiz File State