/*-------Moon Wiz Image Loader (WIP)-------
*	Alias: MWIL
*	Reads and process an image file
*	--Image reading and allocation handler
*		--Require "CustomTypes.h" for custom variables definitions
*		--Require "ImageHandler.h" for handling the file data
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
*	Version 0.85
*/

#ifndef _H_MWIMGLOADER_
#define _H_MWIMGLOADER_
//Moon Wiz Image Loader
//MWIL -> MoonWizImageLoader

//File Warnings
#define MWIL_FILE_SUCESS		  1	//File FOUND and READ
#define MWIL_ERROR_NO_PATH		 -1	//NO file path/name given
#define MWIL_ERROR_NO_FILE		 -2	//NO file found/read with given file name
#define MWIL_ERROR_INVALID_FILE  -3	//File is not supported/not a valid image file
//File Message Warnings
#define MWIL_WR_FILE_SUCCESS		"FILE SUCCESSFULLY READ"
#define	MWIL_WR_ERROR_NO_PATH		"ERROR! NO PATH GIVEN"
#define MWIL_WR_ERROR_NO_FILE		"ERROR! NO FILE FOUND"
#define MWIL_WR_ERROR_INVALID_PATH	"ERROR! INVALID/UNREADABLE FILE"
//File Types
#define MWIL_IS_BMP			"BMP\0"
#define MWIL_IS_PNG			"PNG\0"
#define MWIL_IS_JPG			"JPG\0"
#define MWIL_IS_TGA			"TGA\0"
#define MWIL_IS_NOT_FOUND	"NONE\0"
#define MWIL_IS_UNKNOW		"UNKW\0"
#define MWIL_IS_UNSUPPORTED	"UNSP\0"
//Loading Types
#define MWIL_LOAD_INFO		0	//Just reads the file for it's info, no image buffering, scale nor filtering 
#define MWIL_LOAD_NORMAL	1	//Loads the file using normal procedures
#define MWIL_LOAD_COMPLETE	2	//Loads the file and gather extra info, like file name
#define MWIL_LOAD_FAST		3	//Custom reading routine: Shortcut for faster reading and access, bypassing some unnecessary IF statements
//Specific definitions (undef on end of library)
#define MAX_PRE_LOAD_BUFFER	256	//Max size of the pre-load buffer
#define MAX_LEN_FILEPATH    256	//Max filepath length

//#include <fstream>				//To replaces stdio's FILE (stdio's FILE is supposed to be faster thought)
#include <stdio.h>
#include "ImageHandler.h"
#include "..\Control\CustomTypes-SA.h"
#include "..\Control\ThreadController.h"
#include "..\Control\FileStates.h"
//Plataform Especific Libraries
#ifdef X86
	#include <FreeImage\Dist\x32\FreeImage.h>
#endif;
#ifdef X64
	#include <FreeImage\Dist\x64\FreeImage.h>
#endif;

static unsigned int ImgID;			//For multi image support and control (ImgLoader **Var)

//TODO: Support for 16bits BMP images

class ImgLoader : public FileStates
{
public:
	ImgLoader(void) { };										//Don't construct, only instantiates (possibly VERY UNSAFE)
	ImgLoader(char *Filename);									//Contructs, and calls "Load", uses the default loading procedure (MWIL_READ_NORMAL)
	ImgLoader(char *Filename, uint8 LoadingMode);				//Contructs, and calls "Load", but uses user defined loading method
	~ImgLoader(void);

	bool haveAlphaCh(void)		{ return have_Alpha; };			//Return true if the image has alpha channel and false if it doesn't
	bool isIndexed(void)		{ return is_indexed; };			//Return true if the image is indexed / have color pallete an false if doesn't
	bool isImageSucess(void)	{ return is_imageSucess; };		//Return true if the image was sucefully loaded, and false, if it failed
	int getImageWidth(void);									//Return the image's width
	int getImageHeigth(void);									//Return the image's heigth
	unsigned char getImageDensity(void);						//Return the image's bits-per-pixel (pixel density)
	BaseString* getImageName(void)	{ return m_name; };		//Return the file's name
	char* getFileFormat(void)	{ return ImgType; };			//Return the image format
	char* getResolution(void)	{ return Resolution; };			//Return the image's resolution written (like 600x800, string format)
	void* getImageData(void);									//Return the file's processed image data (the meaning of this thing FYI)
	void Load(char *Filename);									//Loads a image
	void swapRedBlue(void);										//Swap an image's red and blue colors
	void fixBMPAlpha(void);										//Detects and fix BMP images that have an alpha channel, but doesn't use it
	void unloadImage(void);										//Unloads all the data
	void setThreadControllerPointer(ThreadController* TGT);		//Sets a target thread controller instance

protected:
	char PreLoadHeader[MAX_PRE_LOAD_BUFFER];					//Pre-loaded header to image file. Used to detect image format
	char FlPath[MAX_LEN_FILEPATH];								//File Path
	char ImgType[5];											//Image Format

private:
	FIBITMAP *FIImage = NULL;										//For FreeImage's reading
	uint8 *FIImageData = NULL;										//Actual FreeImage's data
	ThreadController *ExternalThrdCtrl = NULL;						//Pointer to an external instance of a Thread Controller. If NOT set to NULL, the ImageLoader will try to use multi-thread reading
	char Resolution[16];											//Image resolution (e.g.: 600 x 800)
	bool have_Alpha = false;										//Image has alpha channel?
	bool is_indexed = false;										//Is the image a index/pallete color image?
	bool is_imageSucess = false;									//Is the image sucefully loaded?
	int Width = 0;
	int Height = 0;
	void processFreeImage(FREE_IMAGE_FORMAT Format, uint8 Flag);	//Process the image data using FreeImage's library
	void setImageName(char *FileName);								//Gets and sets the image name and format
	void setImageFormat(void);										//Gets and sets the image format and reads the file for data
};

#undef MAX_PRE_LOAD_BUFFER
#endif;	//Moon Wiz Image Loader