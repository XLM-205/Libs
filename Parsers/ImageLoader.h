/*-------Image Loader-------
*	??
*	--??
*
*
*	Moon Wiz Studios (c) - 30/10/2020
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.0.0
*/

#ifndef _H_MWIMAGELOADER_
#define _H_MWIMAGELOADER_

#define DATA_TYPES_FOREACH

#include "../Control/CustomTypes-SA.h"
#include "../Control/DataTypes.h"

enum class ImageFormat
{
	FORMAT_UNSOPORTED = -1,
	FORMAT_PNG,
};

class BaseImage
{
private:
	uint32 m_width, m_height;
	uint8 m_colorBits;
	uint8 m_bitDepth;

	Pixel **m_pixels;

	ImageFormat validateFormat(char *header)
	{
		const int supFormats = 1, maxSigLen = 16;
		//Matrix of signatures [i][j] where:
		//	* i is the format, following the 'ImageFormat' enumerator
		//	* [i][0] defines the length of the signature
		char sigMatrix[supFormats][maxSigLen] = { { 8, 137, 80, 78, 71, 13, 10, 26, 10 }	//PNG Signature
												};
		for (int i = 0; i < supFormats; i++)
		{
			if (CharOperations::isEqualSensitive(header, &sigMatrix[i][1], (int)sigMatrix[i][0]))
			{
				return (ImageFormat)i;
			}
		}
		return ImageFormat::FORMAT_UNSOPORTED;
	}

public:
	BaseImage() : m_width(0), m_height(0), m_colorBits(0), m_bitDepth(0), m_pixels(nullptr) { }
	~BaseImage()
	{
		if (m_pixels)
		{
			for (uint32 i = 0; i < m_width; i++)
			{
				delete[] m_pixels[i];
			}
			delete[] m_pixels;
		}
			
	}

};

class ImagePNG : public BaseImage
{
private:
	//Chunk Specifications: http://www.libpng.org/pub/png/spec/1.2/PNG-Chunks.html
	//	IHDR (Image Header) - Critical, must be the FIRST CHUNK. Contains (bytes)
	//		* Width(4), Height(4)	- Cannot be 0. Max is 2^31 (CST_INT32_MAX)
	//		* Bit Depht(1)			- Define how many bits per sample or palette. Can be 1, 2, 4, 8 or 16. Not all values are allowed for all color types
	//		* Color Type(1)			- Defines how to interpret the data. [1] - Palette | [2] - Color used | [4] - Alpha Used. 0, 2, 3, 4 and 6 are valid
	//		* Compression Method(1)	- Currently, only [0] - Deflate/Inflate. Must report if invalid
	//		* Filter Method(1)		- Currently, only [0] - Adaptative. Must report if invalid
	//		* Interlace Method(1)	- [0] - No Interlace | [1] - Adam7
	//		[Clr Tp]	|	[Bit Dpth]	 |
	//			0		| 1, 2, 4, 8, 16 |	Each pixel is a grayscale sample
	//			2		|	  8, 16		 |	Each pixel is a RGB triple
	//			3		|   1, 2, 4, 8	 |	Each pixel is a palette index. a PLTE chunk MUST appear
	//			4		|	  8, 16		 |	Each pixel is a grayscale sample followed by an Alpha
	//			6		|	  8, 16		 |	Each pixel is a RGB triple followed by an Alpha
	//	PLTE (Palette) - Contains palette entries from 1 to 256, each a 3 byte series of R, G, B (Palettes doesn't have Alpha). Entries amount determined from chunk length, which MUST BE divisible by 3 (Report if not)
	//		* There must be ONLY ONE PLTE chunk and always 1 byte per sample regardless of bit depth
	//		* Must appear for color types 3, with 2 and 6 being optional. Must NOT appear for 0 and 4, but if does, it must precede the first IDAT
	//		* For color type 3: Firsty entry in PLTE is referenced by pixel value 0, then 1 and so on. Must not exceed the range defined by the bit depth (Entries = 2^(bit depth))
	//		* For color 2 and 6 (truecolor with/out alpha) it's optional. If present, it's a suggestion from 1 to 256 to quantize if the viewer cannot display truecolor
	//	IDAT (Image Data) - Actual image data (Reading)
	//		* Concatenate all IDATs and Deflate
	//		* Filter the image with IDHR's filter method
	//		* Apply image scanlines. Pixels appear from left-to-right, top-to-bottom
	//		* When they start appearing, there CANNOT BE another type of chunk aside IDAT or IEND
	struct PNGChunk
	{
		uint32 m_length;	//Number of bytes of the Chunk's data field. Does NOT include the chunk itself nor CRC. 0 is a valid value. Max is 2^31 (CST_INT32_MAX)
		uint32 m_chunkType;	//Type Descriptor: bit 5 of each byte convey chunk properties (if the letter is uppercase or lowercase)
		uint32 m_CRC;		//Cyclic Redundancy Check. Computed on the preceding bytes of the chunk, with type code and data fields but WITHOUT the length
		uint8 *m_chunk;		//The chunk of data itself

		//Chunk Type values (each bit 5 of...):
		//	Byte 1 [ 5] "Ancillary"	: [0] - Critical | [1] - Ancillary	* Ancillary chunks are NOT required to display the image. If a critical but unknow chunk is found, it MUST be reported
		//	Byte 2 [11] "Private"	: [0] - Public	 | [1] - Private	* If public, this chunk is part of the PNG specification. Not necessary to test this property
		//	Byte 3 [21] "Reserved"	: [0] - Must be 0 (as of 2020)		* ...but don't complain if it's 1, too (possibly can be skipped during reading)
		//	Byte 4 [29] "Safe-to-Copy"	: [0] - Unsafe | [1] - Safe		* Only used in editors. If 0, the chunk depends on the image data, then, if there is ANY change on CRITICAL chunks ONLY, unsafe chunks should NOT be copied

		PNGChunk() : m_length(0), m_chunkType(0), m_CRC(0), m_chunk(nullptr) { }
		PNGChunk(int len, int type, int CRC) : m_length(len < 0 ? 0 : len), m_chunkType(type), m_CRC(CRC), m_chunk(m_length == 0 ? nullptr : new uint8[m_length]) { }

		~PNGChunk()
		{
			if (m_chunk)
			{
				delete[] m_chunk;
			}
		}
	};

public:
	ImagePNG() { }
	~ImagePNG()
	{

	}


};


#endif
