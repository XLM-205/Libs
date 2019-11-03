/*------- ?? -------
*	--
*
*
*	Moon Wiz Studios (c) - 30/08/2018
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version -.-.-
*/

#ifndef _H_MWLEX_
#define _H_MWLEX_

#include "..\Control\FileStates.h"
#include "..\Control\DataTypes.h"
#include "..\Control\CustomTypes-SA.h"

class LexSymbol
{
private:
	const int m_UID;
	BaseString *m_lexicon;
	BaseString *m_lexComment;

public:
	LexSymbol(const int ID, const char *lexicon) : m_UID(ID), m_lexicon(new BaseString(lexicon)), m_lexComment(nullptr) { }
	LexSymbol(const int ID, const char *lexicon, const char *comment) : m_UID(ID), m_lexicon(new BaseString(lexicon)), m_lexComment(new BaseString(comment)) { }
	~LexSymbol()
	{
		if (m_lexicon)
		{
			delete m_lexicon;
			m_lexicon = nullptr;
		}
		if (m_lexComment)
		{
			delete m_lexComment;
			m_lexComment = nullptr;
		}
	}

	int ID(void)
	{
		return m_UID;
	}
	
	char* Lexicon(void)
	{
		return m_lexicon->getString();
	}

	int getLexiconLenght(void)
	{
		return m_lexicon->Length();
	}

	char* Comment(void)
	{
		if (m_lexComment)
		{
			return m_lexComment->getString();
		}
		return nullptr;
	}
};

class LexDictionary
{
private:
	BaseString *m_lexDefPath;
	Trie<LexSymbol> *m_lexDictionary;
	int m_extraSymbolsStart;
	int m_aditionalSymbols;
	bool m_buildSucess;

public:
	LexDictionary(const char *lexDefinition) : m_buildSucess(false), m_aditionalSymbols(0)
	{
		m_lexDefPath = new BaseString(lexDefinition);
		m_lexDictionary = new Trie<LexSymbol>();
		if (m_lexDefPath->Length())
		{
			FileStates reader(m_lexDefPath->getString(), "r");
			if (reader.fileSuccess())
			{
				m_buildSucess = true;
				while (!reader.endOfFile())
				{
					char *lex = reader.readWord();
					int ID = reader.readInteger();
					char *com = reader.readLine();
					if (lex && com && lex[0] != '!')	//Only insert a new Lexicon if everything is valid!
					{
						char *temp = CharOperations::substringBetween(com, CST_KEY_SINGLE_QUOTE, CST_KEY_SINGLE_QUOTE);
						m_lexDictionary->Insert(lex, new LexSymbol(ID, lex, temp));
						delete[] lex;
						delete[] com;
						delete[] temp;
					}
				}
				m_extraSymbolsStart = getSymbol("__RES_VAR_SLOT__")->ID();
			}
		}
	}
	~LexDictionary()
	{
		if (m_lexDefPath)
		{
			delete m_lexDefPath;
			m_lexDefPath = nullptr;
		}
		if (m_lexDictionary)
		{
			delete m_lexDictionary;
			m_lexDictionary = nullptr;
		}
	}

	LexSymbol* Add(const char *symbol)
	{
		return Add(symbol, nullptr);
	}
	LexSymbol* Add(const char *symbol, const char *comment)
	{
		if (getSymbol(symbol) == nullptr)	//Only add if the symbol actually exists
		{
			m_lexDictionary->Insert(symbol, new LexSymbol(m_extraSymbolsStart + (++m_aditionalSymbols), symbol, comment));
		}
		return getSymbol(symbol);
	}

	LexSymbol* getSymbol(const char *symbol)
	{
		if (m_buildSucess)
		{
			return m_lexDictionary->Get(symbol);
		}
		return nullptr;
	}

	List<LexSymbol>* listAll(void)
	{
		return m_lexDictionary->ListAll();
	}

	int getAmountSymbolsAdded(void)
	{
		return m_aditionalSymbols;
	}

	bool buildSuccess(void)
	{
		return m_buildSucess;
	}
};

#endif