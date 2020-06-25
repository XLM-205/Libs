/*-------DOCUMENTATION PENDING-------
*
*	Moon Wiz Studios (c) - --/--/----
*	by: Ramon Darwich de Menezes
*
*	This work is a copyright of Moon Wiz Studios
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 1.2.0
*/

#include <stdio.h>
#include "ImagePanel.h"
#include "TextureHandler.h"

//#define MWIP_DISPLAY_PROGRESS			//Display progress of reading / mounting / fixing / etc... of panels
//#undef MWIP_DISPLAY_PROGRESS

//Internal Macros----------------------------------------------------------------------------------------------------------------------------------
#define I_MWIP_BASE_SCALE					1		//(internal) used when constructed with "XScale" / "YScale" option. Must not be 0

//Holds the texture position for easy orientation changing. Change the first array slot to change between each orientation mode
//The second slot is for each call, and the last is the data. [0] for X and [1] for Y
//Texture mapping sequence: LT -> RT -> LB -> RB
// ==> (0, 1) / (1, 1) / (0, 0) / (1, 0) / (1, 1)
//L = Left | R = Right | T = Top | B = Bottom
//Using a clockwise sequence: Top -> right -> bottom -> left

//Default UV Data Holder (CONSTANT)
static const float DEFAULTOrient[4][4][2] =
{ { { 0, 1 }, { 1, 1 }, { 0, 0 }, { 1, 0 } },
  { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
  { { 1, 0 }, { 0, 0 }, { 1, 1 }, { 0, 1 } },
  { { 1, 1 }, { 1, 0 }, { 0, 1 }, { 0, 0 } }, };

ImagePanel::ImagePanel(void)
{
	UVA = 0;
	UVB = 1;
	setOrientToDefault();
	XRatio = 1;
	YRatio = 1;
	NormalAmount = 0;
	PanelBackColor[3] = PanelBackColor[2] = PanelBackColor[1] = PanelBackColor[0] = 255;
	PanelOrientation[1] = PanelOrientation[0] = MWIP_ORIENTATION_UP;
}

ImagePanel::ImagePanel(mwVec3f &PositionMax, mwVec3f &PositionMin, char *ImagePath, uint8 Dimension)
{
	is_textured = true;
	PointA = PositionMax;
	PointB = PositionMin;
	setDimension(Dimension);
	Texture[0] = MWTHLoadTexture(ImagePath);
	UVA = 0;
	UVB = 1;
	XRatio = 1;
	YRatio = 1;
	PanelBackColor[3] = PanelBackColor[2] = PanelBackColor[1] = PanelBackColor[0] = 255;
	PanelOrientation[1] = PanelOrientation[0] = MWIP_ORIENTATION_UP;
	setOrientToDefault();
}

ImagePanel::ImagePanel(mwVec3f &PositionMax, mwVec3f &PositionMin, uint32 LTexture, uint8 Dimension)
{
	is_textured = true;
	PointA = PositionMax;
	PointB = PositionMin;
	setDimension(Dimension);
	Texture[0] = LTexture;
	UVA = 0;
	UVB = 1;
	XRatio = 1;
	YRatio = 1;
	PanelBackColor[3] = PanelBackColor[2] = PanelBackColor[1] = PanelBackColor[0] = 255;
	PanelOrientation[1] = PanelOrientation[0] = MWIP_ORIENTATION_UP;
	setOrientToDefault();
}

ImagePanel::ImagePanel(mwVec3f &PositionMax, mwVec3f &PositionMin, char *ImagePath1, char *ImagePath2, uint8 Dimension)
{
	is_textured = true;
	PointA = PositionMax;
	PointB = PositionMin;
	is_panelSingleFace = false;
	setDimension(Dimension);
	Texture[0] = MWTHLoadTexture(ImagePath1);
	Texture[1] = MWTHLoadTexture(ImagePath2);
	UVA = 0;
	UVB = 1;
	XRatio = 1;
	YRatio = 1;
	PanelBackColor[3] = PanelBackColor[2] = PanelBackColor[1] = PanelBackColor[0] = 255;
	PanelOrientation[1] = PanelOrientation[0] = MWIP_ORIENTATION_UP;
	setOrientToDefault();
}

ImagePanel::ImagePanel(mwVec3f &PositionMax, mwVec3f &PositionMin, uint32 LTexture, uint32 LTexture2, uint8 Dimension)
{
	is_textured = true;
	PointA = PositionMax;
	PointB = PositionMin;
	is_panelSingleFace = false;
	setDimension(Dimension);
	Texture[0] = LTexture;
	Texture[1] = LTexture2;
	UVA = 0;
	UVB = 1;
	XRatio = 1;
	YRatio = 1;
	PanelBackColor[3] = PanelBackColor[2] = PanelBackColor[1] = PanelBackColor[0] = 255;
	PanelOrientation[1] = PanelOrientation[0] = MWIP_ORIENTATION_UP;
	setOrientToDefault();
}

void ImagePanel::allocNormals(void)
{
	if (have_normals)
	{
		purgeNormalsData();
		PanelNormals = new mwVec3f[NormalAmount];
	}
}

void ImagePanel::buildNormalData(void)
{
	if (have_normals)
	{
		PanelNormals[0]._X = 1;
		if (!is_panelSingleFace)
		{
			PanelNormals[1]._X = -1;
		}
		is_normalsBuilt = true;
	}
}

void ImagePanel::computeRatio(uint8 Side)
{
	switch (Side)
	{
	case MWIP_SIDE_BACK:
		break;
	case MWIP_SIDE_BOTH:
		computeRatio(MWIP_SIDE_BACK);
	case MWIP_SIDE_FRONT:
	default:
		Side = MWIP_SIDE_FRONT;
		break;
	}
	if (TextureHeigth[Side] <= 0 || TextureWidth[Side] < 0)
	{
		GLint Width = 0, Height = 0;
		glBindTexture(GL_TEXTURE_2D, Texture[Side]);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &Width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &Height);
		if (Width >= Height)
		{
			XRatio = (float)Width / (float)Height;
			YRatio = 1;
		}
		else
		{
			YRatio = (float)Height / (float)Width;
			XRatio = 1;
		}
		//TODO - Update code to handle ratio on both sides. Base code is allready here \/
		if (!is_panelSingleFace)
		{
			glBindTexture(GL_TEXTURE_2D, Texture[1]);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &Width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &Height);
			if (Width >= Height)
			{
				XRatio = (float)Width / (float)Height;
				YRatio = 1;
			}
			else
			{
				YRatio = (float)Height / (float)Width;
				XRatio = 1;
			}
		}
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
	else
	{
		if (TextureWidth >= TextureHeigth)
		{
			XRatio = (float)TextureWidth[Side] / (float)TextureHeigth[Side];
			YRatio = 1;
		}
		else
		{
			YRatio = (float)TextureHeigth[Side] / (float)TextureWidth[Side];
			XRatio = 1;
		}
	}
}

void ImagePanel::copyBasicDataAllSides(ImagePanel &Panel)
{
	PanelOrientation[MWIP_SIDE_FRONT] = Panel.PanelOrientation[MWIP_SIDE_FRONT];
	PanelOrientation[MWIP_SIDE_BACK] = Panel.PanelOrientation[MWIP_SIDE_BACK];
	Texture[MWIP_SIDE_FRONT] = Panel.Texture[MWIP_SIDE_FRONT];
	Texture[MWIP_SIDE_BACK] = Panel.Texture[MWIP_SIDE_BACK];
	TextureWidth[MWIP_SIDE_FRONT] = Panel.TextureWidth[MWIP_SIDE_FRONT];
	TextureWidth[MWIP_SIDE_BACK] = Panel.TextureWidth[MWIP_SIDE_BACK];
	TextureHeigth[MWIP_SIDE_FRONT] = Panel.TextureHeigth[MWIP_SIDE_FRONT];
	TextureHeigth[MWIP_SIDE_BACK] = Panel.TextureHeigth[MWIP_SIDE_BACK];
	sprintf(TextureRatio[MWIP_SIDE_FRONT], Panel.TextureRatio[MWIP_SIDE_FRONT]);
	sprintf(TextureRatio[MWIP_SIDE_BACK], Panel.TextureRatio[MWIP_SIDE_BACK]);
	sprintf(TextureResolution[MWIP_SIDE_FRONT], Panel.TextureResolution[MWIP_SIDE_FRONT]);
	sprintf(TextureResolution[MWIP_SIDE_BACK], Panel.TextureResolution[MWIP_SIDE_BACK]);
	XRatio = Panel.XRatio;
	YRatio = Panel.YRatio;
	is_textured = Panel.is_textured;
	is_textureAlpha = Panel.is_textureAlpha;
	is_panelBuilt = Panel.is_panelBuilt;
	is_child = true;
}

void ImagePanel::copyBasicData(ImagePanel &Panel, uint8 Side)
{
	switch (Side)
	{
	case MWIP_SIDE_BACK:
		break;
	case MWIP_SIDE_FRONT:
	default:
		Side = MWIP_SIDE_FRONT;
	}
	PanelOrientation[Side] = Panel.PanelOrientation[Side];
	Texture[Side] = Panel.Texture[Side];
	TextureWidth[Side] = Panel.TextureWidth[Side];
	TextureHeigth[Side] = Panel.TextureHeigth[Side];
	sprintf(TextureRatio[Side], Panel.TextureRatio[Side]);
	sprintf(TextureResolution[Side], Panel.TextureResolution[Side]);
	XRatio = Panel.XRatio;
	YRatio = Panel.YRatio;
	is_textured = Panel.is_textured;
	is_textureAlpha = Panel.is_textureAlpha;
	is_panelBuilt = Panel.is_panelBuilt;
	is_child = true;
}

void ImagePanel::deleteTextureData(uint8 Side)
{
	switch (Side)
	{
	case MWIP_SIDE_BACK:
		glDeleteTextures(1, &Texture[1]);
		Texture[1] = 0;
		break;
	case MWIP_SIDE_BOTH:
		glDeleteTextures(2, Texture);
		Texture[0] = 0;
		Texture[1] = 0;
		break;
	case MWIP_SIDE_FRONT:
	default:
		glDeleteTextures(1, Texture);
		Texture[0] = 0;
		break;
	}
}

void ImagePanel::destroyPanel(void)
{
	unloadPanelImage();
	resetValues();
}

bool ImagePanel::detectClick(float MouseX, float MouseY)
{
	return detectClick(mwVec2f(MouseX, MouseY));
}

bool ImagePanel::detectClick(mwVec2f &MousePos)
{
	if (is_clickActive)
	{
		if (KeepRatio)
		{
			if ((MousePos._X <= PointA._X + Position._X) && (MousePos._X >= PointB._X + Position._X))
			{
				if ((MousePos._Y <= PointA._Y + Position._Y) && (MousePos._Y >= PointB._Y + Position._Y))
				{
					return true;
				}
			}
			return false;
		}
		else
		{
			if ((MousePos._X <= PointA._X * XRatio + Position._X) && (MousePos._X >= PointB._X * XRatio + Position._X))
			{
				if ((MousePos._Y <= PointA._Y * YRatio + Position._Y) && (MousePos._Y >= PointB._Y * YRatio + Position._Y))
				{
					return true;
				}
			}
			return false;
		}
	}
	else
	{
		return false;
	}
}

void ImagePanel::drawPanel(void)
{
	if (!is_normalsBuilt)
	{
		buildNormalData();
	}
	if (!is_hidden)
	{
		glPushMatrix();
			glTranslatef(Position._X, Position._Y, Position._Z);				//Global position
			glPushMatrix();
				glTranslatef(RotationAxis._X, RotationAxis._Y, RotationAxis._Z);
				glRotatef(Rotation._X, 1, 0, 0);
				glRotatef(Rotation._Y, 0, 1, 0);
				glRotatef(Rotation._Z, 0, 0, 1);
				glTranslatef(-RotationAxis._X, -RotationAxis._Y, -RotationAxis._Z);
				glColor4ubv(PanelBackColor);
				if (is_textured)
				{
					glBindTexture(GL_TEXTURE_2D, Texture[0]);
				}
				glBegin(GL_TRIANGLE_STRIP);
				//Texture mapping sequence: LT -> RT -> LB -> RB
				//L = Left | R = Right | T = Top | B = Bottom
				if (KeepRatio)
				{
					if (is_flipped)
					{
						glTexCoord2fv(Orient[PanelOrientation[0]][0]);
						//Min X / Max Y
						glVertex3f(PointA._X, PointA._Y, PointA._Z);
						glTexCoord2fv(Orient[PanelOrientation[0]][1]);
						//Max X / Max Y
						glVertex3f(PointB._X, PointA._Y, PointA._Z);
						glTexCoord2fv(Orient[PanelOrientation[0]][2]);
						//Min X / Min Y
						glVertex3f(PointA._X, PointB._Y, PointA._Z);
						glTexCoord2fv(Orient[PanelOrientation[0]][3]);
						//Max X / Min Y
						glVertex3f(PointB._X, PointB._Y, PointA._Z);
					}
					else
					{
						glTexCoord2fv(Orient[PanelOrientation[0]][0]);
						glVertex3f(PointB._X, PointA._Y, PointA._Z);
						glTexCoord2fv(Orient[PanelOrientation[0]][1]);
						glVertex3f(PointA._X, PointA._Y, PointA._Z);
						glTexCoord2fv(Orient[PanelOrientation[0]][2]);
						glVertex3f(PointB._X, PointB._Y, PointA._Z);
						glTexCoord2fv(Orient[PanelOrientation[0]][3]);
						glVertex3f(PointA._X, PointB._Y, PointA._Z);
					}

					glEnd();
					if (!is_panelSingleFace)		//Change texture to the back panel
					{
						if (is_textured)
						{
							glBindTexture(GL_TEXTURE_2D, Texture[1]);
						}
						glBegin(GL_TRIANGLES);
						if (is_flipped)
						{
							glTexCoord2fv(Orient[PanelOrientation[1]][0]);
							glVertex3f(PointA._X, PointA._Y, PointB._Z);
							glTexCoord2fv(Orient[PanelOrientation[1]][1]);
							glVertex3f(PointB._X, PointA._Y, PointB._Z);
							glTexCoord2fv(Orient[PanelOrientation[1]][2]);
							glVertex3f(PointA._X, PointB._Y, PointB._Z);
							glTexCoord2fv(Orient[PanelOrientation[1]][3]);
							glVertex3f(PointB._X, PointB._Y, PointB._Z);
						}
						else
						{
							glTexCoord2fv(Orient[PanelOrientation[1]][0]);
							glVertex3f(PointB._X, PointA._Y, PointB._Z);
							glTexCoord2fv(Orient[PanelOrientation[1]][1]);
							glVertex3f(PointA._X, PointA._Y, PointB._Z);
							glTexCoord2fv(Orient[PanelOrientation[1]][2]);
							glVertex3f(PointB._X, PointB._Y, PointB._Z);
							glTexCoord2fv(Orient[PanelOrientation[1]][3]);
							glVertex3f(PointA._X, PointB._Y, PointB._Z);
						}
						glEnd();
					}
				}
				else
				{
					if (AddaptRatioUVMode)
					{
						if (is_flipped)
						{
							glTexCoord2fv(Orient[PanelOrientation[0]][0]);
							glVertex3f(PointA._X, PointA._Y, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][1]);
							glVertex3f(PointB._X * XRatio, PointA._Y, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][2]);
							glVertex3f(PointA._X, PointB._Y * YRatio, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][3]);
							glVertex3f(PointB._X * XRatio, PointB._Y * YRatio, PointA._Z);
						}
						else
						{
							glTexCoord2fv(Orient[PanelOrientation[0]][0]);
							glVertex3f(PointB._X, PointA._Y * YRatio, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][1]);
							glVertex3f(PointA._X * XRatio, PointA._Y * YRatio, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][2]);
							glVertex3f(PointB._X, PointB._Y, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][3]);
							glVertex3f(PointA._X * XRatio, PointB._Y, PointA._Z);
						}

						glEnd();
						if (!is_panelSingleFace)		//Change texture to the back panel
						{
							if (is_textured)
							{
								glBindTexture(GL_TEXTURE_2D, Texture[1]);
							}
							glBegin(GL_TRIANGLES);
							if (is_flipped)
							{
								glTexCoord2fv(Orient[PanelOrientation[1]][0]);
								glVertex3f(PointA._X, PointA._Y, PointA._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][1]);
								glVertex3f(PointB._X * XRatio, PointA._Y, PointA._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][2]);
								glVertex3f(PointA._X, PointB._Y * YRatio, PointA._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][3]);
								glVertex3f(PointB._X * XRatio, PointB._Y * YRatio, PointA._Z);
							}
							else
							{
								glTexCoord2fv(Orient[PanelOrientation[1]][0]);
								glVertex3f(PointB._X * XRatio, PointA._Y, PointA._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][1]);
								glVertex3f(PointA._X, PointA._Y, PointA._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][2]);
								glVertex3f(PointB._X * XRatio, PointB._Y * YRatio, PointA._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][3]);
								glVertex3f(PointA._X, PointB._Y * YRatio, PointA._Z);
							}
							glEnd();
						}
					}
					else
					{
						if (is_flipped)
						{
							glTexCoord2fv(Orient[PanelOrientation[0]][0]);
							glVertex3f(PointA._X * XRatio, PointA._Y * YRatio, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][1]);
							glVertex3f(PointB._X * XRatio, PointA._Y * YRatio, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][2]);
							glVertex3f(PointA._X * XRatio, PointB._Y * YRatio, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][3]);
							glVertex3f(PointB._X * XRatio, PointB._Y * YRatio, PointA._Z);
						}
						else
						{
							glTexCoord2fv(Orient[PanelOrientation[0]][0]);
							glVertex3f(PointB._X * XRatio, PointA._Y * YRatio, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][1]);
							glVertex3f(PointA._X * XRatio, PointA._Y * YRatio, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][2]);
							glVertex3f(PointB._X * XRatio, PointB._Y * YRatio, PointA._Z);
							glTexCoord2fv(Orient[PanelOrientation[0]][3]);
							glVertex3f(PointA._X * XRatio, PointB._Y * YRatio, PointA._Z);
						}

						glEnd();
						if (!is_panelSingleFace)		//Change texture to the back panel
						{
							if (is_textured)
							{
								glBindTexture(GL_TEXTURE_2D, Texture[1]);
							}
							glBegin(GL_TRIANGLES);
							if (is_flipped)
							{
								glTexCoord2fv(Orient[PanelOrientation[1]][0]);
								glVertex3f(PointA._X * XRatio, PointA._Y * YRatio, PointB._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][1]);
								glVertex3f(PointB._X * XRatio, PointA._Y * YRatio, PointB._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][2]);
								glVertex3f(PointA._X * XRatio, PointB._Y * YRatio, PointB._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][3]);
								glVertex3f(PointB._X * XRatio, PointB._Y * YRatio, PointB._Z);
							}
							else
							{
								glTexCoord2fv(Orient[PanelOrientation[1]][0]);
								glVertex3f(PointB._X * XRatio, PointA._Y * YRatio, PointB._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][1]);
								glVertex3f(PointA._X * XRatio, PointA._Y * YRatio, PointB._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][2]);
								glVertex3f(PointB._X * XRatio, PointB._Y * YRatio, PointB._Z);
								glTexCoord2fv(Orient[PanelOrientation[1]][3]);
								glVertex3f(PointA._X * XRatio, PointB._Y * YRatio, PointB._Z);
							}
							glEnd();
						}
					}
				}
			glPopMatrix();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
}

void ImagePanel::flipPanel(void)
{
	is_flipped = !is_flipped;
}

mwVec2f ImagePanel::getHitPosition(mwVec2f &MousePosition)
{
	if (detectClick(MousePosition))
	{
		if (KeepRatio)
		{
			return -(mwVec2f((PointB._X + Position._X), (PointB._Y + Position._Y)) - MousePosition);
		}
		else
		{
			return -(mwVec2f((PointB._X * XRatio + Position._X), (PointB._Y * YRatio + Position._Y)) - MousePosition);
		}
	}
	else
	{
		return mwVec2f(MWIP_HIT_FAIL, MWIP_HIT_FAIL);
	}
}

mwVec2f ImagePanel::getHitPositionFree(mwVec2f &MousePosition)
{
	if (KeepRatio)
	{
		return -(mwVec2f((PointB._X + Position._X), (PointB._Y + Position._Y)) - MousePosition);
	}
	else
	{
		return -(mwVec2f((PointB._X * XRatio + Position._X), (PointB._Y * YRatio + Position._Y)) - MousePosition);
	}
}

char* ImagePanel::getTextureResolution(uint8 Side)
{
	if (is_panelBuilt)
	{
		switch (Side)
		{
		case MWIP_SIDE_BACK:
			break;
		case MWIP_SIDE_FRONT:
		default:
			Side = MWIP_SIDE_FRONT;
			break;
		}
		if (TextureResolution[Side][0] == '\0')
		{
			sprintf(TextureResolution[Side], "%d x %d", TextureWidth[Side], TextureHeigth[Side]);
		}
		return TextureResolution[Side];
	}
	return NULL;
}

char* ImagePanel::getTextureRatio(uint8 Side)
{
	if (is_panelBuilt)
	{
		switch (Side)	//Defaults to MWIP_SIDE_FRONT if it's invalid
		{
		case MWIP_SIDE_BACK:
			break;
		case MWIP_SIDE_FRONT:
		default:
			Side = MWIP_SIDE_FRONT;
			break;
		}
		if (TextureRatio[Side][0] == '\0')
		{
			int ASY = TextureHeigth[Side];	//Aspect Y
			int ASX = TextureWidth[Side];	//Aspect X
			while (true)
			{
				if ((ASY % 10 == 0) && (ASX % 10 == 0))
				{
					ASY /= 10;
					ASX /= 10;
				}
				else if ((ASY % 2 == 0) && (ASX % 2 == 0))
				{
					ASY /= 2;
					ASX /= 2;
				}
				else if ((ASY % 3 == 0) && (ASX % 3 == 0))
				{
					ASY /= 3;
					ASX /= 3;
				}
				else if ((ASY % 5 == 0) && (ASX % 5 == 0))
				{
					ASY /= 5;
					ASX /= 5;
				}
				else
				{
					break;
				}
			}

			if (YRatio == 1)
			{
				sprintf(TextureRatio[Side], "%.2f (%d:%d)", XRatio, ASX, ASY);
			}
			else if (XRatio == 1)
			{
				sprintf(TextureRatio[Side], "%.2f (%d:%d)", YRatio, ASX, ASY);
			}
		}
		return TextureRatio[Side];
	}
	return NULL;
}

mwVec2f ImagePanel::getUVHitPosition(mwVec2f &MousePosition)
{
	if (detectClick(MousePosition))
	{
		if (KeepRatio)
		{
			mwVec2f Temp = (-(mwVec2f((PointB._X + Position._X), (PointB._Y + Position._Y)) - MousePosition));
			Temp._X = Temp._X / (PointA._X * 2);
			Temp._Y = Temp._Y / (PointA._Y * 2);
			return Temp;
		}
		else
		{
			mwVec2f Temp = (-(mwVec2f((PointB._X * XRatio + Position._X), (PointB._Y * YRatio + Position._Y)) - MousePosition));
			Temp._X = Temp._X / (PointA._X * XRatio * 2);
			Temp._Y = Temp._Y / (PointA._Y * YRatio * 2);
			return Temp;
		}
	}
	else
	{
		return mwVec2f(MWIP_HIT_FAIL, MWIP_HIT_FAIL);
	}
}

mwVec2f ImagePanel::getUVHitPositionFree(mwVec2f &MousePosition)
{
	if (KeepRatio)
	{
		mwVec2f Temp = (-(mwVec2f((PointB._X + Position._X), (PointB._Y + Position._Y)) - MousePosition));
		Temp._X = Temp._X / (PointA._X * 2);
		Temp._Y = Temp._Y / (PointA._Y * 2);
		return Temp;
	}
	else
	{
		mwVec2f Temp = (-(mwVec2f((PointB._X * XRatio + Position._X), (PointB._Y * YRatio + Position._Y)) - MousePosition));
		Temp._X = Temp._X / (PointA._X * XRatio * 2);
		Temp._Y = Temp._Y / (PointA._Y * YRatio * 2);
		return Temp;
	}
}

void ImagePanel::incrementPosition(float X, float Y, float Z)
{
	Position._X += X;
	Position._Y += Y;
	Position._Z += Z;
}

void ImagePanel::loadPanelImage(char *ImagePath)
{
	if (ImagePath[0] != '\0')
	{
		is_textured = true;
		if (!is_panelBuilt)
		{
			TextureInfo TempConverter;
			if (ExtThrdCtrl != NULL)
			{
				TempConverter = MWTHInfoLoadTextureEx(ImagePath, ExtThrdCtrl);
			}
			else
			{
				TempConverter = MWTHInfoLoadTexture(ImagePath);
			}
			copyFileStateData(TempConverter.getFileStateDataPointer());
			Texture[MWIP_SIDE_FRONT] = TempConverter.Texture;
			TextureHeigth[MWIP_SIDE_FRONT] = TempConverter.Heigth;
			TextureWidth[MWIP_SIDE_FRONT] = TempConverter.Width;
			is_textureAlpha = TempConverter.have_alpha;
			if (TextureHeigth[0] != 0 && TextureWidth[0] != 0)	//If diferent than 0, then an image was actually loaded (hooray)
			{
				is_panelBuilt = true;
				computeRatio(MWIP_SIDE_FRONT);
			}
			if (flstState == -2)
			{
				flstStateWr = MWIP_WR_ERROR_INVALID_PATH;
			}
		}
	}
	else
	{
		flstState = MWIP_ERROR_NO_PATH;
		flstStateWr = MWIP_WR_ERROR_NO_PATH;
	}
}

void ImagePanel::loadPanelImage(char *ImagePath, uint8 Side)	//!! WIP !! make an array for dual texture loading
{
	if (ImagePath[0] != '\0')
	{
		is_textured = true;
		if (!is_panelBuilt)
		{
			TextureInfo TempConverter;
			switch (Side)
			{
			case MWIP_SIDE_BACK:
				if (ExtThrdCtrl != NULL)
				{
					TempConverter = MWTHInfoLoadTextureEx(ImagePath, ExtThrdCtrl);
				}
				else
				{
					TempConverter = MWTHInfoLoadTexture(ImagePath);
				}
				Texture[MWIP_SIDE_BACK] = TempConverter.Texture;
				TextureHeigth[Side] = TempConverter.Heigth;
				TextureWidth[Side] = TempConverter.Width;
				break;
			case MWIP_SIDE_FRONT:
			default:
				if (ExtThrdCtrl != NULL)
				{
					TempConverter = MWTHInfoLoadTextureEx(ImagePath, ExtThrdCtrl);
				}
				else
				{
					TempConverter = MWTHInfoLoadTexture(ImagePath);
				}
				Texture[MWIP_SIDE_FRONT] = TempConverter.Texture;
				TextureHeigth[Side] = TempConverter.Heigth;
				TextureWidth[Side] = TempConverter.Width;
				break;
			}
			copyFileStateData(TempConverter.getFileStateDataPointer());
			if (TextureHeigth != 0 && TextureWidth != 0)
			{
				is_panelBuilt = true;
				computeRatio(Side);
			}
			if (flstState == -2)
			{
				flstStateWr = MWIP_WR_ERROR_INVALID_PATH;
			}
		}
	}
	else
	{
		flstState = MWIP_ERROR_NO_PATH;
		flstStateWr = MWIP_WR_ERROR_NO_PATH;
	}
}

void ImagePanel::loadPanelImage(char *ImagePath1, char *ImagePath2)
{
	if (ImagePath1[0] != '\0' && ImagePath2[0] != '\0')
	{
		is_textured = true;
		if (!is_panelBuilt)
		{
			//TODO - Update for the new standard (see other 'load')
			Texture[MWIP_SIDE_FRONT] = MWTHLoadTexture(ImagePath1);
			Texture[MWIP_SIDE_BACK] = MWTHLoadTexture(ImagePath2);
			computeRatio(MWIP_SIDE_BACK);
			computeRatio(MWIP_SIDE_FRONT);
			if (flstState == -2)
			{
				flstStateWr = MWIP_WR_ERROR_INVALID_PATH;
			}
		}
	}
	else
	{
		flstState = MWIP_ERROR_NO_PATH;
		flstStateWr = MWIP_WR_ERROR_NO_PATH;
	}
}

void ImagePanel::preSetPanel(uint8 Dimension, uint8 Faces, uint8 NormalResolution, uint8 Behavior)
{
	setNormal(NormalResolution);
	setDimension(Dimension);
	setVisibleFaces(Faces);
	setRatioBehavior(Behavior);
}

void ImagePanel::purgeNormalsData(void)
{
	delete[] PanelNormals;
	PanelNormals = NULL;
}

void ImagePanel::setDimension(uint8 Dimension)
{
	if (Dimension == MWIP_2D)
	{
		is_panel2D = true;
	}
	else if (Dimension == MWIP_3D)
	{
		is_panel2D = false;
	}
}

void ImagePanel::setNormal(uint8 Resolution)
{
	switch (Resolution)
	{
	case MWIP_NORMALS_ON:
		have_normals = true;
		NormalAmount = 1;
		break;
	case MWIP_NORMALS_OFF:
	default:
		have_normals = false;
		NormalAmount = 0;
		purgeNormalsData();
		break;
	}
	if (!is_panelSingleFace)		//If false, then the panel have 2 visible sides, thus, we will need to double the normal amount
	{
		NormalAmount *= 2;
	}
	allocNormals();
}

void ImagePanel::setOrientation(int8 Orientation1, int8 Orientation2)
{
	if (Orientation1 > MWIP_ORIENTATION_IGNORE && Orientation1 <= MWIP_ORIENTATION_LEFT)
	{
		PanelOrientation[0] = Orientation1;
	}
	if (Orientation2 > MWIP_ORIENTATION_IGNORE && Orientation2 <= MWIP_ORIENTATION_LEFT)
	{
		PanelOrientation[1] = Orientation2;
	}
}

void ImagePanel::setOrientToCustom(void)
{
	//DEFAULTOrient[4][4][2] ->
	/*{ { { 0, 1 }, { 1, 1 }, { 0, 0 }, { 1, 0 } },
	{ { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
	{ { 1, 0 }, { 0, 0 }, { 1, 1 }, { 0, 1 } },
	{ { 1, 1 }, { 1, 0 }, { 0, 1 }, { 0, 0 } }, };*/

	//Orient data: [x][y][z] where: X is the orientation, Y is wich point of the triangle strip (4 points), and Z is the X and Y UV values
	//Since UVA is the min and UVB is the max, from the DEFAULTOrient, we can assure that where is 0, is a UVA point and 1 a UVB, with X and Y corresponding

	Orient[MWIP_ORIENTATION_UP][0][0] = UVA._X;
	Orient[MWIP_ORIENTATION_UP][0][1] = UVB._Y;
	Orient[MWIP_ORIENTATION_UP][1][0] = UVB._X;
	Orient[MWIP_ORIENTATION_UP][1][1] = UVB._Y;
	Orient[MWIP_ORIENTATION_UP][2][0] = UVA._X;
	Orient[MWIP_ORIENTATION_UP][2][1] = UVA._Y;
	Orient[MWIP_ORIENTATION_UP][3][0] = UVB._X;
	Orient[MWIP_ORIENTATION_UP][3][1] = UVA._Y;

	Orient[MWIP_ORIENTATION_RIGHT][0][0] = UVA._X;
	Orient[MWIP_ORIENTATION_RIGHT][0][1] = UVA._Y;
	Orient[MWIP_ORIENTATION_RIGHT][1][0] = UVA._X;
	Orient[MWIP_ORIENTATION_RIGHT][1][1] = UVB._Y;
	Orient[MWIP_ORIENTATION_RIGHT][2][0] = UVB._X;
	Orient[MWIP_ORIENTATION_RIGHT][2][1] = UVA._Y;
	Orient[MWIP_ORIENTATION_RIGHT][3][0] = UVB._X;
	Orient[MWIP_ORIENTATION_RIGHT][3][1] = UVB._Y;

	//DEFAULTOrient[4][4][2] ->
	/*{ { { 0, 1 }, { 1, 1 }, { 0, 0 }, { 1, 0 } },
	{ { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
	{ { 1, 0 }, { 0, 0 }, { 1, 1 }, { 0, 1 } },
	{ { 1, 1 }, { 1, 0 }, { 0, 1 }, { 0, 0 } }, };*/

	Orient[MWIP_ORIENTATION_DOWN][0][0] = UVB._X;
	Orient[MWIP_ORIENTATION_DOWN][0][1] = UVA._Y;
	Orient[MWIP_ORIENTATION_DOWN][1][0] = UVA._X;
	Orient[MWIP_ORIENTATION_DOWN][1][1] = UVA._Y;
	Orient[MWIP_ORIENTATION_DOWN][2][0] = UVB._X;
	Orient[MWIP_ORIENTATION_DOWN][2][1] = UVB._Y;
	Orient[MWIP_ORIENTATION_DOWN][3][0] = UVA._X;
	Orient[MWIP_ORIENTATION_DOWN][3][1] = UVB._Y;

	Orient[MWIP_ORIENTATION_LEFT][0][0] = UVB._X;
	Orient[MWIP_ORIENTATION_LEFT][0][1] = UVB._Y;
	Orient[MWIP_ORIENTATION_LEFT][1][0] = UVB._X;
	Orient[MWIP_ORIENTATION_LEFT][1][1] = UVA._Y;
	Orient[MWIP_ORIENTATION_LEFT][2][0] = UVA._X;
	Orient[MWIP_ORIENTATION_LEFT][2][1] = UVB._Y;
	Orient[MWIP_ORIENTATION_LEFT][3][0] = UVA._X;
	Orient[MWIP_ORIENTATION_LEFT][3][1] = UVA._Y;
}

void ImagePanel::setOrientToDefault(void)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				Orient[i][j][k] = DEFAULTOrient[i][j][k];
			}
		}
	}

	//DEFAULTOrient[4][4][2] ->
	/*{ { { 0, 1 }, { 1, 1 }, { 0, 0 }, { 1, 0 } },
	{ { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
	{ { 1, 0 }, { 0, 0 }, { 1, 1 }, { 0, 1 } },
	{ { 1, 1 }, { 1, 0 }, { 0, 1 }, { 0, 0 } }, };*/
}

void ImagePanel::setPanelBackColor(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha)
{
	PanelBackColor[0] = Red;
	PanelBackColor[1] = Green;
	PanelBackColor[2] = Blue;
	PanelBackColor[3] = Alpha;
}

void ImagePanel::setPanelBackColor(uint8 ColorArray[4])
{
	PanelBackColor[0] = ColorArray[0];
	PanelBackColor[1] = ColorArray[1];
	PanelBackColor[2] = ColorArray[2];
	PanelBackColor[3] = ColorArray[3];
}

void ImagePanel::setPosition(mwVec3f &NewPos)
{
	Position = NewPos;
}

void ImagePanel::setPosition(float X, float Y, float Z)
{
	Position._X = X;
	Position._Y = Y;
	Position._Z = Z;
}

void ImagePanel::setRatioAddaptMode(uint8 Mode)
{
	switch (Mode)
	{
	case MWIP_ADDAPT_RATIO_UV:
		AddaptRatioUVMode = true;
		break;
	case MWIP_ADDAPT_RATIO_NORMAL:
	default:
		AddaptRatioUVMode = false;
		break;
	}
}

void ImagePanel::setRatioBehavior(uint8 Behavior)
{
	switch (Behavior)
	{
	case MWIP_BEHAVE_ADAPT_RATIO:
		KeepRatio = false;
		break;
	case MWIP_BEHAVE_KEEP_RATIO:
	default:
		KeepRatio = true;
		break;
	}
}

void ImagePanel::setRatioMaster(uint8 Side)
{
	switch (Side)
	{
	case MWIP_SIDE_BACK:
		RatioMaster = 1;
		break;
	case MWIP_SIDE_FRONT:
	default:
		RatioMaster = 0;
		break;
	}
}

void ImagePanel::setSize(mwVec3f &Point1, mwVec3f &Point2)
{
	PointA = Point1;
	PointB = Point2;
}

void ImagePanel::setSize(uint16 XScale, uint16 YScale)
{
	XRatio = XScale;
	YRatio = YScale;
}

void ImagePanel::setTexturePointer(uint8 Tex, uint8 Side)
{
	Texture[Side] = Tex;
	is_panelBuilt = true;
}

void ImagePanel::setThreadControllerPointer(ThreadController* TGT)
{
	if (TGT != NULL)
	{
		if (TGT->isControllerReady())
		{
			//Controller found and ready!
			ExtThrdCtrl = TGT;
		}
	}
	else
	{
		ExtThrdCtrl = NULL;
	}
}

void ImagePanel::setUVMode(uint8 UVMode)
{
	switch (UVMode)
	{
	case MWIP_UV_MANUAL:
		is_UVDataCustom = true;
		setOrientToCustom();
		break;
	case MWIP_UV_FULL:
	default:
		is_UVDataCustom = false;
		UVA = 0;
		UVB = 1;
		setOrientToDefault();
		break;
	}
}

void ImagePanel::setUVMap(mwVec2f &UVA, mwVec2f &UVB)
{
	this->UVA = UVA;
	this->UVB = UVB;
}

void ImagePanel::setUVMap(float UVAX, float UVAY, float UVBX, float UVBY)
{
	UVA._X = UVAX;
	UVA._Y = UVAY;
	UVB._X = UVBX;
	UVB._Y = UVBY;
	setOrientToCustom();
}

void ImagePanel::setVisibleFaces(uint8 Faces)
{
	if (Faces >= MWIP_FACE_DOUBLE_SIDE)
	{
		is_panelSingleFace = false;
	}
	else
	{
		is_panelSingleFace = true;
	}
}

void ImagePanel::switchHide(void)
{
	is_hidden = !is_hidden;
}

void ImagePanel::switchRatioAddaptMode(void)
{
	AddaptRatioUVMode = !AddaptRatioUVMode;
}

void ImagePanel::switchRatioBehavior(void)
{
	KeepRatio = !KeepRatio;
}

void ImagePanel::switchRatioMaster(void)
{
	if (RatioMaster == 0)
	{
		RatioMaster = 1;
	}
	else
	{
		RatioMaster = 0;
	}
}

void ImagePanel::switchUVMode(void)
{
	is_UVDataCustom = !is_UVDataCustom;
	if (is_UVDataCustom)
	{
		setOrientToCustom();
	}
	else
	{
		UVA = 0;
		UVB = 1;
		setOrientToDefault();
	}
}

void ImagePanel::reloadPanelImage(char *ImagePath)
{
	unloadPanelImage();
	loadPanelImage(ImagePath);
}

void ImagePanel::reloadPanelImage(char *ImagePath, uint8 Side)
{
	is_panelSingleFace = false;
	unloadPanelImage();
	loadPanelImage(ImagePath, Side);
}

void ImagePanel::reloadPanelImage(char *ImagePath1, char *ImagePath2)
{
	is_panelSingleFace = false;
	unloadPanelImage();
	loadPanelImage(ImagePath1, ImagePath2);
}

void ImagePanel::resetValues(void)
{
	purgeNormalsData();
	PointA = 0;
	PointB = 0;
	Rotation = 0;
	RotationAxis = 0;
	is_panelBuilt = false;
	KeepRatio = true;
	is_panel2D = true;
	is_panelSingleFace = true;
	have_normals = false;
	is_hidden = false;
	is_clickActive = false;
}

void ImagePanel::unloadPanelImage(void)
{
	if (is_panelSingleFace)
	{
		glDeleteTextures(1, Texture);
	}
	else
	{
		glDeleteTextures(2, Texture);
	}
	is_panelBuilt = false;
	TextureHeigth[MWIP_SIDE_FRONT] = 0;
	TextureWidth[MWIP_SIDE_FRONT] = 0;
	TextureHeigth[MWIP_SIDE_BACK] = 0;
	TextureWidth[MWIP_SIDE_BACK] = 0;
	TextureResolution[MWIP_SIDE_FRONT][0] = '\0';
	TextureResolution[MWIP_SIDE_BACK][0] = '\0';
	TextureRatio[MWIP_SIDE_FRONT][0] = '\0';
	TextureRatio[MWIP_SIDE_BACK][0] = '\0';
}