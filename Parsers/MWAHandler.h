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

#ifndef _MWAH_H_
#define _MWAH_H_

#include "..\Control\CustomTypes-SA.h"
#include "..\Control\FileStates.h"
#include "..\Maths\MoonPhysics-Vec2d.h"

enum MWAErrorCodes
{
	MWAH_ERROR_BAD_FORMAT = 5	//File doesn't conform with standard (Bad formatted file)
};

struct UVData
{
	mwVec2f UVA;
	mwVec2f UVB;
};

class MWAtlasHandler : protected FileStates
{
private:
	class Seq
	{
	private:
		union ID
		{
			uint16 m_GlobalID;
			uint8 m_IDChar;

			uint16 getID(void)
			{
				return m_GlobalID;
			}
			char getCharID(void)
			{
				return m_IDChar;
			}

			void operator=(uint16 k)
			{
				m_GlobalID = k;
			}
		};

		ID m_ID;				//This sequence unique ID. If 'm_isCharacterTable' is true, this
		uint16 m_Selections;	//The total of selections this sequence contains from 0 ~ (Total - 1).
		uint16 m_Speed;			//Sequence's speed in FPS
		String32 m_SeqName;		//This sequence named identifier
		RGBAColorPalette m_BG;	//The Sprite background that should be removed when using this sequence
		UVData *m_UV;			//The UVX/UVY of that relates to the sprite

	public:
		Seq()
		{
			m_ID = 0;
			m_Selections = 0;
			m_Speed = 0;
			m_SeqName = "??";
			m_UV = nullptr;
		}
		~Seq(void)
		{
			if (m_UV)
			{
				delete[] m_UV;
				m_UV = nullptr;
			}
		}

		void buildSequence(uint16 id, uint16 spd, uint16 sel, String32* sqnm)
		{
			m_ID = id;
			m_Speed = spd;
			m_Selections = sel;
			m_SeqName = *sqnm;
			if (!m_UV)
			{
				m_UV = new UVData[m_Selections];
			}
		}
		inline bool isSequenceBuilt(void)
		{
			return m_UV == nullptr ? false : true;
		}

		inline uint8 getAlphaBG()
		{
			return m_BG.getAlpha();
		}
		inline uint8* getBGColors(void)
		{
			return (uint8*)(m_BG.getRed(), m_BG.getGreen(), m_BG.getBlue(), m_BG.getAlpha());
		}
		inline uint8 getBlueBG()
		{
			return m_BG.getBlue();
		}
		inline uint8 getGreenBG()
		{
			return m_BG.getGreen();
		}
		inline uint16 getID(void)
		{
			return m_ID.getID();
		}
		inline char getIDChar(void)
		{
			return m_ID.getCharID();
		}
		inline uint8 getRedBG(void)
		{
			return m_BG.getRed();
		}
		inline uint16 getSpeed(void)
		{
			return m_Speed;
		}
		inline uint16 getSelectionTotal(void)
		{
			return m_Selections;
		}
		inline char* getSequenceName(void)
		{
			return m_SeqName.getString();
		}
		inline mwVec2f getUVA(uint16 index)
		{
			return m_UV[index].UVA;
		}
		inline mwVec2f getUVB(uint16 index)
		{
			return m_UV[index].UVB;
		}
		inline UVData getUVData(uint16 index)
		{
			return m_UV[index];
		}
		
		inline void setAlphaBG(uint8 Alpha)
		{
			m_BG.setAlpha(Alpha);
		}
		void setBGColors(uint8* Colors)
		{
			m_BG.setRed(Colors[0]);
			m_BG.setGreen(Colors[1]);
			m_BG.setBlue(Colors[2]);
			m_BG.setAlpha(Colors[3]);
		}
		void setBGColors(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha)
		{
			m_BG.setRed(Red);
			m_BG.setGreen(Green);
			m_BG.setBlue(Blue);
			m_BG.setAlpha(Alpha);
		}
		inline void setBlueBG(uint8 Blue)
		{
			m_BG.setBlue(Blue);
		}
		inline void setGreenBG(uint8 Green)
		{
			m_BG.setGreen(Green);
		}
		inline void setID(uint16 id)
		{
			m_ID = id;
		}
		inline void setIDChar(char id)
		{
			m_ID.m_IDChar = id;
		}
		inline void setRedBG(uint8 Red)
		{
			m_BG.setRed(Red);
		}
		inline void setSpeed(uint16 spd)
		{
			m_Speed = spd;
		}
		inline void setUVA(mwVec2f* uva, uint16 i)
		{
			m_UV[i].UVA = *uva;
		}
		inline void setUVB(mwVec2f* uvb, uint16 i)
		{
			m_UV[i].UVB = *uvb;
		}

	};

protected:
	bool m_isAtlasBuilt			= false;	//If true, this Atlas is ready to use
	bool m_isSetupStep1			= false;	//If true, the handler HEADER is ready
	bool m_isSetupStep2			= false;	//If true, the handler BODY is ready. Requires 'm_isSetupStep1' to be true
	bool m_isSetupStep3			= false;	//If true, the handler UV is ready. Requires 'm_isSetupStep2' to be true
	uint16 m_SheetsAmount		= 0;		//How many sheets should this file manages?
	uint16 m_DescriptionsAmount = 0;		//How many descriptions this file contains?
	String128* m_Files			= nullptr;	//Holds all sheets paths
	uint16* m_SequenceTotal		= nullptr;	//The total amount of sequences contained in this sheet (or file)
	uint16* m_SelectionTotal	= nullptr;	//The total amount of selections contained in this sheet (or file)
	int16* m_GlobalSpeed		= nullptr;	//The global speed of all sequences of this sheet. If [-1] - Each sequence have it's own speed | [0] - No animation | [>0] - Global Animation Speed
	bool* m_isCharacterTable	= nullptr;	//If true, this sheet is a character table (each sequence contains only one selection WITH NONE speed and it's ID is the char value)
	Seq** m_Sequences			= nullptr;	//Holds all the sequences this Atlas manages
	char** m_Desc				= nullptr;	//Holds the descriptions (if any) of each (or overall) sprite-sheet(s)

	void allocInit(uint16 Memory);						//Safelly allocate memory
	void errorDuplication(char* Symbol, uint8 Line);	//Duplicated symbol found! Purge memory

public:
	MWAtlasHandler(char* mwaFile);
	MWAtlasHandler(void) { };
	~MWAtlasHandler(void);

	//Reading Methods
	bool parseMWA(char* File);		//Returns true if the file was successfuly read
	void swapMWA(char* NewFile);

	//Saving Methods
	bool generateMWA(void);			//Returns true if the file was successfuly generated
	void setupMWAHeader(uint16 Sheets, uint16 Descriptions);
	void setupMWABody(String128* FilePath, bool isCharTable, uint16 TotSequences, uint16 TotSelections, int16 GlobalSpeed, uint16 SeqSpeed, char* Description, uint16 SheetIndex);
	void setupMWAUV(uint16 SeqGlobalID, uint16 Selections, String32* SeqName, mwVec2f* UVA, mwVec2f* UVB, uint16 SequenceIndex);

	//Reset Methods
	void purgeData(void);
	inline void resetValues(void);
	
};

#endif;	//Moon Wiz Atlas Parser (MWAP)