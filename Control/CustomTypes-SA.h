/*-------Custom Types-------
*	Stand-alone custom variable types and useful/base classes
*	--Typedef large variables to small ones, with under/over-flow limit description, byte consumption and handy classes
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
*	Version 1.8.9
*/

#ifndef _H_CSTTYPE_
#define _H_CSTTYPE_

//Custom variables types and classes for easy use

//Math and Constants
#define CST_PI		3.141592653589793				//
#define CST_2PI		6.283185307179586				//
#define CST_PI_180	0.017453292516666				// PI / 180
#define CST_180_PI	57.29577951308232				// 180 / PI
#define CST_EULER	2.718281828459045				//
#define CST_KB		1024							// Byte / CST_KB = KB
#define CST_MB		1048576							// Byte / CST_MB = MB
#define CST_GB		1073741824						// Byte / CST_GB = GB
#define CST_TB		1099511627776					// Byte / CST_TB = TB
//Keyboard Key codes
#define CST_KEY_BACKSPACE		8					//
#define CST_KEY_TAB				9					//
#define CST_KEY_ENTER			13					//
#define CST_KEY_ESC				27					//
#define CST_KEY_SPACE			32					//
#define CST_KEY_DOUBLE_QUOTE	34					//
#define CST_KEY_SINGLE_QUOTE	39					//
#define CST_KEY_DELETE			127					//
//External Macros
#define CST_TYPE_NONE			 0					//
#define CST_TYPE_SUCCESS		 1					//
#define CST_TYPE_WARNING		 2					//
#define CST_TYPE_ERROR			 3					//
#define CST_TYPE_EXCEPTION		 4					//
//Variables Limits
#define CST_INT8_MAX			 127				//
#define CST_INT8_MIN     		-128
#define CST_UINT8_MAX			 255
#define CST_INT16_MAX		    32767
#define CST_INT16_MIN		   -32768
#define CST_UINT16_MAX			65535
#define CST_INT32_MAX		  2147483647
#define CST_INT32_MIN		 -2147483648
#define CST_UINT32_MAX		  4294967295	
#define CST_INT64_MAX     9223372036854775807		
#define CST_INT64_MIN	 -9223372036854775808
#define CST_UINT64_MAX	 18446744073709551615
//Variables Digits Count
#define CST_DIGITS_INT8			 4			//Digits with minus sign consideration
#define CST_DIGITS_INT16		 6			//Digits with minus sign consideration
#define CST_DIGITS_INT32		11			//Digits with minus sign consideration
#define CST_DIGITS_INT64		20			//Digits with minus sign consideration

//Unsigned Types --------------------------------------------------------------------------------------------------
typedef unsigned char				uint8;		//Range > 0 to 255 (1 byte / BYTE)
typedef unsigned short				uint16;		//Range > 0 to 65,535 (2 bytes / WORD)
//typedef unsigned short int		uint16;		//Range > 0 to 65,535 (2 bytes / WORD)
typedef unsigned int				uint32;		//Range > 0 to 4,294,967,295 (4 bytes / DWORD)
//typedef unsigned long int			uint32;		//Range > 0 to 4,294,967,295 (4 bytes / DWORD)
typedef unsigned long long			uint64;		//Range > 0 to 18,446,744,073,709,551,615 (8 bytes / QWORD)
//typedef unsigned long long int	uint64;		//Range > 0 to 18,446,744,073,709,551,615 (8 bytes / QWORD)

//Signed Types ----------------------------------------------------------------------------------------------------
typedef signed char					int8;		//Range > -128 to 127 (1 byte / BYTE)
typedef signed short				int16;		//Range > –32,768 to 32,767 (2 bytes / WORD)
//typedef signed short int			int16;		//Range > –32,768 to 32,767 (2 bytes / WORD)
typedef signed int					int32;		//Range > –2,147,483,648 to 2,147,483,647 (4 bytes / DWORD)
//typedef signed long int			int32;		//Range > –2,147,483,648 to 2,147,483,647 (4 bytes / DWORD)
typedef signed long long			int64;		//Range > –9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 (8 bytes / QWORD)
//typedef signed long long int		int64;		//Range > –9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 (8 bytes / QWORD)

#ifdef WIN32
//Plataform especific variables
typedef unsigned __int64			uint64;		//Range > 0 to 18,446,744,073,709,551,615 (8 bytes / QWORD)
typedef signed __int64				int64;		//Range > –9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 (8 bytes / QWORD)

#endif

//Namespaces and worker classes	//	------------------------------------------------------------------------------------------------------

enum BitOperationsExtractByte
{
	BITOPERATIONS_BYTE_EXTRACT_FIRST,
	BITOPERATIONS_BYTE_EXTRACT_SECOND,
	BITOPERATIONS_BYTE_EXTRACT_THIRD,
	BITOPERATIONS_BYTE_EXTRACT_FORTH
};

enum BitOperationsEndian
{
	BITOPERATIONS_BIG_ENDIAN,	//Most significant bit (generaly the 'sign' bit) stays at the front
	BITOPERATIONS_LITTLE_ENDIAN	//Most significant bit (generaly the 'sign' bit) stays at the back
};

namespace Tables
{
	inline uint64 Fatorial(int i)
	{
		switch (i)
		{
			default:
			case 0:
			case 1:
				return 1;
			case 2:
				return 2;
			case 3:
				return 6;
			case 4:
				return 24;
			case 5:
				return 120;
			case 6:
				return 720;
			case 7:
				return 5040;
			case 8:
				return 40320;
			case 9:
				return 362880;
			case 10:
				return 3628800;
			case 11:
				return 39916800;
			case 12:
				return 479001600;
			case 13:
				return 6227020800;
			case 14:
				return 87178291200;
			case 15:
				return 1307674368000;
			case 16:
				return      20922789888000;
			case 17:
				return     355687428096000;
			case 18:
				return    6402373705728000;
			case 19:
				return  121645100408832000;
			case 20:
				return 2432902008176640000;
		}
	}
}

namespace BitOperations
{
	inline uint8 ExtractFromUInt32(uint32 src, BitOperationsExtractByte Byte, BitOperationsEndian Endianess)
	{
		if (Endianess == BITOPERATIONS_LITTLE_ENDIAN)
		{
			switch (Byte)
			{
			case BITOPERATIONS_BYTE_EXTRACT_FIRST:
				return ((src & 0xFF000000) >> 24);
			case BITOPERATIONS_BYTE_EXTRACT_SECOND:
				return ((src & 0x00FF0000) >> 16);
			case BITOPERATIONS_BYTE_EXTRACT_THIRD:
				return ((src & 0x0000FF00) >> 8);
			case BITOPERATIONS_BYTE_EXTRACT_FORTH:
				return ((src & 0x000000FF) >> 0);
			}
		}
		switch (Byte)
		{
		case BITOPERATIONS_BYTE_EXTRACT_FIRST:
			return ((src & 0xFF000000) >> 0);
		case BITOPERATIONS_BYTE_EXTRACT_SECOND:
			return ((src & 0x00FF0000) >> 8);
		case BITOPERATIONS_BYTE_EXTRACT_THIRD:
			return ((src & 0x0000FF00) >> 16);
		case BITOPERATIONS_BYTE_EXTRACT_FORTH:
			return ((src & 0x000000FF) >> 24);
		}
		return 0;	//Invalid parameters
	}
	inline int8 ExtractFromInt32(int32 src, BitOperationsExtractByte Byte, BitOperationsEndian Endianess)
	{
		if (Endianess == BITOPERATIONS_LITTLE_ENDIAN)
		{
			switch (Byte)
			{
			case BITOPERATIONS_BYTE_EXTRACT_FIRST:
				return ((src & 0xFF000000) >> 24);
			case BITOPERATIONS_BYTE_EXTRACT_SECOND:
				return ((src & 0x00FF0000) >> 16);
			case BITOPERATIONS_BYTE_EXTRACT_THIRD:
				return ((src & 0x0000FF00) >> 8);
			case BITOPERATIONS_BYTE_EXTRACT_FORTH:
				return ((src & 0x000000FF) >> 0);
			}
		}
		switch (Byte)
		{
		case BITOPERATIONS_BYTE_EXTRACT_FIRST:
			return ((src & 0xFF000000) >> 0);
		case BITOPERATIONS_BYTE_EXTRACT_SECOND:
			return ((src & 0x00FF0000) >> 8);
		case BITOPERATIONS_BYTE_EXTRACT_THIRD:
			return ((src & 0x0000FF00) >> 16);
		case BITOPERATIONS_BYTE_EXTRACT_FORTH:
			return ((src & 0x000000FF) >> 24);
		}
		return 0;
	}
	inline uint8* ExtractFromUInt32(uint32 src, BitOperationsEndian Endianess)
	{
		uint8 *Out = new uint8[4];
		if (Endianess == BITOPERATIONS_LITTLE_ENDIAN)
		{
			Out[0] = (src & 0xFF000000) >> 24;
			Out[1] = (src & 0x00FF0000) >> 16;
			Out[2] = (src & 0x0000FF00) >> 8;
			Out[3] = (src & 0x000000FF);	// >> 0
			return Out;
		}
		Out[3] = (src & 0xFF000000) >> 24;
		Out[2] = (src & 0x00FF0000) >> 16;
		Out[1] = (src & 0x0000FF00) >> 8;
		Out[0] = (src & 0x000000FF);		// >> 0
		return Out;
	}
	inline 	int8* ExtractFromInt32(int32 src, BitOperationsEndian Endianess)
	{
		int8 *Out = new int8[4];
		if (Endianess == BITOPERATIONS_LITTLE_ENDIAN)
		{
			Out[0] = (src & 0xFF000000) >> 24;
			Out[1] = (src & 0x00FF0000) >> 16;
			Out[2] = (src & 0x0000FF00) >> 8;
			Out[3] = (src & 0x000000FF);
			return Out;
		}
		Out[3] = (src & 0xFF000000) >> 24;
		Out[2] = (src & 0x00FF0000) >> 16;
		Out[1] = (src & 0x0000FF00) >> 8;
		Out[0] = (src & 0x000000FF);
		return Out;
	}
	inline uint32 ComputeUInt32(uint8 *src, BitOperationsEndian Endianess)
	{	// !! (WIP) !! Verify if this is the correct order for BIG ENDIAN
		uint32 Out = 0;
		if (Endianess == BITOPERATIONS_BIG_ENDIAN)
		{
			Out = (src[0] << 0) | (src[1] << 8) | (src[2] << 16) | (src[3] << 24);
			return Out;
		}
		Out = (src[3] << 0) | (src[2] << 8) | (src[1] << 16) | (src[0] << 24);
		return Out;
	}
	inline int32 ComputeInt32(int8 *src, BitOperationsEndian Endianess)
	{
		int32 Out = 0;
		if (Endianess == BITOPERATIONS_BIG_ENDIAN)
		{
			Out = (src[0] << 0) | (src[1] << 8) | (src[2] << 16) | (src[3] << 24);
			return Out;
		}
		Out = (src[3] << 0) | (src[2] << 8) | (src[1] << 16) | (src[0] << 24);
		return Out;
	}

	template <typename T>
	inline void SetBit(T& src, uint8 Bit)
	{
		src |= (1 << Bit);
	}
	inline void SetBit(int64 src, uint8 Bit)
	{
		src |= (1LL << Bit);
	}
	inline void SetBit(uint64 src, uint8 Bit)
	{
		src |= (1LLU << Bit);
	}
	template <typename T>
	inline void SetBitSafe(T& src, uint8 Bit)
	{
		if (Bit < (sizeof(T) * 8))
		{
			SetBit(src, Bit);
		}
	}

	template <typename T>
	inline void UnSetBit(T& src, uint8 Bit)
	{
		src &= ~(1 << Bit);
	}
	inline void UnSetBit(int64 src, uint8 Bit)
	{
		src &= ~(1LL << Bit);
	}
	inline void UnSetBit(uint64 src, uint8 Bit)
	{
		src &= ~(1LLU << Bit);
	}
	template <typename T>
	inline void UnSetBitSafe(T& src, uint8 Bit)
	{
		if (Bit < (sizeof(T) * 8))
		{
			UnSetBit(src, Bit);
		}
	}

	template <typename T>
	inline void ToggleBit(T& src, uint8 Bit)
	{
		src ^= (1 << Bit);
	}
	inline void ToggleBit(int64 src, uint8 Bit)
	{
		src ^= (1LL << Bit);
	}
	inline void ToggleBit(uint64 src, uint8 Bit)
	{
		src ^= (1LLU << Bit);
	}
	template <typename T>
	inline void ToggleBitSafe(T& src, uint8 Bit)
	{
		if (Bit < (sizeof(T) * 8))
		{
			ToggleBit(src, Bit);
		}
	}

	template <typename T>
	T ComputeEquivalent(const char *bitString)
	{
		T src = 0;
		int bitSize = sizeof(T) * 8;
		int redct = bitSize - 1;
		int i = 0;
		for (; i < bitSize && bitString[i]; i++)
		{
			if (bitString[i] == '1')
			{
				SetBit(src, redct - i);
			}
			else
			{
				UnSetBit(src, redct - i);
			}
		}
		while (i++ < bitSize)
		{
			UnSetBit(src, redct - i);
		}
		return src;
	}

	template <typename T>
	inline bool IsBitSet(T Ref, uint16 Bit)
	{
		return Ref & (1 << Bit);
	}
	inline bool IsBitSet(int64 Ref, uint16 Bit)
	{
		return Ref & (1LL << Bit);
	}
	inline bool IsBitSet(uint64 Ref, uint16 Bit)
	{
		return Ref & (1LLU << Bit);
	}
	template <typename T>
	inline bool IsBitSetSafe(T Ref, uint16 Bit)
	{
		if (Bit < (sizeof(T) * 8))
		{
			return IsBitSet(Ref, Bit);
		}
		return false;	//The bit range is too big for testing, returning false...
	}

#ifdef _INC_STDIO	//If <stdio.h> is included, allow print function
	template <typename T>
	void PrintBinary(T src, BitOperationsEndian Endianess)
	{
		uint16 Size = sizeof(T);
		uint16 Bits = Size * 8;
		uint16 Ops = (Size << 1) - 1;
		if (Endianess == BITOPERATIONS_BIG_ENDIAN)
		{
			printf("[BIG ENDIAN]Binary of 'src' = %d [%d] Bytes (%d bits):\n", src, Size, Bits);
			printf("[");
			for (int i = Bits - 1, j = 0; i >= 0; i -= 4, j++)
			{
				printf("%d%d%d%d", IsBitSet(src, i), IsBitSet(src, i - 1), IsBitSet(src, i - 2), IsBitSet(src, i - 3));
				if (!(j - Ops))
				{
					break;
				}
				else if (j && (j % 2))
				{
					printf("][");
				}
				else
				{
					printf(" ");
				}
			}
			printf("]\n");
		}
		else
		{
			printf("[LITTLE ENDIAN]Binary of 'src' = %d [%d] Bytes (%d bits):\n", src, Size, Bits);
			printf("[");
			for (int i = 0, j = 0; i < Bits; i += 4, j++)
			{
				printf("%d%d%d%d", IsBitSet(src, i), IsBitSet(src, i + 1), IsBitSet(src, i+2), IsBitSet(src, i + 3));
				if (!(j - Ops))
				{
					break;
				}
				else if (j && (j % 2))
				{
					printf("][");
				}
				else
				{
					printf(" ");
				}
			}
			printf("]\n");
		}
	}
#endif
}

namespace Utils
{
	inline bool isOdd(int N)
	{
		// N -> XX XX XX XY
		// 1 -> 00 00 00 01
		//      ----------- [&]
		//	    XX XX XX XZ, where 'Z' is either 1 or 0 if the bit 'Y' is 1 or 0, respectively
		return (N & 1);
	}
	inline bool isEven(int N)
	{
		return !isOdd(N);
	}
	inline bool haveDecimal(double N)
	{
		return (N - (int)N);
	}
	inline bool haveDifSignals(double A, double B)
	{
		return (A * B) < 0;
	}

	inline int countDigits(int N)
	{
		if (N)	//Testing against 0
		{
			return 1 + countDigits(N * 0.1);
		}
		return 0;
	}

	inline int matArrayIndex(int row, int column, int rowSz)
	{
		return column + (rowSz * row);
	}
	template <class T>
	inline T matArrayIndex(int row, int column, int rowSz, T *in)
	{
		return in[matArrayIndex(row, column, rowSz)];
	}

	inline int sumDigits(int n)
	{
		if (n)
		{
			return n % 10 + sumDigits(n * 0.1);
		}
		return 0;
	}
	inline int getDigit(int input, int n)
	{
		if (input > 9 && n > 1)
		{
			return getDigit(input * 0.1, n - 1);
		}
		return input % 10;
	}

	inline double degToRad(double D)
	{
		return D * CST_PI_180;
	}
	inline double radToDeg(double R)
	{
		return R * CST_180_PI;
	}

	inline uint64 DecToBin(uint16 n)
	{
		if (n <= 1)
		{
			return n;
		}
		return DecToBin(n >> 1) * 10 + n % 2;
	}
	inline bool Cycle(int cont, int every)
	{
		if (every)
		{
			return !((cont + 1) % every);
		}
		return false;
	}
	template <typename T>
	inline void Swap(T &A, T &B)
	{
		T temp = A;
		A = B;
		B = temp;
	}
	inline void Swap(int &A, int &B)
	{
		A ^= B;
		B ^= A;
		A ^= B;
	}

	template <class T>
	inline T** matrixAllocate(int rows, int columns)
	{
		T **out = new T*[rows]();
		for (int i = 0; i < rows; i++)
		{
			out[i] = new T[columns]();
		}
		return out;
	}
	template <class T>
	inline void matrixDispose(T **matrix, int rows)
	{
		for (int i = 0; i < rows; i++)
		{
			delete[] matrix[i];
		}
		delete[] matrix;
	}
	template <class T>
	inline void matrixFill(T **matrix, int rows, int columns, double st, double inc)
	{
		double val = st;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				matrix[i][j] = val;
				val += inc;
			}
		}
	}
	template <class T>
	inline void arrayFill(T *arr, int sz, double st, double inc)
	{
		double val = st;
		for (int i = 0; i < sz; i++)
		{
			arr[i] = val;
			val += inc;
		}
	}

	template <typename T>
	inline T Abs(T &X)
	{
		return X > 0 ? X : -X;
	}
	inline uint64 Abs(int64 &X)
	{
		const int mask = X >> ((sizeof(int64) * 8) - 1);
		return (X + mask) ^ mask;
	}
	inline uint32 Abs(int32 &X)
	{
		const int mask = X >> ((sizeof(int32) * 8) - 1);
		return (X + mask) ^ mask;
	}
	inline uint16 Abs(int16 &X)
	{
		const int mask = X >> ((sizeof(int16) * 8) - 1);
		return (X + mask) ^ mask;
	}
	inline uint8 Abs(int8 &X)
	{
		const int mask = X >> ((sizeof(int8) * 8) - 1);
		return (X + mask) ^ mask;
	}
	template <class T>
	inline void SafeDelete(T **mem)
	{
		if (*mem)
		{
			delete *mem;
			*mem = nullptr;
		}
	}

#ifdef _INC_STDIO	//If <stdio.h> is included, allow these functions
	template <typename T>
	void inline PrintArray(T *in, int size, int returnAfter, const char* format)
	{
		for (int i = 0; i < size; i++)
		{
			printf(format, in[i]);
			if (!((i + 1) % returnAfter))
			{
				printf("\n");
			}
		}
	}
	template <typename T>
	void inline PrintArray(T *in, int size, const char *format)
	{
		for (int i = 0; i < size; i++)
		{
			printf(format, in[i]);
		}
	}
	template <typename T>
	void inline PrintMat(T **in, int row, int col, const char *format)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				printf(format, in[i][j]);
			}
			printf("\n");
		}
	}

	bool endOfFile(FILE *file)
	{
		char c = fgetc(file);
		if (c != EOF)
		{
			fseek(file, -1, SEEK_CUR);
			return false;
		}
		return true;
	}
#endif
}

namespace CharOperations
{
	inline char getLowerCase(char);

	//Basic functions
	inline bool isLetter(char C)
	{
		return (C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z');
	}
	inline bool isNumber(char C)
	{
		return (C >= '0' && C <= '9');
	}
	inline bool isLowerCase(char C)
	{
		return (C >= 'a' && C <= 'z');
	}
	inline bool isUpperCase(char C)
	{
		return (C >= 'A' && C <= 'Z');
	}
	inline bool isVowel(char C)
	{
		switch (C)
		{
		case 'A':
		case 'a':
		case 'E':
		case 'e':
		case 'I':
		case 'i':
		case 'O':
		case 'o':
		case 'U':
		case 'u':
			return true;
		}
		return false;
	}
	inline bool isConsonant(char C)
	{
		return !isVowel(C);
	}
	inline bool isEqualSensitive(char C1, char C2)
	{
		return C1 == C2;
	}
	inline bool isWordSeparator(char C)
	{
		switch (C)
		{
		case ',':
		case '.':
		case ' ':
		case '?':
		case '!':
		case ':':
		case ';':
		case '\t':
		case '\n':
			return true;
		}
		return false;
	}
	//Returns 0 (false) if it isn't and 1..3 (all true) depending on the 'priority' of it
	//[3] -> '(' OR ')'
	//[2] -> '[' OR ']'
	//[1] -> '{' OR '}'
	//[0] -> Everything else
	inline int isOperatorSeparator(char C)
	{
		switch (C)
		{
		case '(':
		case ')':
			return 3;
		case '[':
		case ']':
			return 2;
		case '{':
		case '}':
			return 1;
		}
		return 0;
	}
	//Returns 0 (false) if it isn't and 1..3 (all true) depending on the precedence of it
	//[3] -> '^'
	//[2] -> '*' OR '/'
	//[1] -> '+' OR '-'
	//[0] -> Everything else
	inline int isOperatorMath(char C)
	{
		switch (C)
		{
		case '^':
			return 3;
		case '*':
		case '/':
			return 2;
		case '+':
		case '-':
			return 1;
		}
		return 0;
	}
	inline bool isOperator(char C)
	{
		return isOperatorSeparator(C) || isOperatorMath(C);
	}
	inline bool isAccented(char C)
	{
		switch (C)
		{
		case 'À':
		case 'Á':
		case 'Â':
		case 'Ã':
		case 'Ä':
		case 'Å':
		case 'Ç':
		case 'È':
		case 'É':
		case 'Ê':
		case 'Ë':
		case 'Ì':
		case 'Í':
		case 'Î':
		case 'Ï':
		case 'Ñ':
		case 'Ò':
		case 'Ó':
		case 'Ô':
		case 'Õ':
		case 'Ö':
		case 'Ù':
		case 'Ú':
		case 'Û':
		case 'Ü':
		case 'Ý':
		case 'à':
		case 'á':
		case 'â':
		case 'ã':
		case 'ä':
		case 'å':
		case 'ç':
		case 'è':
		case 'é':
		case 'ê':
		case 'ë':
		case 'ì':
		case 'í':
		case 'î':
		case 'ï':
		case 'ñ':
		case 'ò':
		case 'ó':
		case 'ô':
		case 'õ':
		case 'ö':
		case 'ù':
		case 'ú':
		case 'û':
		case 'ü':
		case 'ý':
			return true;
		}
		return false;
	}
	inline bool isEqualSensitive(char *S1, char *S2, uint16 length)
	{
		for (int i = 0; i < length; i++)
		{
			if (S1[i] != S2[i])
			{
				return false;	//Just say it's false and forget about it
			}
		}
		return true;
	}
	inline bool isEqualRangeSensitive(char *S1, char *S2, uint16 from, uint16 to)
	{
		for (int i = from; i < to; i++)
		{
			if (S1[i] != S2[i])
			{
				return false;
			}
		}
		return true;
	}
	inline bool isEqual(char C1, char C2)
	{
		//Since the chances of either 'C1' or 'C2' being lowercase is greater, let's check against it
		if (isLetter(C1) && isLetter(C2))
		{
			C1 = getLowerCase(C1);
			C2 = getLowerCase(C2);
		}
		return C1 == C2 ? true : false;
	}
	inline bool isEqual(char *S1, char *S2, uint16 length)
	{
		for (int i = 0; i < length; i++)
		{
			if (!isEqual(S1[i], S2[i]))
			{
				return false;
			}
		}
		return true;
	}
	inline bool isEqualRange(char *S1, char *S2, uint16 from, uint16 to)
	{
		for (int i = from; i < to; i++)
		{
			if (!isEqual(S1[i], S2[i]))
			{
				return false;
			}
		}
		return true;
	}

	inline int getAlphabeticalIndex(char C)
	{
		if (isLowerCase(C))
		{
			return C - 'a';
		}
		else if (isUpperCase(C))
		{
			return C - 'A';
		}
		return -1;
	}
	inline int getNumberEquivalent(char C)		//Returns -1 on failure to convert
	{
		if (isNumber(C))
		{
			return '0' ^ C;
		}
		return -1;
	}
	inline char getCharEquivalent(int N)		//Returns '\0' on failure to convert
	{
		switch (N)
		{
		case 0:
			return '0';
		case -1:
		case 1:
			return '1';
		case -2:
		case 2:
			return '2';
		case -3:
		case 3:
			return '3';
		case -4:
		case 4:
			return '4';
		case -5:
		case 5:
			return '5';
		case -6:
		case 6:
			return '6';
		case -7:
		case 7:
			return '7';
		case -8:
		case 8:
			return '8';
		case -9:
		case 9:
			return '9';
		default:
			return '\0';
		}
	}
	inline char getLowerCase(char C)
	{
		//return (C + 32);
		if (isLetter(C))
		{
			return C | ' ';
		}
		return C;
	}
	inline char getUpperCase(char C)
	{
		//return (C - 32);
		if (isLetter(C))
		{
			return C & '_';
		}
		return C;
	}

	inline int indexOfFirst(const char *str, char C)
	{
		for (int i = 0; str[i]; i++)
		{
			if (!(str[i] - C))
			{
				return i;
			}
		}
		return -1;
	}
	inline int indexOfLast(const char *str, char C)
	{
		int index = -1;
		for (int i = 0; str[i]; i++)
		{
			if (!(str[i] - C))
			{
				index = i;
			}
		}
		return index;
	}

	inline int countLowerCase(char *in)
	{
		int count = 0;
		for (int i = 0; in[i]; i++)
		{
			if (isLowerCase(in[i]))
			{
				count++;
			}
		}
		return count;
	}
	inline int countUpperCase(char *in)
	{
		int count = 0;
		for (int i = 0; in[i]; i++)
		{
			if (isUpperCase(in[i]))
			{
				count++;
			}
		}
		return count;
	}

	inline char removeAccent(char C)
	{
		switch (C)
		{
		case 'À':
		case 'Á':
		case 'Â':
		case 'Ã':
		case 'Ä':
		case 'Å':
			return 'A';
		case 'Ç':
			return 'C';
		case 'È':
		case 'É':
		case 'Ê':
		case 'Ë':
			return 'E';
		case 'Ì':
		case 'Í':
		case 'Î':
		case 'Ï':
			return 'I';
		case 'Ñ':
			return 'N';
		case 'Ò':
		case 'Ó':
		case 'Ô':
		case 'Õ':
		case 'Ö':
			return 'O';
		case 'Ù':
		case 'Ú':
		case 'Û':
		case 'Ü':
			return 'U';
		case 'Ý':
			return 'Y';
		case 'à':
		case 'á':
		case 'â':
		case 'ã':
		case 'ä':
		case 'å':
			return 'a';
		case 'ç':
			return 'c';
		case 'è':
		case 'é':
		case 'ê':
		case 'ë':
			return 'e';
		case 'ì':
		case 'í':
		case 'î':
		case 'ï':
			return 'i';
		case 'ñ':
			return 'n';
		case 'ò':
		case 'ó':
		case 'ô':
		case 'õ':
		case 'ö':
			return 'o';
		case 'ù':
		case 'ú':
		case 'û':
		case 'ü':
			return 'u';
		case 'ý':
			return 'y';
		}
		return C;
	}
	inline char* removeAccent(char *str)
	{
		for (int i = 0; str[i]; i++)
		{
			str[i] = removeAccent(str[i]);
		}
		return str;
	}

	inline char* numberToString(int N)
	{
		char *Out = new char[CST_DIGITS_INT32 + 1]();
		int i = 0;
		int Digits = Utils::countDigits(N);
		int DigitRegion = 1;		//Should NOT be unsigned!!
		if (Digits > 0)
		{
			for (int j = 0; j < Digits - 1; j++)
			{
				DigitRegion *= 10;
			}
			if (N < 0)
			{
				i++;
				Digits++;
				Out[0] = '-';
			}
			for (int Number; i < Digits; i++)
			{
				Number = N / DigitRegion;
				Out[i] = getCharEquivalent(Number);
				N = N - (Number * DigitRegion);
				DigitRegion *= 0.1;
			}
		}
		return Out;
	}
	inline char* numberToString(int N, uint8 Digits)
	{
		char *Out = new char[CST_DIGITS_INT32 + 1]();
		uint32 i = 0;
		int DigitRegion = 1;
		if (Digits == 0)							//No digits?
		{
			Digits = Utils::countDigits(N);	//Check if there are digits
			if (Digits == 0)						//There AREN'T digits, abort
			{
				return nullptr;
			}
		}
		for (int j = 0; j < Digits - 1; j++)
		{
			DigitRegion *= 10;
		}
		if (N < 0)
		{
			i++;
			Digits++;
			Out[0] = '-';
		}
		for (int Number; i < Digits; i++)
		{
			Number = N / DigitRegion;
			Out[i] = getCharEquivalent(Number);
			N = N - (Number * DigitRegion);
			DigitRegion *= 0.1;
		}
		return Out;
	}

	inline char* substringBetween(const char *str, char start, char end)
	{
		char *subs = nullptr;
		int size = 0;
		for (int i = 0; str[i]; i++, size++) { }
		if (size)
		{
			subs = new char[size]();
			for (int i = 0, j = 0, flag = 0; str[i]; i++)
			{
				if (flag)
				{
					if (str[i] == end)
					{
						break;
					}
					subs[j++] = str[i];
				}
				if (str[i] == start)
				{
					flag = 1;
				}
			}
		}
		return subs;
	}
}

namespace MemoryOperations
{
	template <typename T>
	inline bool memIsAnySet(void* Tgt, T Val, uint32 Size)
	{
		T* Tester = (T*)Tgt;
		for (int i = 0; i < Size; i++)
		{
			if (Tester[i] == Val)
			{
				return true;
			}
		}
		return false;
	}
	template <typename T>
	inline bool memIsAllSet(void* Tgt, T Val, uint32 Size)
	{
		T* Tester = (T*)Tgt;
		for (int i = 0; i < Size; i++)
		{
			if (Tester[i] != Val)
			{
				return false;
			}
		}
		return true;
	}
}

//Basic Classes					//	------------------------------------------------------------------------------------------------------

class BaseString
{
protected:
	char *m_text;
	uint32 m_length;
	uint32 m_size;

public:
	BaseString(const char *str) : m_length(0)
	{
		int sz = 0;
		while(str[sz])
		{
			sz++;
		}
		m_size = ++sz;
		m_text = new char[sz];
		setString(str);
	}
	BaseString(uint32 size) : m_size(!size ? 1 : size), m_length(0)
	{
		m_text = new char[m_size]();
	}
	BaseString(uint32 size, const char *str) : m_size(!size ? 1 : size), m_length(0)
	{
		m_text = new char[m_size]();
		setString(str);
	}
	BaseString(const BaseString &str)
	{
		m_text = new char[str.m_size]();
		m_size = str.m_size;
		m_length = 0;
		setString(str.m_text);
	}

	~BaseString()
	{
		if(m_text)
		{
			delete[] m_text;
			m_text = nullptr;
		}
	}

	void Add(const char *str)
	{
		if(str && str[0])
		{
			for(int i = 0; str[i] && m_length < m_size; i++)
			{
				m_text[m_length++] = str[i];
			}
			m_text[m_size - 1] = '\0';
		}
	}
	void Add(char c)
	{
		if(m_length < m_size)
		{
			m_text[m_length++] = c;
		}
		m_text[m_size - 1] = '\0';
	}
	void Clear()
	{
		for(int i = 0; m_text[i]; i++)
		{
			m_text[i] = '\0';
		}
		m_length = 0;
	}
	bool Contains(char c)
	{
		for(int i = 0; m_text[i]; i++)
		{
			if(m_text[i] == c)
			{
				return true;
			}
		}
		return false;
	}
	bool Equal(const char *str)
	{
		return *this == str;
	}
	uint32 Length(void)
	{
		return m_length;
	}
	uint32 Size(void)
	{
		return m_size;
	}
	void RemoveLast(void)
	{
		if(m_length)
		{
			m_text[--m_length] = '\0';
		}
	}
	void CapitalizeWords(void)
	{
		bool found = false;
		for (int i = 0; m_text[i]; i++)
		{
			if (!found && CharOperations::isLetter(m_text[i]))
			{
				m_text[i] = CharOperations::getUpperCase(m_text[i]);
				found = true;
			}
			else if (CharOperations::isWordSeparator(m_text[i]))
			{
				found = false;
			}
		}
	}
	void Replace(char cTgt, char cNew)
	{
		for (int i = 0; m_text[i]; i++)
		{
			if (!(m_text[i] - cTgt))
			{
				m_text[i] = cNew;
			}
		}
	}

	int indexOfFirst(char C)
	{
		return CharOperations::indexOfFirst(m_text, C);
	}
	int indexOfLast(char C)
	{
		return CharOperations::indexOfLast(m_text, C);
	}

	char* indexedPointer(int Index)
	{
		return &m_text[Index];
	}

	void setLower(void)
	{
		for(int i = 0; m_text[i]; i++)
		{
			if(m_text[i] >= 'A' && m_text[i] <= 'Z')
			{
				m_text[i] += 32;
			}
		}
	}
	void setLower(int start, int stop)
	{
		for(int i = start; i < stop && m_text[i]; i++)
		{
			if(m_text[i] >= 'A' && m_text[i] <= 'Z')
			{
				m_text[i] += 32;
			}
		}
	}
	void setString(const char *str)
	{
		if(str && str[0])
		{
			m_length = 0;
			while(m_length < m_size)
			{
				m_text[m_length] = str[m_length];
				m_length++;
				if(!str[m_length])	//str[i] != '\0'
				{
					break;
				}
			}
			m_text[m_length] = '\0';
		}
	}
	void setString(BaseString *bs)
	{
		setString(bs->getString());
	}
	void setUpper(void)
	{
		for(int i = 0; m_text[i]; i++)
		{
			if(m_text[i] >= 'a' && m_text[i] <= 'z')
			{
				m_text[i] -= 32;
			}
		}
	}
	void setUpper(int start, int stop)
	{
		for(int i = start; i < stop && m_text[i]; i++)
		{
			if(m_text[i] >= 'a' && m_text[i] <= 'z')
			{
				m_text[i] -= 32;
			}
		}
	}
	char* getString(void)
	{
		return m_text;
	}

	//Operators
	char* operator=(char* str)
	{
		if (str)
		{
			setString(str);
			return m_text;
		}
		return nullptr;
	}
	char* operator=(const char* str)
	{
		if(str)
		{
			setString(str);
			return m_text;
		}
		return nullptr;
	}
	char* operator+=(const char* str)
	{
		if(str)
		{
			Add(str);
			return m_text;
		}
		return nullptr;
	}
	BaseString operator+(const char c)
	{
		BaseString temp(*this);
		temp.Add(c);
		return temp;
	}
	char* operator=(const char c)
	{
		m_text[0] = c;
		m_text[1] = '\0';
		m_length = 1;
		return m_text;
	}
	char* operator+=(const char c)
	{

		Add(c);
		return m_text;
	}
	bool operator==(const BaseString &s) const
	{
		int i = 0;
		for( ; m_text[i] && s.m_text[i]; i++)
		{
			if(s.m_text[i] != m_text[i])
			{
				return false;
			}
		}
		return m_text[i] == s.m_text[i];
	}
	bool operator!=(const BaseString &s) const
	{
		return !(*this == s);
	}
	bool operator==(const char *s) const
	{
		int i = 0;
		for( ; m_text[i] && s[i]; i++)
		{
			if(s[i] != m_text[i])
			{
				return false;
			}
		}
		return m_text[i] == s[i];	//If equal size, this will return true. If one is shorther than the other, return false
	}
	bool operator!=(const char *s) const
	{
		return !(*this == s);
	}
	char operator[](int Index) const
	{
		return m_text[Index];
	}
	char& operator[](int Index)
	{
		return m_text[Index];
	}
};

class String32 : public BaseString
{
public:
	String32() : BaseString(32) { };
	String32(const char* str) : BaseString(32, str) { };

	//Operators
	char* operator=(const char* str)
	{
		if(str)
		{
			setString(str);
			return this->m_text;
		}
		return nullptr;
	}
	char* operator+=(const char* str)
	{
		if(str)
		{
			Add(str);
			return m_text;
		}
		return nullptr;
	}
	char* operator=(const char c)
	{

		m_text[0] = c;
		m_text[1] = '\0';
		return m_text;
	}
	char* operator+=(const char c)
	{

		Add(c);
		return m_text;
	}
	bool operator==(const BaseString &s) const
	{

		for(int i = 0; this->m_text[i] && s[i]; i++)
		{
			if(s[i] != this->m_text[i])
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const BaseString &s) const
	{
		return !(*this == s);
	}
	bool operator==(const char *s) const
	{

		for(int i = 0; m_text[i] && s[i]; i++)
		{
			if(s[i] != m_text[i])
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const char *s) const
	{
		return !(*this == s);
	}
	char operator[](int Index) const
	{
		return m_text[Index];
	}
	char & operator[](int Index)
	{
		return m_text[Index];
	}
};

class String64 : public BaseString
{
public:
	String64() : BaseString(64) { };
	String64(const char* str) : BaseString(64, str) { };

	//Operators
	char* operator=(const char *str)
	{
		if(str)
		{
			setString(str);
			return this->m_text;
		}
		return nullptr;
	}
	char* operator+=(const char *str)
	{
		if(str)
		{
			Add(str);
			return m_text;
		}
		return nullptr;
	}
	char* operator=(const char c)
	{

		m_text[0] = c;
		m_text[1] = '\0';
		return m_text;
	}
	char* operator+=(const char c)
	{

		Add(c);
		return m_text;
	}
	bool operator==(const BaseString &s) const
	{

		for(int i = 0; this->m_text[i] && s[i]; i++)
		{
			if(s[i] != this->m_text[i])
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const BaseString &s) const
	{
		return !(*this == s);
	}
	bool operator==(const char *s) const
	{

		for(int i = 0; m_text[i] && s[i]; i++)
		{
			if(s[i] != m_text[i])
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const char *s) const
	{
		return !(*this == s);
	}
	char operator[](int Index) const
	{
		return m_text[Index];
	}
	char & operator[](int Index)
	{
		return m_text[Index];
	}
};

//Custom bool for specific flip-flop operations ( !! WIP !! )
//Variable available: bool val
class FlagFlipFlop
{
private:
	bool val;

public:
	FlagFlipFlop()
	{
		val = false;
	}
	FlagFlipFlop(bool Boolean)
	{
		val = Boolean;
	}

	bool getBool(void)
	{
		val = !val;
		return val;
	}
	void setBool(bool nBool)
	{
		val = nBool;
	}

	//Operators
	bool operator==(const bool b)
	{
		return (getBool() == b);
	}
	bool operator!=(const bool b)
	{
		return getBool() != b;
	}
};

//Simple Flag class, using bit manipulation techniques to hold up to 8 flags (bool) in a single uint8 variable
//Bit-Index Table (for unsigned char (uint8)):
//	[BIT]	0 0 | 0 0 | 0 0 | 0 0
// [INDEX]	7 6 | 5 4 | 3 2 | 1 0
class Flag8
{
private:
	uint8 m_FlagHolder;

public:
	Flag8(uint8 Flags) : m_FlagHolder(Flags) { };

	bool operator[](int Index) const
	{
		return BitOperations::IsBitSetSafe(m_FlagHolder, Index);
	}
	bool isFlagSet(uint8 Flag)
	{
		return BitOperations::IsBitSetSafe(m_FlagHolder, Flag);
	}
	uint8 getValue(void)
	{
		return m_FlagHolder;
	}
	void setValue(uint8 val)
	{
		m_FlagHolder = val;
	}
	void toggleFlag(uint8 Flag)
	{
		BitOperations::ToggleBitSafe(m_FlagHolder, Flag);
	}
	void setFlag(uint8 Flag)
	{
		BitOperations::SetBitSafe(m_FlagHolder, Flag);
	}
};

//Specific classes             	//	------------------------------------------------------------------------------------------------------

//Color Palette for color data (either separated as 4 uint8 or a single uint32)
//Holds all colors, but keeps Alpha to 255 (full opaque) and don't allow to change it. This class, is the base class for "RGBAColorPalette"
class RGBColorPalette
{
protected:
	union ColorComp
	{
		uint32 Col;		//'Full' color
		uint8 Comps[4];	//Color components: [0] - Red | [1] - Green | [2] - Blue | [3] - Alpha
	}Color;

	void resetPalette(void)
	{
		Color.Col = 0;
		Color.Comps[3] = 255;
	}

public:
	RGBColorPalette() { resetPalette(); };
	RGBColorPalette(const uint8 Red, const uint8 Green, const uint8 Blue)
	{
		Color.Comps[0] = Red;
		Color.Comps[1] = Green;
		Color.Comps[2] = Blue;
	}
	RGBColorPalette(const uint8* Colors)
	{
		Color.Comps[0] = Colors[0];
		Color.Comps[1] = Colors[1];
		Color.Comps[2] = Colors[2];
	}
	RGBColorPalette(const uint32 Colors)
	{
		setColorsFromUInt32(Colors);
	}

	uint8 getRed(void) const
	{
		return Color.Comps[0];
	}
	uint8 getGreen(void) const
	{
		return Color.Comps[1];
	}
	uint8 getBlue(void) const
	{
		return Color.Comps[2];
	}
	void setRed(const uint8 Red)
	{
		Color.Comps[0] = Red;
	}
	void setGreen(const uint8 Green)
	{
		Color.Comps[1] = Green;
	}
	void setBlue(const uint8 Blue)
	{
		Color.Comps[2] = Blue;
	}
	void setColorsFromPalette(RGBColorPalette &Palette)
	{
		Color.Col = Palette.getColorsAsUInt32();
	}
	bool compare(RGBColorPalette &Palette)
	{
		return Color.Col == Palette.getColorsAsUInt32();
	}
	virtual bool compare(const uint32 Colors)
	{
		RGBColorPalette Temp(Colors);
		return Color.Col == Temp.getColorsAsUInt32();
	}
	float getBrightness()
	{
		return brightness(*this);
	}
	virtual uint32 getColorsAsUInt32(void)
	{
		//uint32 Out = (getBlue() << 8) | (getGreen() << 16) | (getRed() << 24);
		return Color.Col;
	}
	virtual void setColorsFromUInt32(const uint32 Colors)
	{
		Color.Col = Colors;
		Color.Comps[3] = 255;
	}
	virtual void setColorsFromUint8(const uint8* Colors)
	{
		Color.Comps[0] = Colors[0];
		Color.Comps[1] = Colors[1];
		Color.Comps[2] = Colors[2];
	}

	bool operator==(RGBColorPalette &Pal)
	{
		return compare(Pal);
	}
	bool operator!=(RGBColorPalette &Pal)
	{
		return !compare(Pal);
	}

	//Computes how much close to white this color is, where 1 is 100% white and 0 is 0% white (black)
	static float brightness(const uint8 Red, const uint8 Green, const uint8 Blue)
	{
		return (float)(Red + Green + Blue) / 765;
	}
	static float brightness(const RGBColorPalette &Palette)
	{
		return (float)(Palette.getRed() + Palette.getGreen() + Palette.getBlue()) / 765;
	}
};

//Color Palette for color data (either separated as 4 uint8 or a single uint32)
//Holds all colors, with Alpha controls
class RGBAColorPalette : public RGBColorPalette
{
public:
	RGBAColorPalette() { };	//Automatically builds with 'RGBColorPalette' constructor
	RGBAColorPalette(uint8 Red, uint8 Green, uint8 Blue)
	{
		Color.Comps[0] = Red;
		Color.Comps[1] = Green;
		Color.Comps[2] = Blue;
		Color.Comps[3] = 255;
	}
	RGBAColorPalette(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha)
	{
		Color.Comps[0] = Red;
		Color.Comps[1] = Green;
		Color.Comps[2] = Blue;
		Color.Comps[3] = Alpha;
	}
	RGBAColorPalette(const uint8 *Colors)
	{
		Color.Comps[0] = Colors[0];
		Color.Comps[1] = Colors[1];
		Color.Comps[2] = Colors[2];
		Color.Comps[3] = Colors[3];
	}
	RGBAColorPalette(const uint32 Colors)
	{
		setColorsFromUInt32(Colors);
	}
	RGBAColorPalette(RGBColorPalette &Clr)
	{
		setColorsFromPalette(Clr);
	}

	uint8 getAlpha(void) const
	{
		return Color.Comps[3];
	}
	void setAlpha(const uint8 Alpha)
	{
		Color.Comps[3] = Alpha;
	}
	void setColorsFromPalette(RGBAColorPalette &Palette)
	{
		Color.Col = Palette.getColorsAsUInt32();
	}
	void setColorsFromPalette(RGBColorPalette &Palette)
	{
		Color.Col = Palette.getColorsAsUInt32();
	}
	bool compare(RGBAColorPalette &Palette)
	{
		return Color.Col == Palette.getColorsAsUInt32();
	}
	bool compare(RGBColorPalette &Palette)
	{
		return (Color.Comps[0] == Palette.getRed() && Color.Comps[1] == Palette.getGreen() && Color.Comps[2] == Palette.getBlue());
	}
	bool compare(uint32 Colors)
	{
		return Color.Col == Colors;
	}

	uint32 getColorsAsUInt32(void)
	{
		//uint32 Out = (getAlpha() << 0) | (getBlue() << 8) | (getGreen() << 16) | (getRed() << 24);
		return Color.Col;
	}
	void setColorsFromUInt32(const uint32 Colors)
	{
		Color.Col = Colors;
	}
	void setColorsFromUint8(const uint8* Colors)
	{
		Color.Comps[0] = Colors[0];
		Color.Comps[1] = Colors[1];
		Color.Comps[2] = Colors[2];
		Color.Comps[3] = Colors[3];
	}

	bool operator==(RGBAColorPalette &Pal)
	{
		return compare(Pal);
	}
	bool operator!=(RGBAColorPalette &Pal)
	{
		return !compare(Pal);
	}
	bool operator==(RGBColorPalette &Pal)
	{
		return compare(Pal);
	}
	bool operator!=(RGBColorPalette &Pal)
	{
		return !compare(Pal);
	}

	static float weight(const uint8 Red, const uint8 Green, const uint8 Blue, const uint8 Alpha)
	{
		return (float)(Red + Green + Blue + Alpha) / 1020;
	}
	static float weight(const RGBAColorPalette &Palette)
	{
		return (float) (Palette.getRed() + Palette.getGreen() + Palette.getBlue() + Palette.getAlpha()) / 1020;
	}
};

//Keeps track of a task progress. Basic version (pgtk)
//Variables available: bool DisplayProgress / bool is_finished / char CurrentWorkWr[31] / uint16 StepsTODO / uint16 CurrentStep
class ProgressTracker
{
public:
	virtual int getProgressTrackerCurrentStep(void){ return pgtkCurrentStep; };			//Return the current step the code is working now
	virtual int getProgressTrackerStepsTODO(void){ return pgtkStepsTODO; };				//Return the total of steps required to finish a work
	virtual char* getProgressTrackerCurrentWorkName(void){ return pgtkCurrentWorkWr; };	//Return the current action name

protected:
	bool pgtkDisplayProgress = false;							//Display progress?
	char pgtkCurrentWorkWr[31];									//Work name
	uint16 pgtkStepsTODO = 0;									//Total steps to finish work
	uint16 pgtkCurrentStep = 0;									//Current action (from 0 to last step)
	char* pgtkGlobalWorkReader;

	//Default Constructor
	ProgressTracker(void)
	{
		pgtkCurrentWorkWr[0] = 'N';
		pgtkCurrentWorkWr[1] = 'O';
		pgtkCurrentWorkWr[2] = 'N';
		pgtkCurrentWorkWr[3] = 'E';
		pgtkCurrentWorkWr[4] = '\0';
	}
	//Sets a Current Work Name to match another instance (WIP)
	//Needs to make the "CurrentWorkWr" points to another "CurrentWorkWr" instance based on it's pointer (Super Global)
	//void setTwin(char (&TargetTwin)[31])
	//{
	//	CurrentWorkWr = TargetTwin;
	//}
};

class TimedAction
{
protected:
	uint64 startTimer;		//Defines the start of the timer, in ms
	uint64 nextCycle;		//Defines when the next 'act' command should execute, in ms
	uint64 totalActs;		//Total 'acts' this function have performed. Increases by 1 each time it CAN ACT
	uint64 secondReset;
	uint32 actInterval;		//Defines after how many ms will a 'action' be executed
	uint32 APS;				//Acts Per Second. How many 'acts' have been executed in 1 second ('resolutionTime' dependent)
	uint32 resolutionTime;	//Defines the resolution this timer will use (or 'ticks per second'). This varies by the OS and function used to get time.
	uint32 cyclesElapsed;
	bool isActing;			//Can act if it's true

public:
	TimedAction(uint64 start, uint32 interval, uint32 resolution)
	{
		startTimer = start;
		actInterval = interval ? interval : 1;
		nextCycle = interval + start;
		resolutionTime = resolution ? resolution : 1000;
		secondReset = startTimer + resolutionTime;
		cyclesElapsed = APS = 0;
		totalActs = 0;
		isActing = true;
	}

	bool canAct(int timeCounter)
	{
		if (isActing)
		{
			if (timeCounter >= nextCycle)
			{
				cyclesElapsed++;
				nextCycle += actInterval;
				if (timeCounter >= secondReset)
				{
					totalActs += APS = cyclesElapsed;
					cyclesElapsed = 0;
					secondReset += resolutionTime;
				}
				return true;
			}
		}
		return false;
	}

	void setActing(bool canAct)
	{
		isActing = canAct;
	}
	void setInterval(int32 in)
	{
		uint32 inter = (in <= 0 ? 1 : in);
		if (actInterval - inter)
		{
			if (actInterval < inter)	//If the new interval is greater than the last one, increase the 'nextCycle' by the diff. of them
			{
				nextCycle += (inter - actInterval);
			}
			else						//If not, then decrease it by the inter.
			{ 
				nextCycle -= inter;
			}
			actInterval = inter;
		}
		//'actInterval' and 'inter' are equal. Ignore them
	}
	void setStart(int64 start)
	{
		startTimer = start <= 0 ? 0 : start;
		nextCycle = start + actInterval;
	}

	bool getActing(void)
	{
		return isActing;
	}
	uint32 getActsPerSecond(void)
	{
		return APS;
	}
	uint32 getResolutionTime(void)
	{
		return resolutionTime;
	}
	uint64 getTimerInterval(void)
	{
		return actInterval;
	}
	uint64 getTimerStart(void)
	{
		return startTimer;
	}
	uint64 getTotalActs(void)
	{
		return totalActs;
	}
	uint64 getTotalActsRelative(void)
	{
		return (nextCycle - startTimer) / actInterval;
	}
};

template <class ret, class par>
class TimedActionCallback : public TimedAction
{
private:
	ret(*callback)(par);	//Callback function. Will be called once the 'canAct()' returns true

public:
	TimedActionCallback(uint64 start, uint64 interval, uint32 resolution, ret(*func)(par)) : TimedAction(start, interval, resolution), callback(func) { };

	ret act(int timeCounter, par parameter)
	{
		if (canAct(timeCounter))
		{
			return callback(parameter);
		}
	}

	void setCallback(ret(*func)(par))
	{
		callback = func;
	}
};

template <class ret>
class TimedActionVoidCallback : public TimedAction
{
private:
	ret(*callback)(void);	//Callback function. Will be called once the 'canAct()' returns true

public:
	TimedActionVoidCallback(uint64 start, uint64 interval, uint32 resolution, ret(*func)(void)) : TimedAction(start, interval, resolution), callback(func) { };

	void act(int timeCounter)
	{
		if (canAct(timeCounter))
		{
			callback();
		}
	}

	void setCallback(ret(*func)(void))
	{
		callback = func;
	}
};
#endif
