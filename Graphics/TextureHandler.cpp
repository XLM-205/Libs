/*-------Texture Handler (WIP)-------
*	--Reads and generated textures based on selected Graphical API
*		--Require "ImageLoader.h" for reading the image data
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

#include "ImageLoader.h"
#include "TextureHandler.h"
#include <string.h>

//TODO: Add support for MWI, for multi API deployment

#ifdef _API_GL			//If we gonna use OpenGL, use this method

GLuint MWTHLoadTexture(char *Filename)
{
	GLuint Texture = 0;
	ImgLoader Image(Filename);
	if (!strcmp(Image.getFileFormat(), MWIL_IS_BMP))		//strcmp return 0 for equal strings, and 0 is boolean false, therefore, !0 = 1 = boolean true
	{
		Image.swapRedBlue();
		if (Image.haveAlphaCh())
		{
			Image.fixBMPAlpha();
		}
	}

	//Gera a textura. Paramentros: (quantidade de texturas, memoria que sera salva)
	glGenTextures(1, &Texture);
	//Produz a textura, e tambem a chama quando for preciso (nesse caso, esta sendo usado para produzir a textura)
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Configura a textura. Parametros: (Tipo de textura, nivel de detalhe, numero de componentes de dados, largura, altura, borda, formato das cores, tipo de dados [unsigned byte], dados da imagem)
	if (Image.haveAlphaCh())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image.getImageWidth(), Image.getImageHeigth(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.getImageData());
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image.getImageWidth(), Image.getImageHeigth(), 0, GL_RGB, GL_UNSIGNED_BYTE, Image.getImageData());
	}

	
	//Define a filtragem da textura:
	//Mag -> Imagem maior que a textura | Min -> Imagem menor que a textura
	//Parametros: (Tipo da textura, tipo de filtro, tipo de filtragem [linear filtering])
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Image.unloadImage();
	return Texture;
}

TextureInfo MWTHInfoLoadTexture(char *Filename)
{
	TextureInfo Info;
	ImgLoader Image(Filename);
	if (!strcmp(Image.getFileFormat(), MWIL_IS_BMP))		//strcmp return 0 for equal strings, and 0 is boolean false, therefore, !0 = 1 = boolean true
	{
		Image.swapRedBlue();
		if (Image.haveAlphaCh())
		{
			Image.fixBMPAlpha();
		}
	}

	//Gera a textura. Paramentros: (quantidade de texturas, memoria que sera salva)
	glGenTextures(1, &Info.Texture);
	//Produz a textura, e tambem a chama quando for preciso (nesse caso, esta sendo usado para produzir a textura)
	glBindTexture(GL_TEXTURE_2D, Info.Texture);

	//Configura a textura. Parametros: (Tipo de textura, nivel de detalhe, numero de componentes de dados, largura, altura, borda, formato das cores, tipo de dados [unsigned byte], dados da imagem)
	if (Image.haveAlphaCh())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image.getImageWidth(), Image.getImageHeigth(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.getImageData());
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image.getImageWidth(), Image.getImageHeigth(), 0, GL_RGB, GL_UNSIGNED_BYTE, Image.getImageData());
	}

	//Define a filtragem da textura:
	//Mag -> Imagem maior que a textura | Min -> Imagem menor que a textura
	//Parametros: (Tipo da textura, tipo de filtro, tipo de filtragem [linear filtering])
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Info.Density = Image.getImageDensity();
	Info.have_alpha = Image.haveAlphaCh();
	Info.is_indexed = Image.isIndexed();
	Info.Width = Image.getImageWidth();
	Info.Heigth = Image.getImageHeigth();
	Info.copyFileStateData(Image.getFileStateDataPointer());

	Image.unloadImage();
	return Info;
}

GLuint MWTHLoadTextureEx(char *Filename, ThreadController *TGT)
{
	GLuint Texture = 0;
	ImgLoader Image;
	Image.setThreadControllerPointer(TGT);
	Image.Load(Filename);
	if (!strcmp(Image.getFileFormat(), MWIL_IS_BMP))		//strcmp return 0 for equal strings, and 0 is boolean false, therefore, !0 = 1 = boolean true
	{
		Image.swapRedBlue();
		if (Image.haveAlphaCh())
		{
			Image.fixBMPAlpha();
		}
	}

	//Gera a textura. Paramentros: (quantidade de texturas, memoria que sera salva)
	glGenTextures(1, &Texture);
	//Produz a textura, e tambem a chama quando for preciso (nesse caso, esta sendo usado para produzir a textura)
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Configura a textura. Parametros: (Tipo de textura, nivel de detalhe, numero de componentes de dados, largura, altura, borda, formato das cores, tipo de dados [unsigned byte], dados da imagem)
	if (Image.haveAlphaCh())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image.getImageWidth(), Image.getImageHeigth(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.getImageData());
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image.getImageWidth(), Image.getImageHeigth(), 0, GL_RGB, GL_UNSIGNED_BYTE, Image.getImageData());
	}


	//Define a filtragem da textura:
	//Mag -> Imagem maior que a textura | Min -> Imagem menor que a textura
	//Parametros: (Tipo da textura, tipo de filtro, tipo de filtragem [linear filtering])
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Image.unloadImage();
	return Texture;
}

TextureInfo MWTHInfoLoadTextureEx(char *Filename, ThreadController *TGT)
{
	TextureInfo Info;
	ImgLoader Image;
	Image.setThreadControllerPointer(TGT);
	Image.Load(Filename);
	if (!strcmp(Image.getFileFormat(), MWIL_IS_BMP))		//strcmp return 0 for equal strings, and 0 is boolean false, therefore, !0 = 1 = boolean true
	{
		Image.swapRedBlue();
		if (Image.haveAlphaCh())
		{
			Image.fixBMPAlpha();
		}
	}

	//Gera a textura. Paramentros: (quantidade de texturas, memoria que sera salva)
	glGenTextures(1, &Info.Texture);
	//Produz a textura, e tambem a chama quando for preciso (nesse caso, esta sendo usado para produzir a textura)
	glBindTexture(GL_TEXTURE_2D, Info.Texture);

	//Configura a textura. Parametros: (Tipo de textura, nivel de detalhe, numero de componentes de dados, largura, altura, borda, formato das cores, tipo de dados [unsigned byte], dados da imagem)
	if (Image.haveAlphaCh())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image.getImageWidth(), Image.getImageHeigth(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.getImageData());
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image.getImageWidth(), Image.getImageHeigth(), 0, GL_RGB, GL_UNSIGNED_BYTE, Image.getImageData());
	}

	//Define a filtragem da textura:
	//Mag -> Imagem maior que a textura | Min -> Imagem menor que a textura
	//Parametros: (Tipo da textura, tipo de filtro, tipo de filtragem [linear filtering])
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Info.Density = Image.getImageDensity();
	Info.have_alpha = Image.haveAlphaCh();
	Info.is_indexed = Image.isIndexed();
	Info.Width = Image.getImageWidth();
	Info.Heigth = Image.getImageHeigth();
	Info.copyFileStateData(Image.getFileStateDataPointer());

	Image.unloadImage();
	return Info;
}

#elif _API_DX			//If  it is DirectX, use this instead

#else
#error [MWTH] -> INVALID OR NONE API SELECTED. USE _API_GLUT OR _API_DX
#endif