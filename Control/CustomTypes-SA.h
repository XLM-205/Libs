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
*	Version 1.8.93
*/

#ifndef _H_CSTTYPE_
#define _H_CSTTYPE_

//Custom variables types and classes for easy use

//Math and Constants
#define CST_PI		3.141592653589793				//PI			 (15 decimal places)
#define CST_2PI		6.283185307179586				//2 * PI		 (15 decimal places)
#define CST_PI_180	0.017453292516666				//PI / 180		 (15 decimal places)
#define CST_180_PI	57.29577951308232				//180 / PI		 (15 decimal places)
#define CST_EULER	2.718281828459045				//Euler Constant (15 decimal places)
#define CST_KB		1024							//Byte / CST_KB = KB
#define CST_MB		1048576							//Byte / CST_MB = MB
#define CST_GB		1073741824						//Byte / CST_GB = GB
#define CST_TB		1099511627776					//Byte / CST_TB = TB
//Keyboard Key codes
#define CST_KEY_BACKSPACE		 8					//Keyboard ASCII code for ' Backspace '
#define CST_KEY_TAB				 9					//Keyboard ASCII code for ' Tab '
#define CST_KEY_ENTER			 13					//Keyboard ASCII code for ' Enter '
#define CST_KEY_ESC				 27					//Keyboard ASCII code for ' Escape '
#define CST_KEY_SPACE			 32					//Keyboard ASCII code for ' Space '
#define CST_KEY_DOUBLE_QUOTE	 34					//Keyboard ASCII code for ' " '
#define CST_KEY_SINGLE_QUOTE	 39					//Keyboard ASCII code for ' ' '
#define CST_KEY_DELETE			 127				//Keyboard ASCII code for ' Delete '
//Variables Limits
#define CST_INT8_MAX			 127				//Upper Limit for a 1 Byte Signed Integer
#define CST_UINT8_MAX			 255				//Upper Limit for a 1 Byte (UN) Signed Integer
#define CST_INT16_MAX		    32767				//Upper Limit for a 2 Bytes Signed Integer
#define CST_UINT16_MAX			65535				//Upper Limit for a 2 Bytes (UN) Signed Integer
#define CST_INT32_MAX		  2147483647			//Upper Limit for a 4 Bytes Signed Integer
#define CST_UINT32_MAX		  4294967295			//Upper Limit for a 4 Bytes (UN) Signed Integer
#define CST_INT64_MAX     9223372036854775807		//Upper Limit for a 8 Bytes Signed Integer
#define CST_UINT64_MAX	 18446744073709551615		//Upper Limit for a 8 Bytes Signed Integer
#define CST_INT8_MIN	   -CST_INT8_MAX  - 1		//Down Limit for a 1 Byte Signed Integer (Inverse of MAX - 1, to prevent warnings)
#define CST_INT16_MIN	   -CST_INT16_MAX - 1		//Down Limit for a 2 Bytes Signed Integer (Inverse of MAX - 1, to prevent warnings)
#define CST_INT32_MIN	   -CST_INT32_MAX - 1		//Down Limit for a 4 Bytes Signed Integer (Inverse of MAX - 1, to prevent warnings)
#define CST_INT64_MIN	   -CST_INT64_MAX - 1		//Down Limit for a 8 Bytes Signed Integer (Inverse of MAX - 1, to prevent warnings)
//Variables Digits Count
#define CST_DIGITS_INT8			 4					//Digits WITH minus sign consideration
#define CST_DIGITS_INT16		 6					//Digits WITH minus sign consideration
#define CST_DIGITS_INT32		11					//Digits WITH minus sign consideration
#define CST_DIGITS_INT64		20					//Digits WITH minus sign consideration
#define CST_DIGITS_UINT8		 3					//Digits WITHOUT minus sign consideration
#define CST_DIGITS_UINT16		 5					//Digits WITHOUT minus sign consideration
#define CST_DIGITS_UINT32		10					//Digits WITHOUT minus sign consideration
#define CST_DIGITS_UINT64		19					//Digits WITHOUT minus sign consideration

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

enum StateType
{
	EXCP = -3,	//Exception
	ERR,		//Error
	WARN,		//Warning
	NONE,		//None State Defined (Default)
	SUCCESS		//Success
};

enum ExtractByte
{
	FIRST,
	SECOND,
	THIRD,
	FORTH
};

enum ArrayCompare
{
	LESS,			// <
	LEQ,			// <=
	GREATER,		// >
	GEQ,			// >=
	EQUAL,			// ==
	DIFF			// !=
};

enum Endian
{
	BIG_ENDIAN,		//Most significant bit (generaly the 'sign' bit) stays at the front
	LITTLE_ENDIAN	//Most significant bit (generaly the 'sign' bit) stays at the back
};

namespace BitOperations
{
	inline int ExtractFromUInt32(uint32 src, ExtractByte Byte, Endian Endianess)
	{
		if (Endianess == LITTLE_ENDIAN)
		{
			switch (Byte)
			{
			case FIRST:
				return ((src & 0xFF000000) >> 24);
			case SECOND:
				return ((src & 0x00FF0000) >> 16);
			case THIRD:
				return ((src & 0x0000FF00) >> 8);
			case FORTH:
				return ((src & 0x000000FF) >> 0);
			}
		}
		switch (Byte)
		{
		case FIRST:
			return ((src & 0xFF000000) >> 0);
		case SECOND:
			return ((src & 0x00FF0000) >> 8);
		case THIRD:
			return ((src & 0x0000FF00) >> 16);
		case FORTH:
			return ((src & 0x000000FF) >> 24);
		}
		return 0;	//Invalid parameters
	}
	inline int ExtractFromInt32(int32 src, ExtractByte Byte, Endian Endianess)
	{
		if (Endianess == LITTLE_ENDIAN)
		{
			switch (Byte)
			{
			case FIRST:
				return ((src & 0xFF000000) >> 24);
			case SECOND:
				return ((src & 0x00FF0000) >> 16);
			case THIRD:
				return ((src & 0x0000FF00) >> 8);
			case FORTH:
				return ((src & 0x000000FF) >> 0);
			}
		}
		switch (Byte)
		{
		case FIRST:
			return ((src & 0xFF000000) >> 0);
		case SECOND:
			return ((src & 0x00FF0000) >> 8);
		case THIRD:
			return ((src & 0x0000FF00) >> 16);
		case FORTH:
			return ((src & 0x000000FF) >> 24);
		}
		return 0;
	}
	inline uint8* ExtractFromUInt32(uint32 src, Endian Endianess)
	{
		uint8 *Out = new uint8[4];
		if (Endianess == LITTLE_ENDIAN)
		{
			Out[0] = (uint8)((src & 0xFF000000) >> 24);
			Out[1] = (uint8)((src & 0x00FF0000) >> 16);
			Out[2] = (uint8)((src & 0x0000FF00) >> 8);
			Out[3] = (uint8)(src & 0x000000FF);	// >> 0
			return Out;
		}
		Out[3] = (uint8)((src & 0xFF000000) >> 24);
		Out[2] = (uint8)((src & 0x00FF0000) >> 16);
		Out[1] = (uint8)((src & 0x0000FF00) >> 8);
		Out[0] = (uint8)(src & 0x000000FF);		// >> 0
		return Out;
	}
	inline 	int8* ExtractFromInt32(int32 src, Endian Endianess)
	{
		int8 *Out = new int8[4];
		if (Endianess == LITTLE_ENDIAN)
		{
			Out[0] = (int8)((src & 0xFF000000) >> 24);
			Out[1] = (int8)((src & 0x00FF0000) >> 16);
			Out[2] = (int8)((src & 0x0000FF00) >> 8);
			Out[3] = (int8)(src & 0x000000FF);
			return Out;
		}
		Out[3] = (int8)((src & 0xFF000000) >> 24);
		Out[2] = (int8)((src & 0x00FF0000) >> 16);
		Out[1] = (int8)((src & 0x0000FF00) >> 8);
		Out[0] = (int8)(src & 0x000000FF);
		return Out;
	}
	inline uint32 ComputeUInt32(uint8 *src, Endian Endianess)
	{	// !! (WIP) !! Verify if this is the correct order for BIG ENDIAN
		uint32 Out = 0;
		if (Endianess == BIG_ENDIAN)
		{
			Out = (src[0] << 0) | (src[1] << 8) | (src[2] << 16) | (src[3] << 24);
			return Out;
		}
		Out = (src[3] << 0) | (src[2] << 8) | (src[1] << 16) | (src[0] << 24);
		return Out;
	}
	inline int32 ComputeInt32(int8 *src, Endian Endianess)
	{
		int32 Out = 0;
		if (Endianess == BIG_ENDIAN)
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
	inline bool IsBitSet(T Ref, int Bit)
	{
		return Ref & (1 << Bit);
	}
	inline bool IsBitSet(int64 Ref, int Bit)
	{
		return Ref & (1LL << Bit);
	}
	inline bool IsBitSet(uint64 Ref, int Bit)
	{
		return Ref & (1LLU << Bit);
	}
	template <typename T>
	inline bool IsBitSetSafe(T Ref, int Bit)
	{
		if (Bit < (sizeof(T) * 8))
		{
			return IsBitSet(Ref, Bit);
		}
		return false;	//The bit range is too big for testing, returning false...
	}

#ifdef _INC_STDIO	//If <stdio.h> is included, allow print function
	template <typename T>
	void PrintBinary(T src, Endian Endianess)
	{
		uint16 Size = sizeof(T);
		uint16 Bits = Size * 8;
		uint16 Ops = (Size << 1) - 1;
		if (Endianess == BIG_ENDIAN)
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
	inline double getDecimals(double);

	template <typename T>
	inline T Abs(T &X)
	{
		return X > 0 ? X : -X;
	}
	inline double Abs(double X)	//If in the binary64 format, the sign bit is the last one (63)
	{
		uint64 i = reinterpret_cast<const uint64&>(X);
		i &= 0x7FFFFFFFFFFFFFFFULL;
		return reinterpret_cast<const double&>(i);
	}
	inline float Abs(float X)	//If in the binary32 format, the sign bit is the last one (31)
	{
		uint32 i = reinterpret_cast<const uint32&>(X);
		i &= 0x7FFFFFFFULL;
		return reinterpret_cast<const float&>(i);
	}
	inline uint64 Abs(int64 X)
	{
		const int mask = X >> 63;
		return (X + mask) ^ mask;
	}
	inline uint32 Abs(int32 X)
	{
		const int mask = X >> 31;
		return (X + mask) ^ mask;
	}
	inline uint16 Abs(int16 X)
	{
		const int mask = X >> 15;
		return (uint16)((X + mask) ^ mask);
	}
	inline uint8 Abs(int8 X)
	{
		const int mask = X >> 7;
		return (uint8)((X + mask) ^ mask);
	}

	inline int Floor(double N)
	{
		return (int)N;
	}
	inline int Celling(double N)
	{
		return (int)(N + 0.9999999);
	}
	inline int Mod(double Num, double Deno)
	{
		return (int)(Num - (Deno * (int)(Num / Deno)));
	}
	inline double ModClamped(double Num, double Max)
	{
		return Num - (Max * (int)(Num / Max));
	}

	//Return the amount of decimals places A have in common to B, up to the first miss or -1 if both have > 16 decimal places in common
	inline int preciseUpTo(double A, double B)
	{
		int prec = 0;
		while ((uint64)A == (uint64)B)
		{
			A *= 10;
			B *= 10;
			prec++;
			if (prec > 16)
			{
				return -1;
			}
		}
		return prec;
	}
	//Return true if 'A' == 'B' within 'precision' decimal places, up to 16 places. Defaults to 1.
	inline bool isEqualRange(double A, double B, int precision)
	{
		switch (precision)
		{
		default:
		case 1:
			return Utils::Abs(A-B) < 0.1;
		case 2:
			return Utils::Abs(A-B) < 0.01;
		case 3:
			return Utils::Abs(A-B) < 0.001;
		case 4:
			return Utils::Abs(A-B) < 0.0001;
		case 5:
			return Utils::Abs(A-B) < 0.00001;
		case 6:
			return Utils::Abs(A-B) < 0.000001;
		case 7:
			return Utils::Abs(A-B) < 0.0000001;
		case 8:
			return Utils::Abs(A-B) < 0.00000001;
		case 9:
			return Utils::Abs(A-B) < 0.000000001;
		case 10:
			return Utils::Abs(A-B) < 0.0000000001;
		case 11:
			return Utils::Abs(A-B) < 0.00000000001;
		case 12:
			return Utils::Abs(A-B) < 0.000000000001;
		case 13:
			return Utils::Abs(A-B) < 0.0000000000001;
		case 14:
			return Utils::Abs(A-B) < 0.00000000000001;
		case 15:
			return Utils::Abs(A-B) < 0.000000000000001;
		case 16:
			return Utils::Abs(A-B) < 0.0000000000000001;
		}
	}

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

	inline double getDecimals(double N)
	{
		return (N - (int)N);
	}
	inline bool haveDecimal(double N)
	{
		return getDecimals(N) == 0.0;
	}
	inline bool haveDifSignals(double A, double B)
	{
		return (A * B) < 0;
	}

	inline int countDigits(int N)
	{
		if (N)	//Testing against 0
		{
			return 1 + countDigits((int)(N * 0.1));
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
			return n % 10 + sumDigits((int)(n * 0.1));
		}
		return 0;
	}
	inline int getDigit(int input, int n)
	{
		if (input > 9 && n > 1)
		{
			return getDigit((int)(input * 0.1), n - 1);
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
	//Clamped to the degree range of [0..360]
	inline double degToRadClamped(double D)
	{
		return Utils::Mod(D, 360) * CST_PI_180;
	}
	//Clamped to the radian range of [0..6.28]
	inline double radToDegClamped(double R)
	{
		return Utils::Mod(R, CST_2PI) * CST_180_PI;
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
	inline void matrixFill(T **matrix, int rows, int columns, T st, T inc)
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
	inline void arrayFill(T *arr, int sz, T st, T inc)
	{
		T val = st;
		for (int i = 0; i < sz; i++)
		{
			arr[i] = val;
			val += inc;
		}
	}
	template <class T>
	inline bool arrayCompare(T *src, T *dst, int szSrc, int szDst, ArrayCompare method)
	{
		if (szSrc == szDst)
		{
			switch (method)
			{
			case ArrayCompare::LESS:
				for (int i = 0; i < szSrc; i++)
				{
					if (src[i] >= dst[i])
					{
						return false;
					}
				}
				return true;
			case ArrayCompare::LEQ:
				for (int i = 0; i < szSrc; i++)
				{
					if (src[i] > dst[i])
					{
						return false;
					}
				}
				return true;
			case ArrayCompare::GREATER:
				for (int i = 0; i < szSrc; i++)
				{
					if (src[i] <= dst[i])
					{
						return false;
					}
				}
				return true;
			case ArrayCompare::GEQ:
				for (int i = 0; i < szSrc; i++)
				{
					if (src[i] < dst[i])
					{
						return false;
					}
				}
				return true;
			case ArrayCompare::EQUAL:
				for (int i = 0; i < szSrc; i++)
				{
					if (src[i] != dst[i])
					{
						return false;
					}
				}
				return true;
			case ArrayCompare::DIFF:
				for (int i = 0; i < szSrc; i++)
				{
					if (src[i] == dst[i])
					{
						return false;
					}
				}
				return true;
			}
		}
		return false;
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
		if (fgetc(file) != EOF)
		{
			fseek(file, -1L, SEEK_CUR);
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
	inline bool isEqualSensitive(char *S1, char *S2, int length)
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
	inline bool isEqualRangeSensitive(char *S1, char *S2, int from, int to)
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
	inline bool isEqual(char *S1, char *S2, int length)
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
	inline bool isEqualRange(char *S1, char *S2, int from, int to)
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

	inline char* numberToString(int N, int Digits)
	{
		int i = 0, DigitRegion = 1;
		char *Out = new char[CST_DIGITS_INT32 + 1]();
		if (Digits == 0)	//No digits?
		{
			return nullptr;
		}
		for (int j = 0; j < Digits - 1; j++)
		{
			DigitRegion *= 10;
		}
		if (N < 0)			//If negative, insert the minus sign
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
			DigitRegion = (int)(DigitRegion * 0.1);
		}
		return Out;
	}
	inline char* numberToString(int N)
	{
		return numberToString(N, Utils::countDigits(N));
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
	inline bool memIsAnySet(void* Tgt, T Val, int Size)	//Returns true if ANY memory space matches 'Val'
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
	inline bool memIsSet(void* Tgt, T Val, int index)	//Returns true if the place at 'index' matches 'Val'
	{
		return ((T*)Tgt)[index] == Val;
	}
	template <typename T>
	inline bool memIsAllSet(void* Tgt, T Val, int Size)	//Returns true if ALL memory space matches 'Val', obeying 'Size'
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

//Self deleting array (to prevent memory leakage)
template <typename T>
class ArrayAuto
{
protected:
	const int m_size;
	T *m_data;

public:

	//Intializes 'n' blank slots
	ArrayAuto(int n) : m_size(n > 0 ? n : 1)
	{
		m_data = new T[m_size];
	}
	//Intializes 'n' slots, all with value 'val'
	ArrayAuto(int n, T val) : m_size(n > 0 ? n : 1)
	{
		m_data = new T[m_size];
		Set(val);
	}
	//Hosts a previous allocated array, deleting it afterwards
	ArrayAuto(int n, T *arr) : m_size(n)
	{
		m_data = arr;
	}
	ArrayAuto(int n, const T *arr) : m_size(n)
	{
		m_data = new T[m_size];
		Set(arr);
	}
	ArrayAuto(int n, T st, T inc) : m_size(n > 0 ? n : 1)
	{
		m_data = new T[m_size];
		Fill(st, inc);
	}
	ArrayAuto(const ArrayAuto<T> &copy) : m_size(copy.Length())
	{
		m_data = new T[m_size];
		for (int i = 0; i < m_size; i++)
		{
			m_data[i] = copy[i];
		}
	}

	~ArrayAuto()
	{
		if (m_data)
		{
			delete[] m_data;
			m_data = nullptr;
		}
	}

	void Fill(T start, T inc)
	{
		Utils::arrayFill(m_data, m_size, start, inc);
	}

	void Set(T *arr)
	{
		for (int i = 0; i < m_size; i++)
		{
			m_data[i] = arr[i];
		}
	}
	void Set(T val)
	{
		for (int i = 0; i < m_size; i++)
		{
			m_data[i] = val;
		}
	}
	virtual void Set(T val, int i)
	{
		m_data[i] = val;
	}
	virtual void Set(T *arr, int st)
	{
		for (int i = st; i < m_size; i++)
		{
			m_data[i] = arr[i];
		}
	}

	int Length(void)
	{
		return m_size;
	}

	bool sizeMatches(const ArrayAuto<T> &a)
	{
		return m_size == a.Length();
	}

	virtual T At(int index)
	{
		return m_data[index];
	}
	T* Whole(void)
	{
		return m_data;
	}

	//Operators -----------------------------------------------------
	bool operator<(const T k) const
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_data[i] >= k)
			{
				return false;
			}
		}
		return true;
	}
	bool operator<=(const T k) const
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_data[i] > k)
			{
				return false;
			}
		}
		return true;
	}
	bool operator>(const T k) const	
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_data[i] <= k)
			{
				return false;
			}
		}
		return true;
	}
	bool operator>=(const T k) const
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_data[i] < k)
			{
				return false;
			}
		}
		return true;
	}

	bool operator<(ArrayAuto<T> &a) const
	{
		return Utils::arrayCompare(m_data, a.Whole(), m_size, a.Length(), ArrayCompare::LESS);
	}
	bool operator<=(ArrayAuto<T> &a) const
	{
		return Utils::arrayCompare(m_data, a.Whole(), m_size, a.Length(), ArrayCompare::LEQ);
	}
	bool operator>(ArrayAuto<T> &a) const
	{
		return Utils::arrayCompare(m_data, a.Whole(), m_size, a.Length(), ArrayCompare::GREATER);
	}
	bool operator>=(ArrayAuto<T> &a) const
	{
		return Utils::arrayCompare(m_data, a.Whole(), m_size, a.Length(), ArrayCompare::GEQ);
	}
	bool operator==(const T k) const
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_data[i] != k)
			{
				return false;
			}
		}
		return true;
	}
	bool operator==(ArrayAuto<T> &a) const
	{
		return Utils::arrayCompare(m_data, a.Whole(), m_size, a.Length(), ArrayCompare::EQUAL);
	}
	bool operator!=(const T k) const
	{
		return !(*this == k);
	}
	bool operator!=(ArrayAuto<T> &a) const
	{
		return !(*this == a);
	}

	virtual T& operator[](const int i)
	{
		return m_data[i];
	}

#ifdef _INC_STDIO	//If <stdio.h> is included, allow print function
	void print(const char *format, const char *onEnd)
	{
		for (int i = 0; i < m_size; i++)
		{
			printf(format, m_data[i]);
		}
		printf(onEnd);
	}
	void print(const char *format)
	{
		for (int i = 0; i < m_size; i++)
		{
			printf(format, m_data[i]);
		}
		printf("\n");
	}
#endif
};

template <typename T>
class ArrayCircular : public ArrayAuto<T>
{
public:
	ArrayCircular(int n) : ArrayAuto<T>(n) { }
	ArrayCircular(int n, T val) : ArrayAuto<T>(n, val) { }
	ArrayCircular(int n, T *arr) : ArrayAuto<T>(n, arr) { }
	ArrayCircular(int n, T st, T inc) : ArrayAuto<T>(n, st, inc) { }
	
	ArrayCircular(const ArrayCircular<T> &copy)
	{
		this->m_size = copy.Length();
		this->m_data = new T[this->m_size];
		for (int i = 0; i < this->m_size; i++)
		{
			this->m_data[i] = copy[i];
		}
	}

	~ArrayCircular()
	{
		if (this->m_data)
		{
			delete[] this->m_data;
			this->m_data = nullptr;
		}
	}

	void Set(T val, int i)
	{
		this->m_data[At(i)] = val;
	}
	void Set(T *arr, int st)
	{
		for (int i = st; i < this->m_size; i++)
		{
			this->m_data[At(i)] = arr[i];
		}
	}

	T At(int index)
	{
		return this->m_data[index % this->m_size];
	}
	//Operators -----------------------------------------------------
	bool operator<(ArrayCircular<T> &a) const
	{
		return Utils::arrayCompare(this->m_data, a.Whole(), this->m_size, a.Length(), ArrayCompare::LESS);
	}
	bool operator<=(ArrayCircular<T> &a) const
	{
		return Utils::arrayCompare(this->m_data, a.Whole(), this->m_size, a.Length(), ArrayCompare::LEQ);
	}
	bool operator>(ArrayCircular<T> &a) const
	{
		return Utils::arrayCompare(this->m_data, a.Whole(), this->m_size, a.Length(), ArrayCompare::GREATER);
	}
	bool operator>=(ArrayCircular<T> &a) const
	{
		return Utils::arrayCompare(this->m_data, a.Whole(), this->m_size, a.Length(), ArrayCompare::GEQ);
	}
	bool operator==(ArrayCircular<T> &a) const
	{
		return Utils::arrayCompare(this->m_data, a.Whole(), this->m_size, a.Length(), ArrayCompare::EQUAL);
	}
	bool operator!=(ArrayCircular &a) const
	{
		return !(*this == a);
	}

	T& operator[](const int i)
	{
		return this->m_data[i % this->m_size];
	}
};

class BaseString
{
protected:
	uint32 m_length;
	uint32 m_size;
	char *m_text;

public:
	BaseString(const char *str) : m_length(0), m_size(0)
	{
		while(str[m_size])
		{
			m_size++;
		}
		m_text = new char[++m_size];
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
	void ClearFull()
	{
		for (uint32 i = 0; i < m_size; i++)
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
	uint32 Count(char c)
	{
		uint32 inc = 0;
		for (int i = 0; m_text[i]; i++)
		{
			if (m_text[i] == c)
			{
				inc++;
			}
		}
		return inc;
	}

	bool Empty(void)
	{
		//If the count of white spaces + 1 (for the '\0') is the length, then we have nothing here
		return Count(' ') + 1 == m_length;
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
	/*char& operator[](int Index)
	{
		return m_text[Index];
	}*/

#ifdef _INC_STDIO
	void Print(void)
	{
		printf("%s\n", m_text);
	}
	void Print(const char *format)
	{
		printf(format, m_text);
	}
	void Scan(const char *format)
	{
		setbuf(stdin, 0);
		scanf(format, m_text);
		m_length = 0;
		while (m_text[++m_length]);
		m_text[m_size - 1] = '\0';	//Failsafe
	}
#endif
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
	virtual bool compare(RGBColorPalette &Palette)
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
	bool compare(const uint32 Colors)
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
		return (float)(Palette.getRed() + Palette.getGreen() + Palette.getBlue() + Palette.getAlpha()) / 1020;
	}
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
	TimedActionCallback(uint64 start, uint32 interval, uint32 resolution, ret(*func)(par)) : TimedAction(start, interval, resolution), callback(func) { };

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
	TimedActionVoidCallback(uint64 start, uint32 interval, uint32 resolution, ret(*func)(void)) : TimedAction(start, interval, resolution), callback(func) { };

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

class ErrorDescription
{
protected:
	const static int m_noError = 0x6F6B6179;	//No Error (Cleared or Default)
	int m_code;									//Error Code
	String64 m_desc;							//Error Description

public:
	ErrorDescription(const int code, const char *desc)
	{
		SetError(code, desc);
	}
	ErrorDescription()
	{
		ClearError();
	}

	//Returns true if no error was defined (or was cleared)
	bool haveNoError(void)
	{
		return m_code == m_noError;
	}

	int Code(void)
	{
		return m_code;
	}
	const char* Description(void)
	{
		return m_desc.getString();
	}

	void SetError(int code, const char *desc)
	{
		m_code = code;
		m_desc.setString(desc);
	}

	void ClearError(void)
	{
		m_code = m_noError;
		m_desc.setString("No Error");
	}

#ifdef _INC_STDIO
	void print(void)
	{
		printf("[%-6d] - '%s'\n", m_code, m_desc.getString());
	}
#endif
};
#endif
