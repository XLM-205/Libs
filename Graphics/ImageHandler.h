/*-------Image file handler (WIP)-------
*	Reads and process an image file
*	--Image reading and allocation handler
*		--Require "CustomTypes.h" for custom variables definitions
*		--Require "ImageLoader.h" core engine on the program implementation
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

#ifndef _H_MWIMGHANDLER_
#define _H_MWIMGHANDLER_
//Struct definitions for each format of image

#include <stdio.h>
#include "..\Control\CustomTypes-SA.h"

//If the image doens't have the image's size saved, guess it, using the formula: ImgWidth * ImgHeigth * 3 for RGB or ImgWidth * ImgHeigth * 4 for RGBA

//Reference: BitsPerPixel -> Color Depth/Bit Depth | RGB/RGBA -> TrueColor/TrueColor + Alpha
//PNG's Critical Data Chunks: [SIG] [IHDR] [PLTE] [IDAT] [IEND] for [Signature] [Header] [Palette] [Actual Image Data] [End of image marker]
//OBS.: The [PLTE] Chunk, only appears if the image is of color type 3 [Indexed Color]
//PNG's Official reference: http://www.w3.org/TR/PNG/
//PNG's Allowed Bit Depths:

//|	COLOR TYPE	|	ALLOWED BIT DEPTHS	|
//|-------------------------------------|
//|		0		|	 1, 2, 4, 8, 16		| -> Each pixel is a grayscale sample
//|		2		|		 8, 16			| -> Each pixel is an R,G,B triple
//|		3		|	   1, 2, 4, 8		| -> Each pixel is a palette index. A PLTE chunk shall appear
//|		4		|	     8, 16			| -> Each pixel is a greyscale sample followed by an alpha sample
//|		6		|	     8, 16			| -> Each pixel is an R,G,B triple followed by an alpha sample

struct BMP
{
	//Total memory usage: 64 Bytes. 16 chuncks of 4 bytes on x32 (x86) systems. No padding required
	char Header[54];					//BMP Default. Each file file has a 54 byte header
	uint8 DataPos;						//Position where the data flow begins
	uint8 BitsPerPixel;					//Image resolution. Found on Header[28]. Possible values: [1, 4, 8, 16, 24, 32] for [monochrome, 16col, 256col, 16bit, 24bit, 32bit]
	uint8 Compression;					//Image compression method. Found on bitmask 0x1E. Value range: [0, 1, 2, 3] for [none, rle 8-bit/pixel, rle 4-bit/pixel, bitfields]
	bool is_infoCatch = false;			//If the "catchBMPInfo" was allready called, skip the size checking and allocate memory using the ImgSize
	uint16 ImgWidth, ImgHeigth;			//Image's Width and Heigth
	uint32 ImgSize;						//Image's Size
	unsigned char *ImgData;				//Image's Data
	void setBMP(char *hedr);			//Actual constructor
	void catchBMPInfo(char *hedr);		//Custom constructor to only get info (don't prepare ImgData)
	void unloadBMP(void);
};

struct PNG
{
	//Will have 3 byte padding after "ColorType"
	char Header[13];					//PNG Default. Each file has at least a 13 byte header
	uint8 DataPos;						//Actual Data flow position
	uint8 IHDRStart;					//Position where the header start
	uint32 ImgWidth;					//PNG's Width and Heigth data (they are stored as 4 bytes unsigned int)
	uint32 ImgHeigth;					//In this format, we can force-convert BigEndian mode to LittleEndian mode
	uint32 ImgSize;						//Image's Size
	int8 BitsPerPixel;					//Image resolution
	int8 FilterMethod;					//Filter Method. By Standard, it is [0] for [Adaptive filtering with five basic filter types]
	int8 Compression;					//Compression Method. By Standard, it is [0] for [Deflate/inflate compression with a sliding window of at most 32768 bytes]
	int8 InterlaceMethod;				//Interlaced Method (indicates the transmission order of the image data). Possible values: [0, 1] for [NO interlace, Adam7 interlace]
	int8 ColorType;						//Defines the PNG color mode. Possible values: [0, 2, 3, 4, 6] for [Grayscale, RGB, Indexed Color, Gray + Alpha, RGBA]
	unsigned char *ImgData;				//Image's Data
	void setPNG(char *hedr);			//Actual constructor (wait setPNGBuffer to prepare ImgData)
	void convertPNGWH(void);			//Convert PNG's Width and Heigth from BigEndian to LittleEndian
	void setPNGBuffer(FILE *file);		//Prepares the ImgData buffer
	void catchPNGInfo(char *hedr);		//Custom constructor to only get info (don't prepare ImgData)
	void unloadPNG(void);
};
#endif;