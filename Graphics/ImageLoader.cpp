/*-------Moon Wiz Image Loader (WIP)-------
*	Alias: MWIL
*	Reads and process an image flstFileData
*	--Image reading and allocation handler
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
*	Version 0.85
*/

//Newer versions:
//sprintf() -> snprintf() or _snprintf()

#include <stdlib.h>
#include "ImageLoader.h"

#include <iostream>
#include <time.h>


//#define MWIL_DISPLAY_PROGRESS			//Display progress of reading / mounting / fixing / etc... of image		
//#define MWIL_DEBUG_TIMES				//Display spent time in which reading job
//#define MWIL_DEBUG_VERBOSE			//Display reading actions

#define ALLOC_SAFE_MARGIN 16			//How many bytes extra will be allocated to prevent heap corruption upon deleting memory

ImgLoader::ImgLoader(char *Filename)
{
	openFile(Filename, "rb");
	if (m_stateCode == FileStatus::FS_SUCCESS)
	{
		sprintf(FlPath, "%s", Filename);
		setImageFormat();
		is_imageSucess = true;
	}
	else if (m_stateCode == FileStatus::FS_ERROR_NO_FILE)
	{
		sprintf((char*)ImgType, MWIL_IS_NOT_FOUND);
		is_imageSucess = false;
	}
	else
	{
		is_imageSucess = false;
	}
	/*if (m_stateCode != FileStatus::FS_ERROR_NO_PATH)
	{
		setImageName(FlPath);
	}*/
}

ImgLoader::ImgLoader(char *Filename, uint8 LoadingMode)
{
	//TODO: Image loading methods
	Load(Filename);
}

ImgLoader::~ImgLoader(void)
{
	unloadImage();
	if (FIImageData != NULL)
	{
		delete[] FIImageData;
		FIImageData = NULL;
	}

}

void ImgLoader::swapRedBlue(void)
{
	if (m_stateCode == FileStatus::FS_SUCCESS)		//ONLY try to filter, IF an image has been loaded
	{
		unsigned char Red, Blue;			//To convert BGR to RGB
		uint8 DensityMultiplier = 3;
		uint32 Index = 0;					//CANNOT BE SIGNED!!
		uint32 VerSize = 0;
		if (have_Alpha)
		{
			DensityMultiplier = 4;
		}
		if (ImgType[0] == 'B')		//Checking if it's BMP
		{
			VerSize = (Width * Height) / DensityMultiplier;
			for (int i = 0; i <= VerSize; i++)
			{
				Index = i * DensityMultiplier;
				Blue = FIImageData[Index];
				Red = FIImageData[Index + 2];

				FIImageData[Index] = Red;
				FIImageData[Index + 2] = Blue;
			}
		}
		else if (ImgType[0] == 'P')	//PNG
		{
			//VerSize = PNGImage.ImgWidth * PNGImage.ImgHeigth;
			// !! (WIP) !!
		}
	}
}

void ImgLoader::fixBMPAlpha(void)
{
	uint8 DensityMultiplier = 3;
	uint32 FakeAlpha = 0;				//Detects if a image have an alpha channel, but doesn't use it (all alpha pixels == 0)
	uint32 Index = 0;					//CANNOT BE SIGNED!!
	uint32 VerSize = 0;
	if (have_Alpha)
	{
		DensityMultiplier = 4;
	}
	VerSize = (Width * Height) / DensityMultiplier;
	for (int i = 0; i <= VerSize; i++)
	{
		Index = i * DensityMultiplier;
		if (have_Alpha)
		{
			if ((FIImageData[Index + 3] != 0) && (i != VerSize))
			{
				FakeAlpha++;
			}
		}
	}
	if (FakeAlpha == 0)	//If true, then the alpha channel is present but not in use. Flip all alpha data to 255 (opaque)
	{
		for (int i = 0; i <= VerSize; i++)
		{
			Index = i * DensityMultiplier;
			FIImageData[Index + 3] = 255;
		}
	}
}

void ImgLoader::Load(char *Filename)
{
	openFile(Filename, "rb");
	if (m_stateCode == FileStatus::FS_SUCCESS)
	{
		sprintf(FlPath, "%s", Filename);
		setImageFormat();
		setImageName(FlPath);
		is_imageSucess = true;
	}
	else if (m_stateCode == FileStatus::FS_ERROR_NO_FILE)
	{
		sprintf((char*)ImgType, MWIL_IS_NOT_FOUND);
		is_imageSucess = false;
	}
	else
	{
		is_imageSucess = false;
	}
}

void ImgLoader::setImageFormat(void)
{
//#ifdef MWIL_DEBUG_VERBOSE
//	std::cout << "[IMGLOADER] Image Reading Started\t---------------------------------------\n";
//#endif
//	uint8 ByteCheck = fread(PreLoadHeader, 1, 54, flstFileData);
//	//Checking for BMP Standard. First 2 bytes are "B" and "M" and the header has 54 bytes in length. The 54 byte is a \0
//	if ((ByteCheck == 54) && (PreLoadHeader[0] == 'B' && PreLoadHeader[1] == 'M'))
//	{
//		sprintf(ImgType, MWIL_IS_BMP);
//		BMPImage.setBMP(PreLoadHeader);
//		fread(BMPImage.ImgData, BMPImage.ImgSize, 1, flstFileData);
//		if (BMPImage.BitsPerPixel == 32)
//		{
//			have_Alpha = true;
//		}
//		else
//		{
//			have_Alpha = false;
//		}
//	}
//	//Checking for PNG Standard. First byte is random, SIGNATURE have 8 bytes total, and SIGNATURE[1~3] is 'P','N' and 'G'. On PNG, this is NOT the header
//	//On PNG, the header have 13 bytes, total, after the SIGNATURE (8 bytes)
//	else if ((ByteCheck >= 8) && (PreLoadHeader[1] == 'P' && PreLoadHeader[2] == 'N' && PreLoadHeader[3] == 'G'))
//	{
//		sprintf(ImgType, MWIL_IS_PNG);
//		PNGImage.setPNG(PreLoadHeader);
//		fseek(flstFileData, PNGImage.IHDRStart, SEEK_SET);
//		fread(&PNGImage.ImgWidth, 4, 1, flstFileData);
//		fread(&PNGImage.ImgHeigth, 4, 1, flstFileData);
//		PNGImage.convertPNGWH();
//		//PNGImage.setPNGBuffer(flstFileData);
//		if (PNGImage.ColorType == 4 ||PNGImage.ColorType == 6)
//		{
//			have_Alpha = true;
//		}
//		else if (PNGImage.ColorType == 3)
//		{
//			have_Alpha = true;
//			is_indexed = true;
//		}
//		else
//		{
//			have_Alpha = false;
//		}
//		processFreeImage(FIF_PNG, PNG_DEFAULT);
//		//BMPImage.~BMP();					//If the format is PNG, then, we can free up the rest of memory alocated for the other image types
//	}
//	//If format is not recognized, then flstFileData is unsupported
//	else
//	{
//		sprintf(ImgType, MWIL_IS_UNSUPPORTED);
//	}
//	fclose(flstFileData);							//Data alocated and read into memory. File stream can be closed
//	sprintf(Resolution, "%d x %d", getImageWidth(), getImageHeigth());
//#ifdef MWIL_DEBUG_VERBOSE
//	std::cout << "[IMGLOADER] Image Reading Finished\t---------------------------------------\n";
//#endif
	processFreeImage(FreeImage_GetFileType(FlPath, 0), 0);
}

//void ImgLoader::setThreadControllerPointer(ThreadController* TGT)
//{
//	//Doesn't check against errors, because the main caller SHOULD do that instead of this
//	ExternalThrdCtrl = TGT;
//}

void ImgLoader::processFreeImage(FREE_IMAGE_FORMAT Format, uint8 Flag)
{
#ifdef MWIL_DISPLAY_PROGRESS
	sprintf(CurrentWorkWr, "Processing PNG");
#endif
#ifdef MWIL_DEBUG_TIMES
	int d_TimeWasted = clock();
	int d_TotalWasted = 0;
#endif
	FIImage = FreeImage_Load(FreeImage_GetFileType(FlPath), FlPath, Flag);
#ifdef MWIL_DEBUG_TIMES
	std::cout << "[IMGLOADER] Time spent in first load [PNG]: " << clock() - d_TimeWasted << "ms\n";
	d_TotalWasted += clock() - d_TimeWasted;
#endif
	uint8 DensityMultiplier = 0;
	uint32 ImageSize, Index;
	uint8 *PixelTempBuffer;
	Width = FreeImage_GetWidth(FIImage);
	Height = FreeImage_GetHeight(FIImage);
	if (is_indexed)
	{
		FIBITMAP *TempPointer = FIImage;
		#ifdef MWIL_DEBUG_TIMES
				d_TimeWasted = clock();
		#endif
		FIImage = FreeImage_ConvertTo32Bits(FIImage);

		#ifdef MWIL_DEBUG_TIMES
				std::cout << "[IMGLOADER] Time spent converting (Index 1st Pass) [PNG]: " << clock() - d_TimeWasted << "ms\n";
				d_TotalWasted += clock() - d_TimeWasted;
		#endif
		FreeImage_Unload(TempPointer);
	}
	PixelTempBuffer = FreeImage_GetBits(FIImage);
	//if (Format == FIF_PNG)
	{	
		//Saving by "Color Chunk mode" (saving entire color set, not width * heigth * colors)
		ImageSize = FreeImage_GetWidth(FIImage) * FreeImage_GetHeight(FIImage);
		DensityMultiplier = getImageDensity() >> 3;
		if (DensityMultiplier - 3)
		{
			have_Alpha = true;
		}
	}
	uint8 Temp = FreeImage_GetPitch(FIImage);
#ifdef MWIL_DEBUG_TIMES
	d_TimeWasted = clock();
#endif
	FIImageData = new uint8[ImageSize * DensityMultiplier];
#ifdef MWIL_DEBUG_TIMES
	std::cout << "[IMGLOADER] Time spent allocating [PNG]: " << clock() - d_TimeWasted << "ms\n";
	d_TotalWasted += clock() - d_TimeWasted;
#endif
	//if (ExternalThrdCtrl != NULL && false)	//"false" temporary so we can safely use the application while in the MTH tests
	//{
	//	//TODO: Multi-threading support
	//}
	//else
	{
	#ifdef MWIL_DEBUG_TIMES
		d_TimeWasted = clock();
	#endif
		for (int i = 0; i < ImageSize; i++)
		{
			Index = i * DensityMultiplier;
			FIImageData[Index] = PixelTempBuffer[Index + 2];
			FIImageData[Index + 1] = PixelTempBuffer[Index + 1];
			FIImageData[Index + 2] = PixelTempBuffer[Index + 0];
			if (have_Alpha)
			{
				FIImageData[Index + 3] = PixelTempBuffer[Index + 3];
			}
		}
	#ifdef MWIL_DEBUG_TIMES
		std::cout << "[IMGLOADER] Time spent converting data formats (Index 2nd Pass) [PNG]: " << clock() - d_TimeWasted << "ms\n";
		d_TotalWasted += clock() - d_TimeWasted;
		std::cout << "[IMGLOADER] Total Time spent [PNG]: " << d_TotalWasted << "ms or " << d_TotalWasted * 0.001 << "s\n";
	#endif
	}
}

void ImgLoader::setImageName(char *Filename)
{
	uint8 Index = 0, i = 0, j = 0;
	for (i = 0; Filename[i] != '\0'; i++)
	{
		if (Filename[i] == '\\' || Filename[i] == '/')
		{
			Index = i;
		}
	}
	//flstName = (char*)malloc(sizeof(char)*(i - Index));
	m_name->setString(&Filename[Index]);
	/*for (i = Index + 1, j = 0; Filename[i] != '\0'; i++, j++)
	{
		flstName[j] = Filename[i];
	}
	flstName[j] = '\0';*/
}

int ImgLoader::getImageHeigth(void)
{
	return Height;
	//if (ImgType[0] == 'B')			//BMP
	//{
	//	return BMPImage.ImgHeigth;
	//}
	//else if (ImgType[0] == 'P')		//PNG
	//{
	//	return PNGImage.ImgHeigth;
	//}
	//else							//Unknown type
	//{
	//	return NULL;
	//}
	//TODO: Case for all image types and exceptions
}

int ImgLoader::getImageWidth(void)
{
	return Width;
	//if (ImgType[0] == 'B')			//BMP
	//{
	//	return BMPImage.ImgWidth;
	//}
	//else if (ImgType[0] == 'P')		//PNG
	//{
	//	return PNGImage.ImgWidth;
	//}
	//else							//Unknown type
	//{
	//	return NULL;
	//}
	//TODO: Case for all image types and exceptions
}

unsigned char ImgLoader::getImageDensity(void)
{
	return FreeImage_GetBPP(FIImage);
	//if (ImgType[0] == 'B')			//BMP
	//{
	//	return BMPImage.BitsPerPixel;
	//}
	//else if (ImgType[0] == 'P')		//PNG
	//{
	//	return PNGImage.BitsPerPixel;
	//}
	//else							//Unknown type
	//{
	//	return NULL;
	//}
}

void* ImgLoader::getImageData(void)
{
	return FIImageData;
}

void ImgLoader::unloadImage(void)
{
	delete[] FIImageData;
	FIImageData = nullptr;
	FreeImage_DestroyICCProfile(FIImage);
	FreeImage_Unload(FIImage);
	FIImage = nullptr;
}

#undef MAX_LEN_FILEPATH 256