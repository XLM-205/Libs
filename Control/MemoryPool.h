/*-------Memory Pool-------
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
*	Version --.--
*/

#ifndef _H_MWMPOOL
#define _H_MWMPOOL

#ifdef MWMP_DEBUG
#include <iostream>
#endif

#include "CustomTypes-SA.h"
#include "NodeHandler.h"

enum PoolOverwriteBehaviour
{
	MWMP_OVERWRITE_DONT_INSERT,		//When an exceeding data is inserted, don't overwrite (ignore input data) (default)
	MWMP_OVERWRITE_FIRST_ENTRY,		//When an exceeding data is inserted, overwrite the first one in the chain
	MWMP_OVERWRITE_LAST_ENTRY,		//When an exceeding data is inserted, overwrite the last one in the chain
	MWMP_OVERWRITE_SEARCH_IN_LINK,	//When an exceeding data is inserted, the code will try to insert it into the next available slot in a linked memory pool
	MWMP_OVERWRITE_REQUEST_MORE		//When an exceeding data is inserted, the code will allocate new blocks (default new) for it
};

enum PoolPaddingBehaviour
{
	MWMP_PADDING_X86,					//Will make paddings until a 4 byte pattern (multiple) is filled (default)
	MWMP_PADDING_X86_SHIELDED,			//Will make paddings until a 4 byte pattern (multiple) is filled. If incoming data is a 4 byte pattern, will shield it with a 4 byte padding
	MWMP_PADDING_X64,					//Will make paddings until a 8 byte pattern (multiple) is filled
	MWMP_PADDING_X64_SHIELDED,			//Will make paddings until a 8 byte pattern (multiple) is filled. If incoming data is a 8 byte pattern, will shield it with a 8 byte padding
	MWMP_PADDING_SINGLE_BYTE,			//Will make a padding of 1 single byte (already shielded)
	MWMP_PADDING_NO_PADDING				//Will NOT make paddings
};

class MemoryPool
{
protected:
	void writeData(void* Tgt, const uint32 Size)
	{
		//Read as: COPY/(OVER)WRITE to 'Tgt', the info FROM 'this', the NUMBER OF BYTES of 'DataChunk' (the data type)
		memcpy(Tgt, this, sizeof(Size));
	}
	void readData(const void* Src, const uint32 Size)
	{
		memcpy(this, Src, sizeof(Size));
	}

	//Signatures
	const int8* SignaturePoolEnd = nullptr;	//Sinalizes the end of the pool (only acount for the pool, not for extra requested memory through 'OVERWRITE_REQUEST')
	const int8  SignatureFreeBlock = 0xAA;	//Sinalizes that the current memory block is free for use (DEC: -86)

	//Data Holders
	uint32 ByteSlots;			//The amount of bytes (slots) available to hold data. Should NOT be changed after class instantiation
	uint32 UsedByteSlots;		//The amount of used bytes (slots) from the available slots. Also serves as pointer to incoming data
	int8* PoolData = nullptr;	//Pointer to all memory blocks available in the memory pool
	int8* NextBlock = nullptr;	//Pointer to the nearest memory block available, following the padding rules

	//Linkage
	MemoryPool *LinkNext = nullptr;	//Link to a 'next' memory pool. Used to make memory pools share data and space between them
	MemoryPool *LinkPrev = nullptr;	//Link to a 'previous' memory pool. Used to make memory pools share data and space between them

	//Control
	ListPointedManager<int8> *ExtraRequests = nullptr;								//Holds all extra request pointers to delete them afterwards
	void* (MemoryPool::*PaddingFN)(int8* MemBlk, const uint32 Size) = nullptr;		//Padding function (defaults to 'X86')
	void* (MemoryPool::*OverwriteFN)(void* InsData, const uint32 Size) = nullptr;	//Overwrite Function (defaults to 'DONT INSERT')
	bool canInsert(const uint32 TgtSize)
	{
		//"Can the data that have 'TgtSize' fit in the pool available space?"
		return (ByteSlots - UsedByteSlots) >= TgtSize ? true : false;
	}
	bool canInsertAnywhere(const uint32 TgtSize)
	{
		return ByteSlots >= TgtSize ? true : false;
	}

	//Start-up
	void poolDataAlloc(void)
	{
		PoolData = new int8[ByteSlots];
		SignaturePoolEnd = PoolData + ByteSlots;
		NextBlock = PoolData;
		memset(PoolData, SignatureFreeBlock, ByteSlots);
	}

	//Delegated Functions Body: Padding
	void* paddingX86(int8* CrrBlk, const uint32 Size)
	{
		uint8 Rest = Size % 4;
		uint8 Padding = 4 - Rest;
		if (UsedByteSlots < ByteSlots)
		{
			CrrBlk += Size;		//Skipping the padded data
		}
		if (Rest)	//If the size is not a multiple of 4, add padding
		{
			if (canInsert(Rest))	//If there is room, add basic padding
			{
				memset(CrrBlk, 0, Padding);
				CrrBlk += Padding;
				UsedByteSlots += Padding;
			}
			else					//If there isn't, add all we can
			{
				uint8 FreeBt = getMemoryPoolFreeBytes();
				memset(CrrBlk, 0, FreeBt);
				CrrBlk += FreeBt;
				UsedByteSlots += FreeBt;
			}
		}
		return CrrBlk;
	}
	void* paddingShieldedX86(int8* CrrBlk, const uint32 Size)
	{
		uint8 Rest = Size % 4;
		uint8 Padding = 4 - Rest;
		if (UsedByteSlots < ByteSlots)
		{
			CrrBlk += Size;		//Skipping the padded data
		}
		if (Rest)	//If the size is not a multiple of 4, add padding
		{
			if (canInsert(Rest))	//If there is room, add basic padding
			{
				memset(CrrBlk, 0, Padding);
				CrrBlk += Padding;
				UsedByteSlots += Padding;
			}
			else					//If there isn't, add all we can
			{
				uint8 FreeBt = getMemoryPoolFreeBytes();
				memset(CrrBlk, 0, FreeBt);
				CrrBlk += FreeBt;
				UsedByteSlots += FreeBt;
			}
		}
		else		//If it is, add a 4 byte padding
		{
			if (canInsert(4))	//If there is room, add basic padding
			{
				memset(CrrBlk, 0, 4);
				CrrBlk += 4;
				UsedByteSlots += 4;
			}
			else					//If there isn't, add all we can
			{
				uint8 FreeBt = getMemoryPoolFreeBytes();
				memset(CrrBlk, 0, FreeBt);
				CrrBlk += FreeBt;
				UsedByteSlots += FreeBt;
			}
		}
		return CrrBlk;
	}
	void* paddingX64(int8* CrrBlk, const uint32 Size)
	{
		uint8 Rest = Size % 8;
		uint8 Padding = 8 - Rest;
		if (UsedByteSlots < ByteSlots)
		{
			CrrBlk += Size;		//Skipping the padded data
		}
		if (Rest)	//If the size is not a multiple of 8, add padding
		{
			if (canInsert(Rest))	//If there is room, add basic padding
			{
				memset(CrrBlk, 0, Padding);
				CrrBlk += Padding;
				UsedByteSlots += Padding;
			}
			else					//If there isn't, add all we can
			{
				uint8 FreeBt = getMemoryPoolFreeBytes();
				memset(CrrBlk, 0, FreeBt);
				CrrBlk += FreeBt;
				UsedByteSlots += FreeBt;
			}
		}
		return CrrBlk;
	}
	void* paddingShieldedX64(int8* CrrBlk, const uint32 Size)
	{
		uint8 Rest = Size % 8;
		uint8 Padding = 8 - Rest;
		if (UsedByteSlots < ByteSlots)
		{
			CrrBlk += Size;		//Skipping the padded data
		}
		if (Rest)	//If the size is not a multiple of 8, add padding
		{
			if (canInsert(Rest))	//If there is room, add basic padding
			{
				memset(CrrBlk, 0, Padding);
				CrrBlk += Padding;
				UsedByteSlots += Padding;
			}
			else					//If there isn't, add all we can
			{
				uint8 FreeBt = getMemoryPoolFreeBytes();
				memset(CrrBlk, 0, FreeBt);
				CrrBlk += FreeBt;
				UsedByteSlots += FreeBt;
			}
		}
		else		//If it is, add a 8 byte padding
		{
			if (canInsert(8))		//If there is room, add basic padding
			{
				memset(CrrBlk, 0, 8);
				CrrBlk += 8;
				UsedByteSlots += 8;
			}
			else					//If there isn't, add all we can
			{
				uint8 FreeBt = getMemoryPoolFreeBytes();
				memset(CrrBlk, 0, FreeBt);
				CrrBlk += FreeBt;
				UsedByteSlots += FreeBt;
			}
		}
		return CrrBlk;
	}
	void* paddingSingle(int8* CrrBlk, const uint32 Size)
	{
		if (UsedByteSlots < ByteSlots)
		{
			CrrBlk += Size;		//Skipping the padded data
		}
		if (canInsert(1))
		{
			memset(CrrBlk, 0, 1);
			CrrBlk++;
			if (CrrBlk == (PoolData + ByteSlots))	//If true, then we are at the pool limit (CrrBlk + 2 will be over the limit: PoolData[ByteSlots]
			{
				return CrrBlk;
			}
			return CrrBlk + 2;
		}
		//We cannot insert EVEN A SINGLE BYTE! That means we are at the limit
		return CrrBlk;
		
	}
	void* paddingNone(int8* CrrBlk, const uint32 Size)
	{
		if (UsedByteSlots < ByteSlots)
		{
			CrrBlk += Size;		//Skipping the padded data
		}
		return CrrBlk + 1;
	}

	//Delegated Functions Body: Overwrite Behaviour
	void* overwriteFirst(void* InsData, const uint32 Size)
	{
		memcpy(PoolData, InsData, Size);
		return PoolData;
	}
	void* overwriteLast(void* InsData, const uint32 Size)
	{
		void* Last = PoolData + (ByteSlots - Size);
		memcpy(Last, InsData, Size);
		return (void*)Last;
	}
	void* overwriteSearch(void* InsData, const uint32 Size)
	{
		if (LinkNext)
		{
			return LinkNext->insertData(InsData, Size);
		}
		else if (LinkPrev)
		{
			return LinkPrev->insertData(InsData, Size);
		}
		return nullptr;
	}
	void* overwriteRequest(void* InsData, const uint32 Size)
	{
		if (!ExtraRequests)
		{
			ExtraRequests = new ListPointedManager<int8>;
		}
		//Checking padding requirements
		if (PaddingFN == &MemoryPool::paddingX86)
		{
			if (Size % 4)
			{
				ExtraRequests->addData(new int8[Size]);
				ByteSlots += Size;
			}
			else
			{
				ExtraRequests->addData(new int8[Size + 4]);
				ByteSlots += Size + 4;
			}
		}
		else if (PaddingFN == &MemoryPool::paddingShieldedX86)
		{
			ExtraRequests->addData(new int8[Size + 4]);
			ByteSlots += Size + 4;
		}
		else if (PaddingFN == &MemoryPool::paddingX64)
		{
			if (Size % 8)
			{
				ExtraRequests->addData(new int8[Size]);
				ByteSlots += Size;
			}
			else
			{
				ExtraRequests->addData(new int8[Size + 8]);
				ByteSlots += Size + 8;
			}
		}
		else if (PaddingFN == &MemoryPool::paddingShieldedX64)
		{
			ExtraRequests->addData(new int8[Size + 8]);
			ByteSlots += Size + 8;
		}
		else if (PaddingFN == &MemoryPool::paddingSingle)
		{
			ExtraRequests->addData(new int8[Size + 1]);
			ByteSlots += Size + 1;
		}
		else	//Padding: None
		{
			ExtraRequests->addData(new int8[Size]);
			ByteSlots += Size;
		}
		UsedByteSlots += Size;
		memcpy(ExtraRequests->getTail()->getClassDataPointer(), InsData, Size);
		return (void*)ExtraRequests->getTail()->getClassDataPointer();
	}
	void* overwriteDont(void* InsData, const uint32 Size)
	{
		return nullptr;
	}

	//Delegated Function Helper
	void* Overwriter(void* InsData, const uint32 Size)
	{
		return (this->*OverwriteFN)(InsData, Size);
	}
	int8* Padder(int8* MemBlk, const uint32 Size)
	{
		return (int8*)(this->*PaddingFN)(MemBlk, Size);
	}

public:
	MemoryPool(const uint32 PoolByteSize) : ByteSlots(PoolByteSize),UsedByteSlots(0)
	{ 
		poolDataAlloc();
		PaddingFN = &MemoryPool::paddingX86;
		OverwriteFN = &MemoryPool::overwriteDont;
	}
	MemoryPool(const uint32 Amount, const PoolOverwriteBehaviour Behaviour) : ByteSlots(Amount), UsedByteSlots(0)
	{ 
		poolDataAlloc();
		changeInsertionMethod(Behaviour);
		PaddingFN = &MemoryPool::paddingX86;
	}
	MemoryPool(const uint32 Amount, const PoolPaddingBehaviour Behaviour) : ByteSlots(Amount), UsedByteSlots(0)
	{
		poolDataAlloc();
		changePaddingMethod(Behaviour);
		OverwriteFN = &MemoryPool::overwriteDont;
	}
	MemoryPool(const uint32 Amount, const PoolOverwriteBehaviour OvrBehaviour, const PoolPaddingBehaviour PdgBehaviour) : ByteSlots(Amount), UsedByteSlots(0)
	{
		poolDataAlloc();
		changeInsertionMethod(OvrBehaviour);
		changePaddingMethod(PdgBehaviour);
	}
	~MemoryPool() 
	{ 
		delete[] PoolData;
		NextBlock = PoolData = nullptr;
		LinkNext = LinkPrev = nullptr;
		PaddingFN = nullptr;
		OverwriteFN = nullptr;
		if (ExtraRequests)
		{
			//delete ExtraRequests;			//Is the same as 'destroyList()'
			ExtraRequests->destroyList();
			ExtraRequests = nullptr;
		}
	}

	void changeInsertionMethod(PoolOverwriteBehaviour Behaviour)
	{
		switch (Behaviour)
		{
		case MWMP_OVERWRITE_DONT_INSERT:
			OverwriteFN = &MemoryPool::overwriteDont;
			break;
		case MWMP_OVERWRITE_FIRST_ENTRY:
			OverwriteFN = &MemoryPool::overwriteFirst;
			break;
		case MWMP_OVERWRITE_LAST_ENTRY:
			OverwriteFN = &MemoryPool::overwriteLast;
			break;
		case MWMP_OVERWRITE_SEARCH_IN_LINK:
			OverwriteFN = &MemoryPool::overwriteSearch;
			break;
		case MWMP_OVERWRITE_REQUEST_MORE:
			OverwriteFN = &MemoryPool::overwriteRequest;
			break;
		}
	}
	void changePaddingMethod(PoolPaddingBehaviour Behaviour)
	{
		switch (Behaviour)
		{
		case MWMP_PADDING_X86:
			PaddingFN = &MemoryPool::paddingX86;
			break;
		case MWMP_PADDING_X86_SHIELDED:
			PaddingFN = &MemoryPool::paddingShieldedX86;
			break;
		case MWMP_PADDING_X64:
			PaddingFN = &MemoryPool::paddingX64;
			break;
		case MWMP_PADDING_X64_SHIELDED:
			PaddingFN = &MemoryPool::paddingShieldedX64;
			break;
		case MWMP_PADDING_SINGLE_BYTE:
			PaddingFN = &MemoryPool::paddingSingle;
			break;
		case MWMP_PADDING_NO_PADDING:
			PaddingFN = &MemoryPool::paddingNone;
			break;
		}
	}

	template <class dt>
	void* insertData(dt* Data, const uint32 DataSize)
	{
		if (canInsert(DataSize))
		{
			void* Dt = NextBlock;
			//Read as: COPY/(OVER)WRITE to 'NextBlock', the info FROM 'Data', the NUMBER OF BYTES of 'DataChunk' (the data type)
			memcpy(NextBlock, Data, DataSize);
			UsedByteSlots += DataSize;
			NextBlock = Padder(NextBlock, DataSize);
			return Dt;
		}
		//If we fall here, simply DON'T update 'NextBlock' address. Maybe with this 'DataSize' we cannot insert, but with a smaller one we could...
		void* DtLocation = Overwriter(Data, DataSize);
		Padder((int8*)DtLocation, DataSize);
		return DtLocation;
	}
	void removeData(void* Data, const uint32 DataSize)
	{
		//TODO: Data protection: The pointer MUST ONLY POINT to a valid MEMORY POOL block
		//TODO: A true generic remove/regroup method

		//When we remove data, we are actually copying everything after that pointer to where we want to remove, and set the remaning blocks to SignatureFreeBlock
		//Read as: "Move to address 'Data' everything after this 'Data' (Data + it's size), which everything have a size of the amount free + 'DataSize'
		memcpy(Data, (int8*)Data + DataSize, getMemoryPoolFreeBytes() + DataSize);
		NextBlock = PoolData + (UsedByteSlots - DataSize);
		memset(NextBlock, SignatureFreeBlock, DataSize);
		UsedByteSlots -= DataSize;
	}
	void defragPool(void)
	{
		//Search for bytes that doesn't have the 'SignatureFreeBlock' value and order the memory 
		int8* FreeBlock = nullptr;
		uint32 UsedSpace = 0;
		for (uint32 i = 0; i < ByteSlots; i++)
		{
			if (PoolData[i] != SignatureFreeBlock)
			{
				if (FreeBlock)	//We have fragmented memory?
				{
					//Guess it's size (with padding)
					uint32 Size = 1;
					uint32 j = ++i;
					while (PoolData[j] != SignatureFreeBlock && j < ByteSlots)
					{
						j++;
						Size++;
					}
					//Move this memory to 'FreeBlock' address, and set 'PoolData[i]' to 'SignatureFreeBlock'
					memcpy(FreeBlock, (void*)PoolData[i], Size);
					memset((void*)PoolData[i], SignatureFreeBlock, Size);
					//To speed up the search process, skip 'i' to last 'j' address, since we already tested those
					i = j;
					UsedSpace += Size;
				}
			}
			else	//We have fragmented memory
			{
				FreeBlock = &PoolData[i];
			}
			
		}
		//Update the current used space
		UsedByteSlots = UsedSpace;
	}

	int getMemoryPoolSizeBytes(void)
	{
		return ByteSlots;
	}
	int getMemoryPoolUsedBytes(void)
	{
		return UsedByteSlots;
	}
	int getMemoryPoolFreeBytes(void)
	{
		return ByteSlots - UsedByteSlots;
	}
	int getFreeBlocks(void)
	{
		int8*  Start = PoolData;
		uint32 FreeBlocks = 0;
		while (Start != SignaturePoolEnd)
		{
			if (*Start == SignatureFreeBlock)
			{
				FreeBlocks++;
			}
			Start++;
		}
		return FreeBlocks;
	}
	int getPaddingBlocks(void)
	{
		int8*  Start = PoolData;
		uint32 Paddings = 0;
		while (Start != SignaturePoolEnd)
		{
			if (!*Start)
			{
				Paddings++;
			}
			Start++;
		}
		return Paddings;
	}
	MemoryPool* getPoolLinkNext(void) const
	{
		return LinkNext;
	}
	MemoryPool* getPoolLinkPrevious(void) const
	{
		return LinkPrev;
	}
	void setPoolLinkNext(MemoryPool* Pool)
	{
		if (Pool)
		{
			LinkNext = Pool;
			Pool->LinkPrev = this;
		}
	}
	void setPoolLinkPrevious(MemoryPool* Pool)
	{
		if (Pool)
		{
			LinkPrev = Pool;
			Pool->LinkNext = this;
		}
	}
	void unlinkNextPool(void)
	{
		if (LinkNext)
		{
			LinkNext->LinkPrev = nullptr;
			LinkNext = nullptr;
		}
	}
	void unlinkPreviousPool(void)
	{
		if (LinkPrev)
		{
			LinkPrev->LinkNext = nullptr;
			LinkPrev = nullptr;
		}
	}

	//Debug exclusive functions --------------------------------------------------
#ifdef MWMP_DEBUG
	void d_printMemoryPoolSize(void)
	{
		uint32 MemoryB  = getMemoryPoolSizeBytes();
		uint32 MemoryKB = MemoryB  / 1024;
		uint32 MemoryMB = MemoryKB / 1024;
		std::cout << "[MWMP] - Current Size: [" << MemoryB << "-B] [" << MemoryKB << "-KB] [" << MemoryMB << "-MB] [" << MemoryMB / 1024 << "-GB]" << std::endl;
	}
	void d_printMemoryPoolUsed(void)
	{
		uint32 MemoryB = getMemoryPoolSizeBytes();
		uint32 MemoryKB = MemoryB / 1024;
		uint32 MemoryMB = MemoryKB / 1024;
		std::cout << "[MWMP] - Current Used: [" << MemoryB << "-B] [" << MemoryKB << "-KB] [" << MemoryMB << "-MB] [" << MemoryMB / 1024 << "-GB]" << std::endl;
	}
	void d_printMemoryPoolFree(void)
	{
		uint32 MemoryB = getMemoryPoolSizeBytes();
		uint32 MemoryKB = MemoryB / 1024;
		uint32 MemoryMB = MemoryKB / 1024;
		std::cout << "[MWMP] - Current Free: [" << MemoryB << "-B] [" << MemoryKB << "-KB] [" << MemoryMB << "-MB] [" << MemoryMB / 1024 << "-GB]" << std::endl;
	}
	void d_printMemoryOverall(void)
	{
		std::cout << "[MWMP] - Overall Memory Usage:" << std::endl;
		d_printMemoryPoolSize();
		d_printMemoryPoolUsed();
		d_printMemoryPoolFree();
	}
#endif
	//Debug exclusive functions --------------------------------------------------
};

#endif;	//Memory Pool Library