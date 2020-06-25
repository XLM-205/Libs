/*-------Texture Handler (WIP)-------
*	--Reads and generated textures based on selected Graphical API 
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
*	Version 0.88
*/

#ifndef _H_MWTEXHANDLER_
#define _H_MWTEXHANDLER_
//Moon Wiz Texture Handler

//TODO: Add support for MWI, for multi API deployment

#include "..\Control\ThreadController.h"

struct TextureInfo : public FileStates
{
	bool have_alpha;		//The target texture have alpha channel?
	bool is_indexed;		//The target texture is indexed
	uint8 Density;			//The density (a.k.a bits-per-pixel) of the texture
	uint16 Width;			//The texture Width
	uint16 Heigth;			//The texture Heigth
	uint32 Texture;			//The texture pointer. Use this to load a texture

	TextureInfo(void)
	{
		is_indexed = false;
		have_alpha = false;
		Density = 0;
		Width = 0;
		Heigth = 0;
		Texture = 0;
	}
};

#define _API_GL			//API set to the texture handler. OpenGL mode
//#define _API_DX		//API set to the texture handler. DirectX mode

#ifdef _API_GL			//If we gonna use OpenGL, use this method
#include <GL\freeglut.h>

uint32 MWTHLoadTexture(char *Filename);				//Generates the texture and return it's pointer
TextureInfo MWTHInfoLoadTexture(char *Filename);	//Generates the texture and return additional data about it

uint32 MWTHLoadTextureEx(char *Filename, ThreadController *TGT);			//Generates the texture and return it's pointer and sets a ThreadController Pointer
TextureInfo MWTHInfoLoadTextureEx(char *Filename, ThreadController *TGT);	//Generates the texture and return additional data about it and sets a ThreadController Pointer

#elif _API_DX			//If is DirectX, use this instead

#else
#error [MWTH] -> INVALID OR NONE API SELECTED. USE _API_GLUT OR _API_DX
#endif

#endif;	//MWTH