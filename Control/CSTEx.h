/*-------Custom Types Extended-------
*	Extend functionallity for the CST library
*
*
*	Moon Wiz Studios (c) - 16/08/2015
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.5.0
*/

#ifndef _H_CSTEX_
#define _H_CSTEX_

#include <stdio.h>

#include "CustomTypes-SA.h"

namespace BitOperations
{
#ifdef _INC_STDIO	//If <stdio.h> is included, allow print function
	template <typename T>
	void PrintBinary(T src, BitOperationsEndian Endianess)
	{
		uint16 Size = sizeof(T);
		uint16 Bits = Size * 8;
		if (Endianess == BITOPERATIONS_LITTLE_ENDIAN)
		{
			printf("[LITTLE ENDIAN]Binary of 'src' = [%d] Bytes (%d bits):\n", Size, Bits);
			for (int i = Bits - 1; i >= 0; i -= 2)
			{
				printf("%d%d ", IsBitSet(src, i), IsBitSet(src, i - 1));
			}
			printf("\n");
		}
		else			
		{
			printf("[BIG ENDIAN]Binary of 'src' = [%d] Bytes (%d bits):\n", Size, Bits);
			for (int i = 0; i < Bits; i += 2)
			{
				printf("%d%d ", IsBitSet(src, i), IsBitSet(src, i + 1));
			}
			printf("\n");
		}
	}
#endif
	void PrintBinary(uint64 src, BitOperationsEndian Endianess)
	{
		uint16 Size = sizeof(long long);
		uint16 Bits = Size * 8;
		if(Endianess == BITOPERATIONS_LITTLE_ENDIAN)
		{
			printf("[LITTLE ENDIAN]Binary of 'src' = %llu [%d] Bytes (%d bits):\n", src, Size, Bits);
			for(int i = Bits - 1; i >= 0; i -= 2)
			{
				printf("%d%d ", IsBitSet(src, i), IsBitSet(src, i - 1));
			}
			printf("\n");
		}
		else
		{
			printf("[BIG ENDIAN]Binary of 'src' = %llu [%d] Bytes (%d bits):\n", src, Size, Bits);
			for(int i = 0; i < Bits; i += 2)
			{
				printf("%d%d ", IsBitSet(src, i), IsBitSet(src, i + 1));
			}
			printf("\n");
		}
	}
	void PrintBinary(int64 src, BitOperationsEndian Endianess)
	{
		uint16 Size = sizeof(long long);
		uint16 Bits = Size * 8;
		if(Endianess == BITOPERATIONS_LITTLE_ENDIAN)
		{
			printf("[LITTLE ENDIAN]Binary of 'src' = %lld [%d] Bytes (%d bits):\n", src, Size, Bits);
			for(int i = Bits - 1; i >= 0; i -= 2)
			{
				printf("%d%d ", IsBitSet(src, i), IsBitSet(src, i - 1));
			}
			printf("\n");
		}
		else
		{
			printf("[BIG ENDIAN]Binary of 'src' = %lld [%d] Bytes (%d bits):\n", src, Size, Bits);
			for(int i = 0; i < Bits; i += 2)
			{
				printf("%d%d ", IsBitSet(src, i), IsBitSet(src, i + 1));
			}
			printf("\n");
		}
	}
}

namespace Utils
{
#ifdef _INC_STDIO	//If <stdio.h> is included, allow print function
	template <typename T>
	void inline PrintArray(T *in, int size, int returnAfter, const char* format)
	{
		for(int i = 0; i < size; i++)
		{
			printf(format, in[i]);
			if(!((i + 1) % returnAfter))
			{
				printf("\n");
			}
		}
	}
	template <typename T>
	void inline PrintArray(T *in, int size, const char *format)
	{
		for(int i = 0; i < size; i++)
		{
			printf(format, in[i]);
		}
	}
	template <typename T>
	void inline PrintMat(T **in, int row, int col, const char *format)
	{
		for(int i = 0; i < row; i++)
		{
			for(int j = 0; j < col; j++)
			{
				printf(format, in[i][j]);
			}
			printf("\n");
		}
	}
#endif
}

#endif