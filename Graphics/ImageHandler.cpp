/*-------Image file handler (WIP)-------
*	Reads and process an image file
*	--Image reading and allocation handler
*		--Require "FreeImage.h" for PNG file loading
*
*
*	Moon Wiz Studios (c) - 02/06/2015
*	by: Ramon Darwich de Menezes
*
*	This work is a copyright of Moon Wiz Studios
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.8
*/

#include "ImageHandler.h"
#include <string>
//OS Especific Libraries
#ifdef _WIN32			//Windows specific BigEndian-LittleEndian converter (ntohl)
#include <WinSock2.h>
#elif _LINUX			//For Linux plataforms..
#include <netinet/in.h> 
#endif;
//Plataform Especific Libraries
#ifdef X86
#include <FreeImage\Dist\x32\FreeImage.h>
#endif;
#ifdef X64
#include <FreeImage\Dist\x64\FreeImage.h>
#endif;

#define BMP_HEADER_SIZE		54						//Header size for BMP Specific images
#define PNG_HEADER_SIZE		13						//Header size for PNG Specific images. This is the sum all [IHDR] elements
#define ALLOC_SAFE_MARGIN 	16						//How many bytes extra will be allocated to prevent heap corruption upon deleting memory

void BMP::setBMP(char *hedr)						//Normal BMP construction (prepares ImgData)
{
	if (!is_infoCatch)
	{
		for (int i = 0; i <= BMP_HEADER_SIZE; i++)
		{
			Header[i] = hedr[i];
		}
		BitsPerPixel = Header[28];
		Compression = *(int*)&(Header[0x1E]);
		DataPos = *(int*)&(Header[0x0A]);
		ImgSize = *(int*)&(Header[0x22]);
		ImgWidth = *(int*)&(Header[0x12]);
		ImgHeigth = *(int*)&(Header[0x16]);
		if (ImgSize == 0 && BitsPerPixel == 32)			//Guess the size, for bad formatted image. 32 bits image (HAVE ALPHA)
		{
			ImgSize = ImgWidth * ImgHeigth * 4;			//Width * Heigth * The 4 colors channels (R, G, B, A)
		}
		else if (ImgSize == 0 && BitsPerPixel == 24)	//Guess the size, for bad formatted image. All case scenario
		{
			ImgSize = ImgWidth * ImgHeigth * 3;			//Width * Heigth * The 3 colors channels (R, G, B)
		}
		if (DataPos == 0)
		{
			DataPos = 54;								//End of header and start of actual image data
		}
	}
	ImgData = new unsigned char[ImgSize + ALLOC_SAFE_MARGIN];
}

void BMP::catchBMPInfo(char *hedr)					//Custom BMP Construction (only get image info)
{
	for (int i = 0; i <= BMP_HEADER_SIZE; i++)
	{
		Header[i] = hedr[i];
	}
	BitsPerPixel = Header[28];
	Compression = *(int*)&(Header[0x1E]);
	DataPos = *(int*)&(Header[0x0A]);
	ImgSize = *(int*)&(Header[0x22]);
	ImgWidth = *(int*)&(Header[0x12]);
	ImgHeigth = *(int*)&(Header[0x16]);
	if (ImgSize == 0 && BitsPerPixel == 32)			//Guess the size, for bad formatted image. 32 bits image (HAVE ALPHA)
	{
		ImgSize = ImgWidth * ImgHeigth * 4;			//Width * Heigth * The 4 colors channels (R, G, B, A)
	}
	else if (ImgSize == 0 && BitsPerPixel == 24)	//Guess the size, for bad formatted image. All case cenario
	{
		ImgSize = ImgWidth * ImgHeigth * 3;			//Width * Heigth * The 3 colors channels (R, G, B)
	}
	if (DataPos == 0)
	{
		DataPos = 54;								//End of header and start of actual image data
	}
	is_infoCatch = true;
}

void BMP::unloadBMP(void)
{
	delete[] ImgData;
	ImgData = NULL;
}

void PNG::setPNG(char *hedr)
{
	IHDRStart = 0;
	for (int i = 0; i <= BMP_HEADER_SIZE; i++)		//Finds where the PNG header [IHDR] is, since BMP_HEADER_SIZE is the full length of the preload header
	{
		if (hedr[i] == 'I' && hedr[i + 1] == 'H' && hedr[i + 2] == 'D' && hedr[i + 3] == 'R')
		{
			IHDRStart = i + 4;						//We, then, set the IHDRStart to i + 4 (The next value AFTER the identifier)
			break;
		}
	}
	for (int i = IHDRStart; i <= (IHDRStart + PNG_HEADER_SIZE); i++)
	{
		Header[i - IHDRStart] = hedr[i];
	}
	BitsPerPixel = Header[8];
	ColorType = Header[9];
	Compression = Header[10];
	FilterMethod = Header[11];
	InterlaceMethod = Header[12];
}

//void PNG::convertPNGWH(void)
//{
//	ImgWidth = ntohl(ImgWidth);
//	ImgHeigth = ntohl(ImgHeigth);
//}

void PNG::setPNGBuffer(FILE *file)
{
	DataPos = 0;
	if (ColorType == 2)
	{
		ImgSize = ImgHeigth * ImgWidth * 3;
	}
	else if (ColorType == 6)
	{
		ImgSize = ImgHeigth * ImgWidth * 4;
	}
	ImgData = new unsigned char[ImgSize + ALLOC_SAFE_MARGIN];
	fseek(file, 0, SEEK_SET);						//Set's the stream back to start. Checking for [IDAT] signature
	fread(ImgData, 1, ImgSize, file);
	for (int i = 0; i <= ImgSize; i++)
	{
		if (ImgData[i] == 'I' && ImgData[i + 1] == 'D' && ImgData[i + 2] == 'A' && ImgData[i + 3] == 'T')
		{
			DataPos = i + 4;
			break;
		}
	}
	fseek(file, DataPos, SEEK_SET);
	fread(ImgData, 1, ImgSize, file);
}

void PNG::catchPNGInfo(char *hedr)
{
	IHDRStart = 0;
	for (int i = 0; i <= BMP_HEADER_SIZE; i++)		//Finds where the PNG header [IHDR] is, since BMP_HEADER_SIZE is the full length of the preload header
	{
		if (hedr[i] == 'I' || hedr[i] == 'H' || hedr[i] == 'D' || hedr[i] == 'R')
		{
			IHDRStart++;
		}
		if (IHDRStart == 4)							//If IHDR = 4, then we found the Header identifier [IHDR]
		{
			IHDRStart = i + 1;						//We, then, set the IHDRStart to i + 1 (The next value AFTER the identifier)
			break;
		}
	}
	for (int i = IHDRStart; i <= (IHDRStart + PNG_HEADER_SIZE); i++)
	{
		Header[i - IHDRStart] = hedr[i];
	}
	BitsPerPixel = Header[8];
	ColorType = Header[9];
	Compression = Header[10];
	FilterMethod = Header[11];
	InterlaceMethod = Header[12];
}

void PNG::unloadPNG(void)
{
	delete[] ImgData;
	ImgData = NULL;
}