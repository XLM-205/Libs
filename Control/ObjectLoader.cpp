/*-------Moon Wiz Object Loader-------
*	Custom Object loader
*	--.obj and .mtl reading, drawing, transformations, hit-box analyser, independent texture and .mtl override, multi-textured object handling
*		--Require "TextureHandler.h" for texture generation
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
*	Version 0.8.4
*/

#include <GL\freeglut.h>
#include "ObjectLoader.h"
#include "..\Graphics\TextureHandler.h"
#include "..\Control\DataTypes.h"
#include <string.h>
#include <stdlib.h>

#define _CRTDBG_MAP_ALLOC	//For heap corruption
#include <stdlib.h>
#include <crtdbg.h>

#define PATTERN_X			1				//Indices reading, pattern X (Only Vertexes)			
#define PATTERN_X_Y			2				//Indices reading, pattern X/Y (Vertexes and UV)
#define PATTERN_X_Y_Z		3				//Indices reading, pattern X/Y/Z (Vertexes, UV and Normals)
#define PATTERN_X_Z			4				//Indices reading, pattern X//Z (Vertexes and Normals)
#define NORMAL_SCALE_FACTOR 2				//Factor that will be used when drawing normals

static uint8 ReadMode = MWOL_READ_SIMPLE;	//Reading mode used to parse the .obj and .mtl data
static uint8 HighlightRed = 255;			//Default highlight color used in highlight visualization (Wireframe + Solid/Textured). Default color is Magenta (255, 0, 255)
static uint8 HighlightGreen = 0;
static uint8 HighlightBlue = 255;
static uint8 BoundingBoxRed = 0;			//Default highlight color used in BB visualization. Default color is Cyan (0, 255, 255)
static uint8 BoundingBoxGreen = 255;
static uint8 BoundingBoxBlue = 255;
static uint8 NormalsRed = 255;				//Default normal color used in normal visualization. Default color is Yellow (255, 255, 0)
static uint8 NormalsGreen = 255;
static uint8 NormalsBlue = 0;

//Pre-configurations for the engine
void ObjLoaderPreConfig(uint8 ReadingMode)
{
	ReadMode = ReadingMode;
}

//Highlight Color
void ObjLoaderHighLigthColor(uint8 HighlightColorR, uint8 HighlightColorG, uint8 HighlightColorB)
{
	HighlightRed = HighlightColorR;
	HighlightGreen = HighlightColorG;
	HighlightBlue = HighlightColorB;
}

//Normal Color
void ObjLoaderNormalColor(uint8 Red, uint8 Green, uint8 Blue)
{
	NormalsRed = Red;
	NormalsGreen = Green;
	NormalsBlue = Blue;
}

//Bounding Box Color
void ObjLoaderBoundingBoxColor(uint8 BBColorR, uint8 BBColorG, uint8 BBColorB)
{
	BoundingBoxRed = BBColorR;
	BoundingBoxGreen = BBColorG;
	BoundingBoxBlue = BBColorB;
}

//Class methods
ObjLoader::ObjLoader(void)
{
	OverrideTexturePath[0] = '\0';
	//flstStateWr = MWOL_WR_WARN_NOT_LOADED;
	m_state->setString(MWOL_WR_WARN_NOT_LOADED);
	//flstState = MWOL_WARN_NOT_LOADED;
	m_stateCode = FileStatus::FS_NONE;
	//flstStateType = CST_TYPE_WARNING;
}

ObjLoader::ObjLoader(char *Filename)
{
	OverrideTexturePath[0] = '\0';
	resetMaxMin();
	openFile(Filename, "r");
	if (m_stateCode == FileStatus::FS_SUCCESS)
	{
		sprintf(FilePath, "%s", Filename);
		is_fromFile = true;
		setData();
		is_objectBuilt = true;
	}
	else
	{
		is_fromFile = false;
	}
}

ObjLoader::ObjLoader(char *Filename, char *Texture)
{
	OverrideTexturePath[0] = '\0';
	TextureOverride = true;
	resetMaxMin();
	openFile(Filename, "r");
	if (m_stateCode == FileStatus::FS_SUCCESS)
	{
		sprintf(FilePath, "%s", Filename);
		is_fromFile = true;
		setData();
		is_objectBuilt = true;
	}
	else
	{
		is_fromFile = false;
	}
}

ObjLoader::~ObjLoader(void)
{
	unloadObject();
}

bool ObjLoader::isPointInsideBB(mwVec3f Point)
{
	return Point > ObjBBMin && Point < ObjBBMax;
}

//bool ObjLoader::isPointInsideBBGroup(mwVec3f Point, uint16 GroupIndex)	TODO
//{
//
//}

void ObjLoader::DrawLoadedObject(uint8 DrawMode, float Scale)
{
	uint32 Counter;					//Used only to speed up the simple i + k operation
	if (!generated_texture)
	{
		generated_texture = true;
		if (TextureOverride)
		{
			loadTexture(OverrideTexturePath);
			if(Texture[0] != NULL)	//If not NULL, we successfully loaded a texture
			{
				is_textured = true;
			}
		}
		else
		{
			loadTexture();
		}
	}
	if (!is_centerComputed)
	{
		is_centerComputed = true;
		setCenter(Scale);
	}
	glColor3ub(255, 255, 255);		//If none color found inside the .mtl (Ka / d), default to white
	glPushMatrix();
	glTranslatef(ObjRotationAxis._X, ObjRotationAxis._Y, ObjRotationAxis._Z);			//Set the postion back to where it should be
	glRotatef(ObjRotation._X, 1, 0, 0);
	glRotatef(ObjRotation._Y, 0, 1, 0);
	glRotatef(ObjRotation._Z, 0, 0, 1);
	glTranslatef(-ObjRotationAxis._X, -ObjRotationAxis._Y, -ObjRotationAxis._Z);		//Set the postion to 0 so the rotation is correct
	switch (DrawMode)
	{
	case MWOL_DRAW_WIREFRAME:
		for (int j = 0; j < GroupAmount; j++)
		{
			glPushMatrix();
			glTranslatef(Groups[j].GrpRotationAxis._X, Groups[j].GrpRotationAxis._Y, Groups[j].GrpRotationAxis._Z);
			glRotatef(Groups[j].GrpRotation._X, 1, 0, 0);
			glRotatef(Groups[j].GrpRotation._Y, 0, 1, 0);
			glRotatef(Groups[j].GrpRotation._Z, 0, 0, 1);
			glTranslatef(-Groups[j].GrpRotationAxis._X, -Groups[j].GrpRotationAxis._Y, -Groups[j].GrpRotationAxis._Z);
			for (int i = Groups[j].RangeStart; i < Groups[j].RangeEnd; i += FaceSize)
			{
				glBegin(GL_LINE_LOOP);
				for (int k = 0; k < FaceSize; k++)
				{
					Counter = i + k;
					if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
					{
						glNormal3f(NormalsData[NormalsIndices[Counter]]._X, NormalsData[NormalsIndices[Counter]]._Y, NormalsData[NormalsIndices[Counter]]._Z);
					}
					glVertex3f(VertexData[VertexIndices[Counter]]._X * Scale, VertexData[VertexIndices[Counter]]._Y * Scale, VertexData[VertexIndices[Counter]]._Z * Scale);
				}
				glEnd();
			}
			glPopMatrix();
		}
		break;
	case MWOL_DRAW_SOLID:
		for (int j = 0; j < GroupAmount; j++)
		{
			glPushMatrix();
			glTranslatef(Groups[j].GrpRotationAxis._X, Groups[j].GrpRotationAxis._Y, Groups[j].GrpRotationAxis._Z);
			glRotatef(Groups[j].GrpRotation._X, 1, 0, 0);
			glRotatef(Groups[j].GrpRotation._Y, 0, 1, 0);
			glRotatef(Groups[j].GrpRotation._Z, 0, 0, 1);
			glTranslatef(-Groups[j].GrpRotationAxis._X, -Groups[j].GrpRotationAxis._Y, -Groups[j].GrpRotationAxis._Z);
			glBegin(GL_TRIANGLES);
			for (int i = Groups[j].RangeStart; i < Groups[j].RangeEnd; i++)
			{
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
				{
					glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
				}
				glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
			}
			glEnd();
			glPopMatrix();
		}
		break;
	case MWOL_DRAW_TEXTURED:
		for (int j = 0; j < GroupAmount; j++)
		{
			if (is_textured && Groups[j].MaterialID != 0)
			{
				glBindTexture(GL_TEXTURE_2D, Texture[Materials[Groups[j].MaterialID - 1].TextureIndex[0]]);
				if ((LastLoadedMaterial != Materials[Groups[j].MaterialID - 1].ID) && (!TextureOverride))
				{
					LastLoadedMaterial = Materials[Groups[j].MaterialID - 1].ID;
					glMaterialfv(GL_FRONT, GL_AMBIENT, Materials[Groups[j].MaterialID - 1].AmbientMaterial);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, Materials[Groups[j].MaterialID - 1].DiffuseMaterial);
					glMaterialfv(GL_FRONT, GL_SPECULAR, Materials[Groups[j].MaterialID - 1].SpecularMaterial);
					glMaterialf(GL_FRONT, GL_SHININESS, Materials[Groups[j].MaterialID - 1].Ns);
				}
			}
			glPushMatrix();
			glTranslatef(Groups[j].GrpRotationAxis._X, Groups[j].GrpRotationAxis._Y, Groups[j].GrpRotationAxis._Z);
			glRotatef(Groups[j].GrpRotation._X, 1, 0, 0);
			glRotatef(Groups[j].GrpRotation._Y, 0, 1, 0);
			glRotatef(Groups[j].GrpRotation._Z, 0, 0, 1);
			glTranslatef(-Groups[j].GrpRotationAxis._X, -Groups[j].GrpRotationAxis._Y, -Groups[j].GrpRotationAxis._Z);
			glBegin(GL_TRIANGLES);
			for (int i = Groups[j].RangeStart; i < Groups[j].RangeEnd; i++)
			{
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
				{
					glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
				}
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Y) && is_textured)			//Drawing a textured object. Parsing the UV Coordinates
				{
					glTexCoord3f(UVData[UVIndices[i]]._X, UVData[UVIndices[i]]._Y, UVData[UVIndices[i]]._Z);
				}
				glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
			}
			glEnd();
			glPopMatrix();
		}
		glBindTexture(GL_TEXTURE_2D, NULL);		//The object texture was used, set it back to prevent applying it to other geometry
		break;
	case MWOL_DRAW_SOLID_HIGHLIGHT:
		for (int j = 0; j < GroupAmount; j++)
		{
			glPushMatrix();
			glTranslatef(Groups[j].GrpRotationAxis._X, Groups[j].GrpRotationAxis._Y, Groups[j].GrpRotationAxis._Z);
			glRotatef(Groups[j].GrpRotation._X, 1, 0, 0);
			glRotatef(Groups[j].GrpRotation._Y, 0, 1, 0);
			glRotatef(Groups[j].GrpRotation._Z, 0, 0, 1);
			glTranslatef(-Groups[j].GrpRotationAxis._X, -Groups[j].GrpRotationAxis._Y, -Groups[j].GrpRotationAxis._Z);
			glBegin(GL_TRIANGLES);
			for (int i = Groups[j].RangeStart; i < Groups[j].RangeEnd; i++)
			{
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
				{
					glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
				}
				glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
			}
			glEnd();
			glPopMatrix();
		}
		glLineWidth(2);
		glColor3ub(HighlightRed, HighlightGreen, HighlightBlue);
		for (int j = 0; j < GroupAmount; j++)
		{
			glPushMatrix();
			glTranslatef(Groups[j].GrpRotationAxis._X, Groups[j].GrpRotationAxis._Y, Groups[j].GrpRotationAxis._Z);
			glRotatef(Groups[j].GrpRotation._X, 1, 0, 0);
			glRotatef(Groups[j].GrpRotation._Y, 0, 1, 0);
			glRotatef(Groups[j].GrpRotation._Z, 0, 0, 1);
			glTranslatef(-Groups[j].GrpRotationAxis._X, -Groups[j].GrpRotationAxis._Y, -Groups[j].GrpRotationAxis._Z);
			for (int i = Groups[j].RangeStart; i < Groups[j].RangeEnd; i += FaceSize)
			{
				glBegin(GL_LINE_LOOP);
				for (int k = 0; k < FaceSize; k++)
				{
					Counter = i + k;
					if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
					{
						glNormal3f(NormalsData[NormalsIndices[Counter]]._X, NormalsData[NormalsIndices[Counter]]._Y, NormalsData[NormalsIndices[Counter]]._Z);
					}
					glVertex3f(VertexData[VertexIndices[Counter]]._X * Scale, VertexData[VertexIndices[Counter]]._Y * Scale, VertexData[VertexIndices[Counter]]._Z * Scale);
				}
				glEnd();
			}
			glPopMatrix();
		}
		break;
	case MWOL_DRAW_TEXTURED_HIGHLIGHT:
		//TODO: Put material tranformation here (glMaterial)
		for (int j = 0; j < GroupAmount; j++)
		{
			if(is_textured && Groups[j].MaterialID != 0)
			{
				glBindTexture(GL_TEXTURE_2D, Texture[Materials[Groups[j].MaterialID - 1].TextureIndex[0]]);
				if((LastLoadedMaterial != Materials[Groups[j].MaterialID - 1].ID) && (!TextureOverride))
				{
					LastLoadedMaterial = Materials[Groups[j].MaterialID - 1].ID;
					glMaterialfv(GL_FRONT, GL_AMBIENT, Materials[Groups[j].MaterialID - 1].AmbientMaterial);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, Materials[Groups[j].MaterialID - 1].DiffuseMaterial);
					glMaterialfv(GL_FRONT, GL_SPECULAR, Materials[Groups[j].MaterialID - 1].SpecularMaterial);
					glMaterialf(GL_FRONT, GL_SHININESS, Materials[Groups[j].MaterialID - 1].Ns);
				}
			}
			glPushMatrix();
			glTranslatef(Groups[j].GrpRotationAxis._X, Groups[j].GrpRotationAxis._Y, Groups[j].GrpRotationAxis._Z);
			glRotatef(Groups[j].GrpRotation._X, 1, 0, 0);
			glRotatef(Groups[j].GrpRotation._Y, 0, 1, 0);
			glRotatef(Groups[j].GrpRotation._Z, 0, 0, 1);
			glTranslatef(-Groups[j].GrpRotationAxis._X, -Groups[j].GrpRotationAxis._Y, -Groups[j].GrpRotationAxis._Z);
			glBegin(GL_TRIANGLES);
			for (int i = Groups[j].RangeStart; i < Groups[j].RangeEnd; i++)
			{
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
				{
					glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
				}
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Y) && is_textured)			//Drawing a textured object. Parsing the UV Coordinates
				{
					glTexCoord3f(UVData[UVIndices[i]]._X, UVData[UVIndices[i]]._Y, UVData[UVIndices[i]]._Z);
				}
				glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
			}
			glEnd();
			glPopMatrix();
		}
		glBindTexture(GL_TEXTURE_2D, NULL);		//The object texture was used, set it back to prevent applying it to other geometry
		glLineWidth(2);
		glColor3ub(HighlightRed, HighlightGreen, HighlightBlue);
		for (int j = 0; j < GroupAmount; j++)
		{
			glPushMatrix();
			glTranslatef(Groups[j].GrpRotationAxis._X, Groups[j].GrpRotationAxis._Y, Groups[j].GrpRotationAxis._Z);
			glRotatef(Groups[j].GrpRotation._X, 1, 0, 0);
			glRotatef(Groups[j].GrpRotation._Y, 0, 1, 0);
			glRotatef(Groups[j].GrpRotation._Z, 0, 0, 1);
			glTranslatef(-Groups[j].GrpRotationAxis._X, -Groups[j].GrpRotationAxis._Y, -Groups[j].GrpRotationAxis._Z);
			for (int i = Groups[j].RangeStart; i < Groups[j].RangeEnd; i += FaceSize)
			{
				glBegin(GL_LINE_LOOP);
				for (int k = 0; k < FaceSize; k++)
				{
					Counter = i + k;
					if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
					{
						glNormal3f(NormalsData[NormalsIndices[Counter]]._X, NormalsData[NormalsIndices[Counter]]._Y, NormalsData[NormalsIndices[Counter]]._Z);
					}
					glVertex3f(VertexData[VertexIndices[Counter]]._X * Scale, VertexData[VertexIndices[Counter]]._Y * Scale, VertexData[VertexIndices[Counter]]._Z * Scale);
				}
				glEnd();
			}
			glPopMatrix();
		}
		break;
	case MWOL_DRAW_SOLID_QUAD:
		for (int j = 0; j < GroupAmount; j++)
		{
			glPushMatrix();
			glTranslatef(Groups[j].GrpRotationAxis._X, Groups[j].GrpRotationAxis._Y, Groups[j].GrpRotationAxis._Z);
			glRotatef(Groups[j].GrpRotation._X, 1, 0, 0);
			glRotatef(Groups[j].GrpRotation._Y, 0, 1, 0);
			glRotatef(Groups[j].GrpRotation._Z, 0, 0, 1);
			glTranslatef(-Groups[j].GrpRotationAxis._X, -Groups[j].GrpRotationAxis._Y, -Groups[j].GrpRotationAxis._Z);
			glBegin(GL_QUADS);
			for (int i = Groups[j].RangeStart; i < Groups[j].RangeEnd; i++)
			{
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
				{
					glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
				}
				glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
			}
			glEnd();
			glPopMatrix();
		}
		break;
	case MWOL_DRAW_TEXTURED_QUAD:
		for (int j = 0; j < GroupAmount; j++)
		{
			if(is_textured && Groups[j].MaterialID != 0)
			{
				glBindTexture(GL_TEXTURE_2D, Texture[Materials[Groups[j].MaterialID - 1].TextureIndex[0]]);
				if((LastLoadedMaterial != Materials[Groups[j].MaterialID - 1].ID) && (!TextureOverride))
				{
					LastLoadedMaterial = Materials[Groups[j].MaterialID - 1].ID;
					glMaterialfv(GL_FRONT, GL_AMBIENT, Materials[Groups[j].MaterialID - 1].AmbientMaterial);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, Materials[Groups[j].MaterialID - 1].DiffuseMaterial);
					glMaterialfv(GL_FRONT, GL_SPECULAR, Materials[Groups[j].MaterialID - 1].SpecularMaterial);
					glMaterialf(GL_FRONT, GL_SHININESS, Materials[Groups[j].MaterialID - 1].Ns);
				}
			}
			glPushMatrix();
			glTranslatef(Groups[j].GrpRotationAxis._X, Groups[j].GrpRotationAxis._Y, Groups[j].GrpRotationAxis._Z);
			glRotatef(Groups[j].GrpRotation._X, 1, 0, 0);
			glRotatef(Groups[j].GrpRotation._Y, 0, 1, 0);
			glRotatef(Groups[j].GrpRotation._Z, 0, 0, 1);
			glTranslatef(-Groups[j].GrpRotationAxis._X, -Groups[j].GrpRotationAxis._Y, -Groups[j].GrpRotationAxis._Z);
			glBegin(GL_QUADS);
			for (int i = Groups[j].RangeStart; i < Groups[j].RangeEnd; i++)
			{
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
				{
					glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
				}
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Y) && is_textured)			//Drawing a textured object. Parsing the UV Coordinates
				{
					glTexCoord3f(UVData[UVIndices[i]]._X, UVData[UVIndices[i]]._Y, UVData[UVIndices[i]]._Z);
				}
				glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
			}
			glEnd();
			glPopMatrix();
		}
		glBindTexture(GL_TEXTURE_2D, NULL);		//The object texture was used, set it back to prevent applying it to other geometry
		break;
	}
	glPopMatrix();
}

void ObjLoader::DrawLoadedObject(uint8 DrawMode, float Scale, uint16 GroupIndex)
{
	uint32 Counter;					//Used only to speed up the simple i + k operation
	if (!generated_texture)
	{
		generated_texture = true;
		if (TextureOverride)
		{
			loadTexture(OverrideTexturePath);
		}
		else
		{
			loadTexture();
		}
	}
	if (!is_centerComputed)
	{
		is_centerComputed = true;
		setCenter(Scale);
	}
	glColor3ub(255, 255, 255);		//If none color found inside the .mtl (Ka / d), default to white
	glPushMatrix();
	glTranslatef(ObjRotationAxis._X, ObjRotationAxis._Y, ObjRotationAxis._Z);			//Set the postion back tha where it should be
	glRotatef(ObjRotation._X, 1, 0, 0);
	glRotatef(ObjRotation._Y, 0, 1, 0);
	glRotatef(ObjRotation._Z, 0, 0, 1);
	glTranslatef(-ObjRotationAxis._X, -ObjRotationAxis._Y, -ObjRotationAxis._Z);		//Set the postion to 0 so the rotation is correct
	glPushMatrix();															//And now, rotate the group
	glTranslatef(Groups[GroupIndex].GrpPosition._X, Groups[GroupIndex].GrpPosition._Y, Groups[GroupIndex].GrpPosition._Z);
	glRotatef(Groups[GroupIndex].GrpRotation._X, 1, 0, 0);
	glRotatef(Groups[GroupIndex].GrpRotation._Y, 0, 1, 0);
	glRotatef(Groups[GroupIndex].GrpRotation._Z, 0, 0, 1);
	glTranslatef(-Groups[GroupIndex].GrpPosition._X, -Groups[GroupIndex].GrpPosition._Y, -Groups[GroupIndex].GrpPosition._Z);		//Set the postion to 0 so the rotation is correct
	switch (DrawMode)
	{
	case MWOL_DRAW_WIREFRAME:
		for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i += FaceSize)
		{
			glBegin(GL_LINE_LOOP);
			for (int j = 0; j < FaceSize; j++)
			{
				Counter = i + j;
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
				{
					glNormal3f(NormalsData[NormalsIndices[Counter]]._X, NormalsData[NormalsIndices[Counter]]._Y, NormalsData[NormalsIndices[Counter]]._Z);
				}
				glVertex3f(VertexData[VertexIndices[Counter]]._X * Scale, VertexData[VertexIndices[Counter]]._Y * Scale, VertexData[VertexIndices[Counter]]._Z * Scale);
			}
			glEnd();
		}
		break;
	case MWOL_DRAW_SOLID:
		glBegin(GL_TRIANGLES);
		for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i++)
		{
			if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
			{
				glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
			}
			glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
		}
		glEnd();
		break;
	case MWOL_DRAW_TEXTURED:
		if (is_textured && Groups[GroupIndex].MaterialID != 0)
		{
			glBindTexture(GL_TEXTURE_2D, Texture[Materials[Groups[GroupIndex].MaterialID - 1].TextureIndex[0]]);
			if ((LastLoadedMaterial != Materials[Groups[GroupIndex].MaterialID - 1].ID) && (!TextureOverride))
			{
				LastLoadedMaterial = Materials[Groups[GroupIndex].MaterialID - 1].ID;
				glMaterialfv(GL_FRONT, GL_AMBIENT, Materials[Groups[GroupIndex].MaterialID - 1].AmbientMaterial);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, Materials[Groups[GroupIndex].MaterialID - 1].DiffuseMaterial);
				glMaterialfv(GL_FRONT, GL_SPECULAR, Materials[Groups[GroupIndex].MaterialID - 1].SpecularMaterial);
				glMaterialf(GL_FRONT, GL_SHININESS, Materials[Groups[GroupIndex].MaterialID - 1].Ns);
			}
		}
		glBegin(GL_TRIANGLES);
		for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i++)
		{
			if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
			{
				glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
			}
			if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Y) && is_textured)			//Drawing a textured object. Parsing the UV Coordinates
			{
				glTexCoord3f(UVData[UVIndices[i]]._X, UVData[UVIndices[i]]._Y, UVData[UVIndices[i]]._Z);
			}
			glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, NULL);		//The object texture was used, set it back to prevent applying it to other geometry
		break;
	case MWOL_DRAW_SOLID_HIGHLIGHT:
		glBegin(GL_TRIANGLES);
		for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i++)
		{
			if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
			{
				glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
			}
			glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
		}
		glEnd();
		glLineWidth(2);
		glColor3ub(HighlightRed, HighlightGreen, HighlightBlue);
		for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i += FaceSize)
		{
			glBegin(GL_LINE_LOOP);
			for (int j = 0; j < FaceSize; j++)
			{
				Counter = i + j;
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
				{
					glNormal3f(NormalsData[NormalsIndices[Counter]]._X, NormalsData[NormalsIndices[Counter]]._Y, NormalsData[NormalsIndices[Counter]]._Z);
				}
				glVertex3f(VertexData[VertexIndices[Counter]]._X * Scale, VertexData[VertexIndices[Counter]]._Y * Scale, VertexData[VertexIndices[Counter]]._Z * Scale);
			}
			glEnd();
		}
		break;
	case MWOL_DRAW_TEXTURED_HIGHLIGHT:
		//TODO: Put material tranformation here (glMaterial)
		if (is_textured && Groups[GroupIndex].MaterialID != 0)
		{
			glBindTexture(GL_TEXTURE_2D, Texture[Materials[Groups[GroupIndex].MaterialID - 1].TextureIndex[0]]);
		}
		glBegin(GL_TRIANGLES);
		for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i++)
		{
			if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
			{
				glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
			}
			if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Y) && is_textured)			//Drawing a textured object. Parsing the UV Coordinates
			{
				glTexCoord3f(UVData[UVIndices[i]]._X, UVData[UVIndices[i]]._Y, UVData[UVIndices[i]]._Z);
			}
			glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, NULL);
		glLineWidth(2);
		glColor3ub(HighlightRed, HighlightGreen, HighlightBlue);
		for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i += FaceSize)
		{
			glBegin(GL_LINE_LOOP);
			for (int j = 0; j < FaceSize; j++)
			{
				Counter = i + j;
				if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
				{
					glNormal3f(NormalsData[NormalsIndices[Counter]]._X, NormalsData[NormalsIndices[Counter]]._Y, NormalsData[NormalsIndices[Counter]]._Z);
				}
				glVertex3f(VertexData[VertexIndices[Counter]]._X * Scale, VertexData[VertexIndices[Counter]]._Y * Scale, VertexData[VertexIndices[Counter]]._Z * Scale);
			}
			glEnd();
		}
		break;
	case MWOL_DRAW_SOLID_QUAD:
		glBegin(GL_QUADS);
		for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i++)
		{
			if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
			{
				glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
			}
			glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
		}
		glEnd();
		break;
	case MWOL_DRAW_TEXTURED_QUAD:
		if (is_textured && Groups[GroupIndex].MaterialID != 0)
		{
			glBindTexture(GL_TEXTURE_2D, Texture[Materials[Groups[GroupIndex].MaterialID - 1].TextureIndex[0]]);
		}
		glBegin(GL_QUADS);
		for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i++)
		{
			if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Z) && have_normals)
			{
				glNormal3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
			}
			if ((Pattern == PATTERN_X_Y_Z || Pattern == PATTERN_X_Y) && is_textured)			//Drawing a textured object. Parsing the UV Coordinates
			{
				glTexCoord3f(UVData[UVIndices[i]]._X, UVData[UVIndices[i]]._Y, UVData[UVIndices[i]]._Z);
			}
			glVertex3f(VertexData[VertexIndices[i]]._X * Scale, VertexData[VertexIndices[i]]._Y * Scale, VertexData[VertexIndices[i]]._Z * Scale);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, NULL);		//The object texture was used, set it back to prevent applying it to other geometry
		break;
	}
	glPopMatrix();
	glPopMatrix();
}

void ObjLoader::DrawLoadedBoundingBox(void)
{
	glColor3ub(BoundingBoxRed, BoundingBoxGreen, BoundingBoxBlue);
	glPushMatrix();
	glTranslatef(ObjRotationAxis._X, ObjRotationAxis._Y, ObjRotationAxis._Z);			//Set the postion back to where it should be
	glRotatef(ObjRotation._X, 1, 0, 0);
	glRotatef(ObjRotation._Y, 0, 1, 0);
	glRotatef(ObjRotation._Z, 0, 0, 1);
	//glTranslatef(-ObjRotationAxis._X, -ObjRotationAxis._Y, -ObjRotationAxis._Z);		//Set the postion to 0 so the rotation is correct
	glTranslatef(ObjPosition._X, ObjPosition._Y, ObjPosition._Z);
	glBegin(GL_LINE_LOOP);		//Upper BB face
	glVertex3f(ObjBBMax._X, ObjBBMax._Y, ObjBBMax._Z);
	glVertex3f(ObjBBMax._X, ObjBBMax._Y, ObjBBMin._Z);
	glVertex3f(ObjBBMin._X, ObjBBMax._Y, ObjBBMin._Z);
	glVertex3f(ObjBBMin._X, ObjBBMax._Y, ObjBBMax._Z);
	glEnd();
	glBegin(GL_LINE_LOOP);		//Down BB face
	glVertex3f(ObjBBMax._X, ObjBBMin._Y, ObjBBMax._Z);
	glVertex3f(ObjBBMax._X, ObjBBMin._Y, ObjBBMin._Z);
	glVertex3f(ObjBBMin._X, ObjBBMin._Y, ObjBBMin._Z);
	glVertex3f(ObjBBMin._X, ObjBBMin._Y, ObjBBMax._Z);
	glEnd();
	glBegin(GL_LINES);			//Structural vertexes
	glVertex3f(ObjBBMax._X, ObjBBMin._Y, ObjBBMax._Z);
	glVertex3f(ObjBBMax._X, ObjBBMax._Y, ObjBBMax._Z);

	glVertex3f(ObjBBMin._X, ObjBBMin._Y, ObjBBMax._Z);
	glVertex3f(ObjBBMin._X, ObjBBMax._Y, ObjBBMax._Z);

	glVertex3f(ObjBBMax._X, ObjBBMin._Y, ObjBBMin._Z);
	glVertex3f(ObjBBMax._X, ObjBBMax._Y, ObjBBMin._Z);

	glVertex3f(ObjBBMin._X, ObjBBMin._Y, ObjBBMin._Z);
	glVertex3f(ObjBBMin._X, ObjBBMax._Y, ObjBBMin._Z);
	glEnd();
	glPopMatrix();
}

void ObjLoader::DrawLoadedBoundingBox(uint16 GroupIndex)
{
	glColor3ub(BoundingBoxRed, BoundingBoxGreen, BoundingBoxBlue);
	glPushMatrix();
	glTranslatef(ObjRotationAxis._X, ObjRotationAxis._Y, ObjRotationAxis._Z);			//Set the postion back tha where it should be
	glRotatef(ObjRotation._X, 1, 0, 0);
	glRotatef(ObjRotation._Y, 0, 1, 0);
	glRotatef(ObjRotation._Z, 0, 0, 1);
	//glTranslatef(-ObjRotationAxis._X, -ObjRotationAxis._Y, -ObjRotationAxis._Z);		//Set the postion to 0 so the rotation is correct
	glTranslatef(ObjPosition._X, ObjPosition._Y, ObjPosition._Z);
	glPushMatrix();
	glTranslatef(Groups[GroupIndex].GrpRotationAxis._X, Groups[GroupIndex].GrpRotationAxis._Y, Groups[GroupIndex].GrpRotationAxis._Z);			//Set the postion back tha where it should be
	glRotatef(Groups[GroupIndex].GrpRotation._X, 1, 0, 0);
	glRotatef(Groups[GroupIndex].GrpRotation._Y, 0, 1, 0);
	glRotatef(Groups[GroupIndex].GrpRotation._Z, 0, 0, 1);
	glTranslatef(-Groups[GroupIndex].GrpRotationAxis._X, -Groups[GroupIndex].GrpRotationAxis._Y, -Groups[GroupIndex].GrpRotationAxis._Z);		//Set the postion to 0 so the rotation is correct
	glBegin(GL_LINE_LOOP);		//Upper BB face
	glVertex3f(Groups[GroupIndex].GrpBBMax._X, Groups[GroupIndex].GrpBBMax._Y, Groups[GroupIndex].GrpBBMax._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMax._X, Groups[GroupIndex].GrpBBMax._Y, Groups[GroupIndex].GrpBBMin._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMin._X, Groups[GroupIndex].GrpBBMax._Y, Groups[GroupIndex].GrpBBMin._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMin._X, Groups[GroupIndex].GrpBBMax._Y, Groups[GroupIndex].GrpBBMax._Z);
	glEnd();
	glBegin(GL_LINE_LOOP);		//Down BB face
	glVertex3f(Groups[GroupIndex].GrpBBMax._X, Groups[GroupIndex].GrpBBMin._Y, Groups[GroupIndex].GrpBBMax._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMax._X, Groups[GroupIndex].GrpBBMin._Y, Groups[GroupIndex].GrpBBMin._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMin._X, Groups[GroupIndex].GrpBBMin._Y, Groups[GroupIndex].GrpBBMin._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMin._X, Groups[GroupIndex].GrpBBMin._Y, Groups[GroupIndex].GrpBBMax._Z);
	glEnd();
	glBegin(GL_LINES);			//Structural vertexes
	glVertex3f(Groups[GroupIndex].GrpBBMax._X, Groups[GroupIndex].GrpBBMin._Y, Groups[GroupIndex].GrpBBMax._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMax._X, Groups[GroupIndex].GrpBBMax._Y, Groups[GroupIndex].GrpBBMax._Z);

	glVertex3f(Groups[GroupIndex].GrpBBMin._X, Groups[GroupIndex].GrpBBMin._Y, Groups[GroupIndex].GrpBBMax._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMin._X, Groups[GroupIndex].GrpBBMax._Y, Groups[GroupIndex].GrpBBMax._Z);

	glVertex3f(Groups[GroupIndex].GrpBBMax._X, Groups[GroupIndex].GrpBBMin._Y, Groups[GroupIndex].GrpBBMin._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMax._X, Groups[GroupIndex].GrpBBMax._Y, Groups[GroupIndex].GrpBBMin._Z);

	glVertex3f(Groups[GroupIndex].GrpBBMin._X, Groups[GroupIndex].GrpBBMin._Y, Groups[GroupIndex].GrpBBMin._Z);
	glVertex3f(Groups[GroupIndex].GrpBBMin._X, Groups[GroupIndex].GrpBBMax._Y, Groups[GroupIndex].GrpBBMin._Z);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void ObjLoader::DrawLoadedNormals(void)
{
	glColor3ub(NormalsRed, NormalsGreen, NormalsBlue);
	glPushMatrix();
	glTranslatef(ObjRotationAxis._X, ObjRotationAxis._Y, ObjRotationAxis._Z);
	glRotatef(ObjRotation._X, 1, 0, 0);
	glRotatef(ObjRotation._Y, 0, 1, 0);
	glRotatef(ObjRotation._Z, 0, 0, 1);
	glTranslatef(-ObjRotationAxis._X, -ObjRotationAxis._Y, -ObjRotationAxis._Z);
	glBegin(GL_LINES);
	for (int i = 0; i < NormalsAmount; i++)
	{
		glVertex3f(NormalsData[i]._X, NormalsData[i]._Y, NormalsData[i]._Z);
		glVertex3f(NormalsData[i]._X * NORMAL_SCALE_FACTOR, NormalsData[i]._Y * NORMAL_SCALE_FACTOR, NormalsData[i]._Z * NORMAL_SCALE_FACTOR);
	}
	glEnd();
	glPopMatrix();
}

void ObjLoader::DrawLoadedNormals(uint16 GroupIndex)
{
	glColor3ub(NormalsRed, NormalsGreen, NormalsBlue);
	glPushMatrix();
	glTranslatef(ObjRotationAxis._X, ObjRotationAxis._Y, ObjRotationAxis._Z);
	glRotatef(ObjRotation._X, 1, 0, 0);
	glRotatef(ObjRotation._Y, 0, 1, 0);
	glRotatef(ObjRotation._Z, 0, 0, 1);
	glTranslatef(-ObjRotationAxis._X, -ObjRotationAxis._Y, -ObjRotationAxis._Z);
	glPushMatrix();
	glTranslatef(Groups[GroupIndex].GrpRotationAxis._X, Groups[GroupIndex].GrpRotationAxis._Y, Groups[GroupIndex].GrpRotationAxis._Z);
	glRotatef(Groups[GroupIndex].GrpRotation._X, 1, 0, 0);
	glRotatef(Groups[GroupIndex].GrpRotation._Y, 0, 1, 0);
	glRotatef(Groups[GroupIndex].GrpRotation._Z, 0, 0, 1);
	glTranslatef(-Groups[GroupIndex].GrpRotationAxis._X, -Groups[GroupIndex].GrpRotationAxis._Y, -Groups[GroupIndex].GrpRotationAxis._Z);
	glBegin(GL_LINES);
	for (int i = Groups[GroupIndex].RangeStart; i < Groups[GroupIndex].RangeEnd; i++)
	{
		glVertex3f(NormalsData[NormalsIndices[i]]._X, NormalsData[NormalsIndices[i]]._Y, NormalsData[NormalsIndices[i]]._Z);
		glVertex3f(NormalsData[NormalsIndices[i]]._X * NORMAL_SCALE_FACTOR, NormalsData[NormalsIndices[i]]._Y * NORMAL_SCALE_FACTOR, NormalsData[NormalsIndices[i]]._Z * NORMAL_SCALE_FACTOR);
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void ObjLoader::setBB(float X, float Y, float Z, ObjGroup *curGroup)
{
	if(X >= ObjBBMax._X)	//Setting the bounding box for the whole object
	{
		ObjBBMax._X = X;
	}
	if(X <= ObjBBMin._X)
	{
		ObjBBMin._X = X;
	}
	if(Y >= ObjBBMax._Y)
	{
		ObjBBMax._Y = Y;
	}
	if(Y <= ObjBBMin._Y)
	{
		ObjBBMin._Y = Y;
	}
	if(Z >= ObjBBMax._Z)
	{
		ObjBBMax._Z = Z;
	}
	if(Z <= ObjBBMin._Z)
	{
		ObjBBMin._Z = Z;
	}
	if(have_groups)
	{
		if(X >= curGroup->GrpBBMax._X)
		{
			curGroup->GrpBBMax._X = X;
		}
		if(X <= curGroup->GrpBBMin._X)
		{
			curGroup->GrpBBMin._X = X;
		}
		if(Y >= curGroup->GrpBBMax._Y)
		{
			curGroup->GrpBBMax._Y = Y;
		}
		if(Y <= curGroup->GrpBBMin._Y)
		{
			curGroup->GrpBBMin._Y = Y;
		}
		if(Z >= curGroup->GrpBBMax._Z)
		{
			curGroup->GrpBBMax._Z = Z;
		}
		if(Z <= curGroup->GrpBBMin._Z)
		{
			curGroup->GrpBBMin._Z = Z;
		}
	}
}

void ObjLoader::parseFaceX(List<uint32> *vertexIndexes, List<uint32> *unused, List<uint32> *unused2)
{
	uint32 vI[3], vIW;
	fscanf(m_fileP, "%d %d %d ", &vI[0], &vI[1], &vI[2]);
	vI[0]--;
	vI[1]--;
	vI[2]--;
	vertexIndexes->AddCopy(vI, 3);
	if(is_quadrilateral)
	{
		fscanf(m_fileP, "%d", &vIW);
		vIW--;
		vertexIndexes->Add(new uint32(vIW));
	}
}

void ObjLoader::parseFaceXY(List<uint32> *vertexIndexes, List<uint32> *uvIndexes, List<uint32> *unused)
{
	uint32 vI[3], vIW, uvI[3], uvIW;
	fscanf(m_fileP, "%d/%d %d/%d %d/%d ", &vI[0], &uvI[0], &vI[1], &uvI[1], &vI[2], &uvI[2]);
	vI[0]--; uvI[0]--;
	vI[1]--; uvI[1]--;
	vI[2]--; uvI[2]--;
	vertexIndexes->AddCopy(vI, 3);
	uvIndexes->AddCopy(vI, 3);
	if(is_quadrilateral)
	{
		fscanf(m_fileP, "%d/%d", &vIW, &uvIW);
		vIW--;	uvIW--;
		vertexIndexes->Add(new uint32(vIW));
		uvIndexes->Add(new uint32(uvIW));
	}
}

void ObjLoader::parseFaceXYZ(List<uint32> *vertexIndexes, List<uint32> *uvIndexes, List<uint32> *normalIndexes)
{
	uint32 vI[3], vIW, uvI[3], uvIW, nI[3], nIW;
	fscanf(m_fileP, "%d/%d/%d %d/%d/%d %d/%d/%d ", &vI[0], &uvI[0], &nI[0], &vI[1], &uvI[1], &nI[1], &vI[2], &uvI[2], &nI[2]);
	vI[0]--; uvI[0]--; nI[0]--;
	vI[1]--; uvI[1]--; nI[1]--;
	vI[2]--; uvI[2]--; nI[2]--;
	vertexIndexes->AddCopy(vI, 3);
	uvIndexes->AddCopy(uvI, 3);
	normalIndexes->AddCopy(nI, 3);
	if(is_quadrilateral)
	{
		fscanf(m_fileP, "%d/%d/%d", &vIW, &uvIW, &nIW);
		vIW--; uvIW--; nIW--;
		vertexIndexes->Add(new uint32(vIW));
		uvIndexes->Add(new uint32(uvIW));
		normalIndexes->Add(new uint32(nIW));
	}
}

void ObjLoader::parseFaceXZ(List<uint32> *vertexIndexes, List<uint32> *normalIndexes, List<uint32> *unused)
{
	uint32 vI[3], vIW, nI[3], nIW;
	fscanf(m_fileP, "%d//%d %d//%d %d//%d ", &vI[0], &nI[0], &vI[1], &nI[1], &vI[2], &nI[2]);
	vI[0]--; nI[0]--;
	vI[1]--; nI[1]--;
	vI[2]--; nI[2]--;
	vertexIndexes->AddCopy(vI, 3);
	normalIndexes->AddCopy(nI, 3);
	if(is_quadrilateral)
	{
		fscanf(m_fileP, "%d//%d", &vIW, &nIW);
		vIW--; nIW--;
		vertexIndexes->Add(new uint32(vIW));
		normalIndexes->Add(new uint32(nIW));
	}
}

void ObjLoader::setData(void)
{
	char TempBuffer[MAX_BUFFER_SIZE];
	char Dump[512];							//For line skiping
	List<uint32> listVertexIndex;
	List<uint32> listNormalIndex;
	List<uint32> listUVIndex;
	List<mwVec3f> listVertexDT;
	List<mwVec3f> listUVDT;
	List<mwVec3f> listNormalDT;
	List<ObjGroup> listGroups;
	void(ObjLoader:: * parseFace)(List<uint32>*, List<uint32>*, List<uint32>*) = nullptr;
	//START - First Pass
	while (!feof(m_fileP))			//First pass. To malloc correctly the arrays
	{
		fscanf(m_fileP, "%s", TempBuffer);
		if (TempBuffer[0] == '#')			//This line is a comment, skip
		{
			fscanf(m_fileP, " %512[^\n]", Dump);
		}
		else if (TempBuffer[0] == 'm' && TempBuffer[1] == 't' && TempBuffer[2] == 'l')		//This line defines a .mtl file
		{
			fscanf(m_fileP, " %512[^\n]", MaterialFilename);
			have_materials = true;
		}
		else if(TempBuffer[0] == 'o' && TempBuffer[1] == '\0')		//This line defines the object name
		{
			fscanf(m_fileP, "%s", ObjectName);
		}
		else if (TempBuffer[0] == 's')		//This line defines the smooth shading. If not present, defaults to off
		{
			fscanf(m_fileP, " %512[^\n]", Dump);
			if (Dump[0] == '1')		//Search for 1. This means the object have smooth shading. If not found, defaults to off
			{
				is_shadingSmooth = true;
			}
		}
		//else if(TempBuffer[0] == 'u')		//This line defines the reference name inside the .mtl file
		else if (TempBuffer[0] == 'g' && TempBuffer[1] == '\0')		//This line defines the group name
		{
			//groups.add()
			//GroupAmount++;
			char buf[64] = { '\0' };
			fscanf(m_fileP, " %64[^\n]", buf);
			listGroups.Add(new ObjGroup(listGroups.Count() + 1, buf));
			if(listGroups.Last()->ID > 1)
			{
				listGroups.RetrieveLast()->getPrevious()->getData()->RangeEnd = listVertexIndex.Count();
			}
			listGroups.Last()->RangeStart = listVertexIndex.Count();
			have_groups = true;
		}
		else if (TempBuffer[0] == 'v')		//This line contains a vertex, a texture coordinate or a vertex normal!
		{
			if (TempBuffer[1] == '\0')		//This line is a vertex
			{
				//VertexAmount++;
				float X, Y, Z;
				fscanf(m_fileP, "%f %f %f\n", &X, &Y, &Z);
				setBB(X, Y, Z, listGroups.Last());
				listVertexDT.Add(new mwVec3f(X, Y, Z));
			}
			else if (TempBuffer[1] == 't' && TempBuffer[2] == '\0')	//This line is a texture coordinate
			{
				//is_textured = true;
				//UVAmount++;
				float UVX, UVY, UVZ = 0;
				char temp = '\0';
				fscanf(m_fileP, "%f %f", &UVX, &UVY);
				fscanf(m_fileP, "%c", &temp);
				if(temp != '\n')	// This texture is three dimensional!
				{
					fseek(m_fileP, -1, SEEK_CUR);
					fscanf(m_fileP, "%f\n", &UVZ);
				}
				listUVDT.Add(new mwVec3f(UVX, UVY, UVZ));
			}
			else if (TempBuffer[1] == 'n' && TempBuffer[2] == '\0')	//This line is a vertex normal
			{
				//have_normals = true;
				//NormalsAmount++;
				float nX, nY, nZ;
				fscanf(m_fileP, "%f %f %f\n", &nX, &nY, &nZ);
				listNormalDT.Add(new mwVec3f(nX, nY, nZ));
			}
		}
		else if (TempBuffer[0] == 'f' && TempBuffer[1] == '\0')		//This line is face data
		{	//By wavefront defaults, [x/y/z] for [Vertex to use/texture coordinate to use/normal to use]. they can appear or not in this format
			//Possible formats: x | x/y | x//z | x/y/z
			if(Pattern)
			{
				(this->*parseFace)(&listVertexIndex, &listUVIndex, &listNormalIndex);
			}
			else if (Pattern == 0)				//If the pattern is not know, find the pattern
			{
				fscanf(m_fileP, " %512[^\n]", TempBuffer);
				uint8 StringSize = patternChecker(TempBuffer);
				fseek(m_fileP, -StringSize - 2, SEEK_CUR);	//Reset the pointer to the start of the line
				//For this first time only!
				//FacesAmount += 3;
				//if (is_quadrilateral)
				//{
				//	FacesAmount++;
				//}
				if(Pattern == PATTERN_X)
				{
					parseFace = &ObjLoader::parseFaceX;
				}
				else if(Pattern == PATTERN_X_Y)
				{
					parseFace = &ObjLoader::parseFaceXY;
				}
				else if(Pattern == PATTERN_X_Y_Z)
				{
					parseFace = &ObjLoader::parseFaceXYZ;
				}
				else if(Pattern == PATTERN_X_Z)
				{
					parseFace = &ObjLoader::parseFaceXZ;
				}
			}
			else
			{
				//We shouldn't fall here...
			}
			//else
			//{
			//	FacesAmount += 3;
			//	if (is_quadrilateral)
			//	{
			//		FacesAmount++;
			//	}
			//}
		}
		else //Unknown line, dump it
		{
			fscanf(m_fileP, " %512[^\n]", Dump);
		}
	}
	have_groups = !listGroups.Empty();
	if (!have_groups)
	{
		GroupAmount = 1;
		Groups = new ObjGroup[1]();
	}
	else
	{
		Groups = listGroups.ToArrayTransfer();
	}

	is_textured = !listUVDT.Empty();
	if(is_textured)
	{
		UVAmount = listUVDT.Count();
		UVIndices = listUVIndex.ToArrayTransfer();
		UVData = listUVDT.ToArrayTransfer();
	}

	have_normals = !listNormalDT.Empty();
	if(have_normals)
	{
		NormalsAmount = listNormalDT.Count();
		NormalsIndices = listNormalIndex.ToArrayTransfer();
		NormalsData = listNormalDT.ToArrayTransfer();
	}
	VertexAmount = listVertexDT.Count();
	FacesAmount = listVertexIndex.Count();
	VertexIndices = listVertexIndex.ToArrayTransfer();
	VertexData = listVertexDT.ToArrayTransfer();
	


	//Groups = new ObjGroup[GroupAmount];
	//VertexIndices = new uint32[FacesAmount]();
	//NormalsIndices = new uint32[FacesAmount]();
	//UVIndices = new uint32[FacesAmount]();
	//VertexData = new mwVec3f[VertexAmount]();
	//UVData = new mwVec2f[UVAmount]();
	//NormalsData = new mwVec3f[NormalsAmount]();
	//GroupAmount = 0;
	//VertexAmount = 0;
	//UVAmount = 0;
	//NormalsAmount = 0;
	//FacesAmount = 0;


	////END - First Pass
	//rewind(m_fileP);							//ALL SET! Reset the pointer and prepare to save the data, FO' REAL!!
	////START - Data gathering
	//while (!feof(m_fileP))
	//{
	//	fscanf(m_fileP, "%s", TempBuffer);
	//	if (TempBuffer[0] == '#')			//This line is a comment, skip
	//	{
	//		fscanf(m_fileP, " %512[^\n]", Dump);
	//	}
	//	else if (TempBuffer[0] == 'o')		//This line is the object name (already read in the first pass, skip it)
	//	{
	//		fscanf(m_fileP, " %512[^\n]", Dump);
	//	}
	//	else if (TempBuffer[0] == 's')		//This line defines the smooth shading (already read in the first pass, skip it)
	//	{
	//		fscanf(m_fileP, " %512[^\n]", Dump);
	//	}
	//	else if (TempBuffer[0] == 'n')		//This line is the material name (already read in the first pass, skip it)
	//	{
	//		fscanf(m_fileP, " %512[^\n]", Dump);
	//	}
	//	else if (TempBuffer[0] == 'u')		//This line defines the reference name inside the .mtl file
	//	{
	//		if (have_groups)
	//		{
	//			fscanf(m_fileP, " %512[^\n]", Groups[GroupAmount - 1].MaterialReference);
	//		}
	//		else
	//		{
	//			fscanf(m_fileP, " %512[^\n]", Groups[0].MaterialReference);
	//		}
	//	}
	//	else if (TempBuffer[0] == 'g' && TempBuffer[1] == '\0')		//This line defines the group name
	//	{
	//		fscanf(m_fileP, " %64[^\n]", Groups[GroupAmount].GroupName);
	//		Groups[GroupAmount].ID = GroupAmount + 1;
	//		if (Groups[GroupAmount].ID > 1)							//To prevent writing values in other memory block than the "Groups" structure
	//		{
	//			Groups[GroupAmount - 1].RangeEnd = FacesAmount;
	//		}
	//		Groups[GroupAmount].RangeStart = FacesAmount;
	//		GroupAmount++;
	//	}
	//	else if (TempBuffer[0] == 'v')		//This line contains a vertex, a texture coordinate or a vertex normal!
	//	{
	//		if (TempBuffer[1] == '\0')		//This line is a vertex
	//		{
	//			fscanf(m_fileP, "%f %f %f\n", &VertexData[VertexAmount]._X, &VertexData[VertexAmount]._Y, &VertexData[VertexAmount]._Z);
	//			GroupAmount--;								//Just for bounding box reading. Must be incremented at the end of this execution
	//			if (VertexData[VertexAmount]._X >= ObjBBMax._X)	//Setting the bounding box for the whole object
	//			{
	//				ObjBBMax._X = VertexData[VertexAmount]._X;
	//			}
	//			if (VertexData[VertexAmount]._X <= ObjBBMin._X)
	//			{
	//				ObjBBMin._X = VertexData[VertexAmount]._X;
	//			}
	//			if (VertexData[VertexAmount]._Y >= ObjBBMax._Y)
	//			{
	//				ObjBBMax._Y = VertexData[VertexAmount]._Y;
	//			}
	//			if (VertexData[VertexAmount]._Y <= ObjBBMin._Y)
	//			{
	//				ObjBBMin._Y = VertexData[VertexAmount]._Y;
	//			}
	//			if (VertexData[VertexAmount]._Z >= ObjBBMax._Z)
	//			{
	//				ObjBBMax._Z = VertexData[VertexAmount]._Z;
	//			}
	//			if (VertexData[VertexAmount]._Z <= ObjBBMin._Z)
	//			{
	//				ObjBBMin._Z = VertexData[VertexAmount]._Z;
	//			}
	//			if (have_groups)
	//			{
	//				if (VertexData[VertexAmount]._X >= Groups[GroupAmount].GrpBBMax._X)
	//				{
	//					Groups[GroupAmount].GrpBBMax._X = VertexData[VertexAmount]._X;
	//				}
	//				if (VertexData[VertexAmount]._X <= Groups[GroupAmount].GrpBBMin._X)
	//				{
	//					Groups[GroupAmount].GrpBBMin._X = VertexData[VertexAmount]._X;
	//				}
	//				if (VertexData[VertexAmount]._Y >= Groups[GroupAmount].GrpBBMax._Y)
	//				{
	//					Groups[GroupAmount].GrpBBMax._Y = VertexData[VertexAmount]._Y;
	//				}
	//				if (VertexData[VertexAmount]._Y <= Groups[GroupAmount].GrpBBMin._Y)
	//				{
	//					Groups[GroupAmount].GrpBBMin._Y = VertexData[VertexAmount]._Y;
	//				}
	//				if (VertexData[VertexAmount]._Z >= Groups[GroupAmount].GrpBBMax._Z)
	//				{
	//					Groups[GroupAmount].GrpBBMax._Z = VertexData[VertexAmount]._Z;
	//				}
	//				if (VertexData[VertexAmount]._Z <= Groups[GroupAmount].GrpBBMin._Z)
	//				{
	//					Groups[GroupAmount].GrpBBMin._Z = VertexData[VertexAmount]._Z;
	//				}
	//			}
	//			VertexAmount++;		//Incremented later so we don't need to make [VertexAmount-1] when computing the bounding box
	//			GroupAmount++;		//This execution is over. Increment the GroupAmount for it's original state
	//		}
	//		else if (TempBuffer[1] == 't' && TempBuffer[2] == '\0')	//This line is a texture coordinate
	//		{
	//			fscanf(m_fileP, "%f %f\n", &UVData[UVAmount]._X, &UVData[UVAmount]._Y);
	//			UVAmount++;
	//		}
	//		else if (TempBuffer[1] == 'n' && TempBuffer[2] == '\0')	//This line is a vertex normal
	//		{
	//			fscanf(m_fileP, "%f %f %f\n", &NormalsData[NormalsAmount]._X, &NormalsData[NormalsAmount]._Y, &NormalsData[NormalsAmount]._Z);
	//			NormalsAmount++;
	//		}
	//	}
	//	else if (TempBuffer[0] == 'f' && TempBuffer[1] == '\0')		//This line is face data
	//	{	//By wavefront defaults, [x/y/z] for [Vertex to use/texture coordinate to use/normal to use]. they can appear or not in this format
	//		//Possible formats: x | x/y | x//z | x/y/z
	//		if (Pattern == 0)				//If the pattern is not know, find the pattern
	//		{
	//			fscanf(m_fileP, " %512[^\n]", TempBuffer);
	//			uint8 StringSize = patternChecker(TempBuffer);
	//			fseek(m_fileP, -StringSize - 2, SEEK_CUR);	//Reset the pointer to the start of the line
	//			//For this first time only!
	//		}
	//		//TODO: Check why with quadrilaterals the value is saved incorrectly (Reading the buffer 2 times after last line hit)
	//		else if (Pattern == PATTERN_X)
	//		{
	//			fscanf(m_fileP, "%d %d %d ", &VertexIndices[FacesAmount], &VertexIndices[FacesAmount + 1], &VertexIndices[FacesAmount + 2]);
	//			VertexIndices[FacesAmount]--;
	//			VertexIndices[FacesAmount + 1]--;
	//			VertexIndices[FacesAmount + 2]--;
	//			if (is_quadrilateral)
	//			{
	//				fscanf(m_fileP, "%d", &VertexIndices[FacesAmount + 3]);
	//				VertexIndices[FacesAmount + 3]--;
	//			}
	//			FacesAmount += 3;
	//			if (is_quadrilateral)
	//			{
	//				FacesAmount++;
	//			}
	//		}
	//		else if (Pattern == PATTERN_X_Y)
	//		{
	//			fscanf(m_fileP, "%d/%d %d/%d %d/%d ", &VertexIndices[FacesAmount], &UVIndices[FacesAmount], &VertexIndices[FacesAmount + 1], &UVIndices[FacesAmount + 1], &VertexIndices[FacesAmount + 2], &UVIndices[FacesAmount + 2]);
	//			VertexIndices[FacesAmount]--;
	//			VertexIndices[FacesAmount + 1]--;
	//			VertexIndices[FacesAmount + 2]--;
	//			UVIndices[FacesAmount]--;
	//			UVIndices[FacesAmount + 1]--;
	//			UVIndices[FacesAmount + 2]--;
	//			if (is_quadrilateral)
	//			{
	//				fscanf(m_fileP, "%d/%d", &VertexIndices[FacesAmount + 3], &UVIndices[FacesAmount + 3]);
	//				VertexIndices[FacesAmount + 3]--;
	//				UVIndices[FacesAmount + 3]--;
	//			}
	//			FacesAmount += 3;
	//			if (is_quadrilateral)
	//			{
	//				FacesAmount++;
	//			}
	//		}
	//		else if (Pattern == PATTERN_X_Y_Z)
	//		{
	//			fscanf(m_fileP, "%d/%d/%d %d/%d/%d %d/%d/%d ", &VertexIndices[FacesAmount], &UVIndices[FacesAmount], &NormalsIndices[FacesAmount], &VertexIndices[FacesAmount + 1], &UVIndices[FacesAmount + 1], &NormalsIndices[FacesAmount + 1], &VertexIndices[FacesAmount + 2], &UVIndices[FacesAmount + 2], &NormalsIndices[FacesAmount + 2]);
	//			VertexIndices[FacesAmount]--;
	//			VertexIndices[FacesAmount + 1]--;
	//			VertexIndices[FacesAmount + 2]--;
	//			UVIndices[FacesAmount]--;
	//			UVIndices[FacesAmount + 1]--;
	//			UVIndices[FacesAmount + 2]--;
	//			NormalsIndices[FacesAmount]--;
	//			NormalsIndices[FacesAmount + 1]--;
	//			NormalsIndices[FacesAmount + 2]--;
	//			if (is_quadrilateral)
	//			{
	//				fscanf(m_fileP, "%d/%d/%d", &VertexIndices[FacesAmount + 3], &UVIndices[FacesAmount + 3], &NormalsIndices[FacesAmount + 3]);
	//				VertexIndices[FacesAmount + 3]--;
	//				UVIndices[FacesAmount + 3]--;
	//				NormalsIndices[FacesAmount + 3]--;
	//			}
	//			FacesAmount += 3;
	//			if (is_quadrilateral)
	//			{
	//				FacesAmount++;
	//			}
	//		}
	//		else
	//		{
	//			fscanf(m_fileP, "%d//%d %d//%d %d//%d ", &VertexIndices[FacesAmount], &NormalsIndices[FacesAmount], &VertexIndices[FacesAmount + 1], &NormalsIndices[FacesAmount + 1], &VertexIndices[FacesAmount + 2], &NormalsIndices[FacesAmount + 2]);
	//			VertexIndices[FacesAmount]--;
	//			VertexIndices[FacesAmount + 1]--;
	//			VertexIndices[FacesAmount + 2]--;
	//			NormalsIndices[FacesAmount]--;
	//			NormalsIndices[FacesAmount + 1]--;
	//			NormalsIndices[FacesAmount + 2]--;
	//			if (is_quadrilateral)
	//			{
	//				fscanf(m_fileP, "%d//%d", &VertexIndices[FacesAmount + 3], &NormalsIndices[FacesAmount + 3]);
	//				VertexIndices[FacesAmount + 3]--;
	//				NormalsIndices[FacesAmount + 3]--;
	//			}
	//			FacesAmount += 3;
	//			if (is_quadrilateral)
	//			{
	//				FacesAmount++;
	//			}
	//		}
	//	}
	//}
	if (have_groups)
	{
		Groups[GroupAmount - 1].RangeEnd = FacesAmount;
	}
	else							//If the object have no groups, make one for all the face data
	{
		Groups[0].RangeStart = 0;
		Groups[0].RangeEnd = FacesAmount;
		//GroupAmount++;
	}
	AbsoluteFacesAmount = FacesAmount;
	if (is_quadrilateral)			//Fix to display the amount correctly
	{
		FacesAmount /= 4;
	}
	else
	{
		FacesAmount /= 3;
	}
	closeFile();
	uint8 Comparator = strlen(OverrideTexturePath);
	if (!TextureOverride || ((OverrideTexturePath[Comparator - 3] == 'm') && (OverrideTexturePath[Comparator - 2] == 't') && (OverrideTexturePath[Comparator - 1] == 'l')))
	{
		TextureOverride = false;		//Fail safe, because if the OverrideTexture was feed with a .mtl, we must NOT override the texture loader with it's path
		if (have_materials)				//If the .mtl indentifier was found within the .obj file, load the materials
		{
			setMaterials();
			if (is_mtlSuccess)			//If .mtl file reading failed, then we CANNOT call fclose, since the file was never opened in the first place
			{
				closeFile();
			}
		}
	}
}

void ObjLoader::setMaterials(void)
{
	uint8 LastSlash = 0, i = 0, j = 0;
	char MaterialPath[MAX_LEN_FILEPATH];
	if (OverrideTexturePath[0] != '\0')			//If the override texture is not NULL, then overwrite the material path with it's data
	{
		sprintf(MaterialPath, OverrideTexturePath);
	}
	else
	{
		for (i = 0; FilePath[i] != '\0'; i++)
		{
			if (FilePath[i] == '\\' || FilePath[i] == '/')
			{
				LastSlash = i;
			}
		}
		for (i = 0; i < LastSlash; i++, j++)
		{
			DirectoryPath[i] = FilePath[i];
			MaterialPath[i] = FilePath[i];
		}
		j++;
		DirectoryPath[i] = '/';
		DirectoryPath[i + 1] = '\0';
		MaterialPath[i] = '/';
		for (i = 0; MaterialFilename[i] != '\0'; i++, j++)
		{
			MaterialPath[j] = MaterialFilename[i];
		}
		MaterialPath[j] = '\0';
	}
	//All set, let's read that data
	openFile(MaterialPath, "r");
	if (m_fileP == NULL)
	{
		have_materials = false;
	}
	else
	{
		is_mtlSuccess = true;
		char TempBuffer[MAX_BUFFER_SIZE];
		char Dump[MAX_BUFFER_SIZE];
		//TODO: Make the reader knows if the end-of-line character is CRLF (Windows) or LF (UNIX)
		//START- First pass
		while (!feof(m_fileP))
		{
			fscanf(m_fileP, " %s", &TempBuffer);
			if (TempBuffer[0] == '#')
			{
				fscanf(m_fileP, " %512[^\n]", &Dump);
			}
			else if (TempBuffer[0] == 'n' && TempBuffer[1] == 'e')										//This line defines the material name
			{
				MaterialAmount++;
			}
		}
		//END - First pass
		rewind(m_fileP);							//ALL SET! Reseting the pointer

		Materials = new MaterialData[MaterialAmount];

		MaterialAmount = 0;
		TextureAmount = 0;
		int16 i;
		//START - Data gathering
		while (!feof(m_fileP))
		{
			fscanf(m_fileP, " %s", &TempBuffer);
			if (TempBuffer[0] == '#')
			{
				fscanf(m_fileP, " %512[^\n]", &Dump);
			}
			else if (TempBuffer[0] == 'n' && TempBuffer[1] == 'e')										//This line defines the material name
			{
				fscanf(m_fileP, " %s", &Materials[MaterialAmount].MaterialName);
				Materials[MaterialAmount].ID = MaterialAmount + 1;
				MaterialAmount++;
			}
			else if (TempBuffer[0] == 'm')									//If the first char is 'm', then the line is "map_". Wich means, a texture
			{
				if (TempBuffer[4] == 'k' || TempBuffer[4] == 'K')			//This line contains a texture property
				{
					//In this pattern: map_k* texture
					if (TempBuffer[5] == 'a' || TempBuffer[5] == 'A')		//This line defines the texture ambient color
					{
						if (TempBuffer[7] != '\0')							//This line is not NULL, then it defines the texture
						{
							fscanf(m_fileP, " %s", &Materials[MaterialAmount - 1].KaMap);
							TextureAmount++;
						}
					}
					else if (TempBuffer[5] == 's' || TempBuffer[5] == 'S')	//This line defines the texture specular color
					{
						if (TempBuffer[7] != '\0')							//This line is not NULL, then it defines the texture
						{
							fscanf(m_fileP, " %s", &Materials[MaterialAmount - 1].KsMap);
							TextureAmount++;
						}
					}
					else if (TempBuffer[5] == 'd' || TempBuffer[5] == 'D')	//This line defines the texture diffuse color
					{
						if (TempBuffer[7] != '\0')							//This line is not NULL, then it defines the texture
						{
							fscanf(m_fileP, " %s", &Materials[MaterialAmount - 1].KdMap);
							TextureAmount++;
						}
					}
				}
				else if (TempBuffer[4] == 'd')								//This line is the alpha texture map
				{
					if (TempBuffer[6] != '\0')								//This line is not NULL, then it defines the texture
					{
						fscanf(m_fileP, " %s", &Materials[MaterialAmount - 1].dMap);
						TextureAmount++;
					}
				}
				else if (TempBuffer[4] == 'n' || TempBuffer[4] == 'N')		//This line sets the specular color texture map
				{
					if (TempBuffer[7] != '\0')								//If this line is not NULL, then it defines the texture
					{
						fscanf(m_fileP, " %s", &Materials[MaterialAmount - 1].NsMap);
						TextureAmount++;
					}
				}
				else if (TempBuffer[4] == 'b')								//This line is the bump map texture. Pattern: "map_bump texture"
				{
					if (TempBuffer[9] != '\0')								//This line is not NULL, then it defines the texture
					{
						fscanf(m_fileP, " %s", &Materials[MaterialAmount - 1].bumpMap);
						TextureAmount++;
					}
				}
			}
			else if (TempBuffer[0] == 'b')									//This line defines the bump map
			{
				if (TempBuffer[5] != '\0')									//This line is not NULL, then it defines the texture
				{
					fscanf(m_fileP, " %s", &Materials[MaterialAmount - 1].bumpMap);
					TextureAmount++;
				}
			}
			else if (TempBuffer[0] == 'd' || TempBuffer[0] == 'D' || TempBuffer[0] == 'T')		//This line contains a material property. In this case, the opacity (dissolve)	
			{
				if (TempBuffer[0] == 'T')
				{
					fscanf(m_fileP, " %f", &Materials[MaterialAmount - 1].d);
				}
				else
				{
					fscanf(m_fileP, " %f", &Materials[MaterialAmount - 1].d);
				}
			}
			else if (TempBuffer[0] == 'k' || TempBuffer[0] == 'K')								//This line contains a material property
			{
				if (TempBuffer[1] == 's' || TempBuffer[1] == 'S')								//This line holds the specular color
				{
					fscanf(m_fileP, " %f %f %f", &Materials[MaterialAmount - 1].Ks[0], &Materials[MaterialAmount - 1].Ks[1], &Materials[MaterialAmount - 1].Ks[2]);
				}
				else if (TempBuffer[1] == 'a' || TempBuffer[1] == 'A')							//This line holds the ambient color
				{
					fscanf(m_fileP, " %f %f %f", &Materials[MaterialAmount - 1].Ka[0], &Materials[MaterialAmount - 1].Ka[1], &Materials[MaterialAmount - 1].Ka[2]);
				}
				else if (TempBuffer[1] == 'd' || TempBuffer[1] == 'D')							//This line holds the diffuse color
				{
					fscanf(m_fileP, " %f %f %f", &Materials[MaterialAmount - 1].Kd[0], &Materials[MaterialAmount - 1].Kd[1], &Materials[MaterialAmount - 1].Kd[2]);
				}
			}
			else if ((TempBuffer[0] == 'n' || TempBuffer[0] == 'N') && TempBuffer[1] != 'e')
			{
				if (TempBuffer[1] == 's' || TempBuffer[1] == 'S')
				{
					fscanf(m_fileP, " %f", &Materials[MaterialAmount - 1].Ns);
				}
				else if (TempBuffer[1] == 'i' || TempBuffer[1] == 'I')
				{
					fscanf(m_fileP, " %f", &Materials[MaterialAmount - 1].Ni);
				}
			}
			else if (TempBuffer[0] == 'i')														//This line sets the Illumination model
			{
				fscanf(m_fileP, "%d", &Materials[MaterialAmount - 1].illum);
			}
		}
	}
	if (is_mtlSuccess)
	{
		TextureAmount = countUniqueTextures(TextureAmount);
		if (TextureAmount == NULL)
		{
			TextureAmount = 1;
		}
		Texture = new uint32[TextureAmount];
		//All materials set, let's make a reference for them, that goes inside each "Group"
		uint16 LastMaterialIndex = 0;
		for (i = 0; i < GroupAmount; i++)
		{
			for (int j = 0; j < MaterialAmount; j++)
			{
				if (!strcmp(Groups[i].MaterialReference, Materials[j].MaterialName))	//If found, use the value as the index for the renderer
				{																		//Only change when another usemtl is defined (or in this case, defined as MaterialReference)
					LastMaterialIndex = Materials[j].ID;
				}
				Groups[i].MaterialID = LastMaterialIndex;

				Materials[j].AmbientMaterial[0] = Materials[j].Ka[0];
				Materials[j].AmbientMaterial[1] = Materials[j].Ka[1];
				Materials[j].AmbientMaterial[2] = Materials[j].Ka[2];
				Materials[j].AmbientMaterial[3] = Materials[j].d;

				Materials[j].DiffuseMaterial[0] = Materials[j].Kd[0];
				Materials[j].DiffuseMaterial[1] = Materials[j].Kd[1];
				Materials[j].DiffuseMaterial[2] = Materials[j].Kd[2];
				Materials[j].DiffuseMaterial[3] = Materials[j].d;

				Materials[j].SpecularMaterial[0] = Materials[j].Ks[0];
				Materials[j].SpecularMaterial[1] = Materials[j].Ks[1];
				Materials[j].SpecularMaterial[2] = Materials[j].Ks[2];
				Materials[j].SpecularMaterial[3] = Materials[j].d;
			}
		}
	}
}

void ObjLoader::setCenter(float Scale)
{
	ObjBBMax *= Scale;
	ObjBBMin *= Scale;

	ObjCenter._X = (ObjBBMax._X + ObjBBMin._X) * 0.5;
	ObjCenter._Y = (ObjBBMax._Y + ObjBBMin._Y) * 0.5;
	ObjCenter._Z = (ObjBBMax._Z + ObjBBMin._Z) * 0.5;
	if (have_groups)
	{
		for (int i = 0; i < GroupAmount; i++)
		{
			Groups[i].GrpBBMax *= Scale;
			Groups[i].GrpBBMin *= Scale;

			Groups[i].GrpCenter._X = (Groups[i].GrpBBMax._X + Groups[i].GrpBBMin._X) * 0.5;
			Groups[i].GrpCenter._Y = (Groups[i].GrpBBMax._Y + Groups[i].GrpBBMin._Y) * 0.5;
			Groups[i].GrpCenter._Z = (Groups[i].GrpBBMax._Z + Groups[i].GrpBBMin._Z) * 0.5;
		}
	}
}

void ObjLoader::setGroupPosition(uint16 GroupIndex, mwVec3f &Position)
{
	if ((GroupIndex >= 0) && (GroupIndex < GroupAmount))
	{
		Groups[GroupIndex].GrpPosition = Position;
	}
}

void ObjLoader::setGroupPosition(uint16 GroupIndex, float Position[])
{
	if ((GroupIndex >= 0) && (GroupIndex < GroupAmount))
	{
		Groups[GroupIndex].GrpPosition._X = Position[0];
		Groups[GroupIndex].GrpPosition._Y = Position[1];
		Groups[GroupIndex].GrpPosition._Z = Position[2];
	}
}

void ObjLoader::setGroupRotation(uint16 GroupIndex, mwVec3f &Rotation, mwVec3f &Axis)
{
	if ((GroupIndex >= 0) && (GroupIndex < GroupAmount))
	{
		Groups[GroupIndex].GrpRotation = Rotation;
		Groups[GroupIndex].GrpRotationAxis = Axis;
	}
}

void ObjLoader::setGroupRotation(uint16 GroupIndex, float Rotation[], float Axis[])
{
	if ((GroupIndex >= 0) && (GroupIndex < GroupAmount))
	{
		Groups[GroupIndex].GrpRotation._X = Rotation[0];
		Groups[GroupIndex].GrpRotation._Y = Rotation[1];
		Groups[GroupIndex].GrpRotation._Z = Rotation[2];

		Groups[GroupIndex].GrpRotationAxis._X = Axis[0];
		Groups[GroupIndex].GrpRotationAxis._Y = Axis[1];
		Groups[GroupIndex].GrpRotationAxis._Z = Axis[2];
	}
}

void ObjLoader::setObjectPosition(mwVec3f &Position)
{
	ObjPosition = Position;
}

void ObjLoader::setObjectPosition(float Position[])
{
	ObjPosition._X = Position[0];
	ObjPosition._Y = Position[1];
	ObjPosition._Z = Position[2];
}

void ObjLoader::setObjectRotation(mwVec3f &Rotation, mwVec3f &Axis)
{
	ObjRotation = Rotation;
	ObjRotationAxis = Axis;
}

void ObjLoader::setObjectRotation(float Rotation[], float Axis[])
{
	ObjRotation._X = Rotation[0];
	ObjRotation._Y = Rotation[1];
	ObjRotation._Z = Rotation[2];

	ObjRotationAxis._X = Axis[0];
	ObjRotationAxis._Y = Axis[1];
	ObjRotationAxis._Z = Axis[2];
}

int ObjLoader::countUniqueTextures(uint16 PredictedAmount)
{
	// !! WARNING !! ASSUMING THE MTL FILE DOESN'T USE THE SAME TEXTURE TO MAP DIFERENT MATERIALS !!! VERY UNSAFE
	int Unique = 0;
	//First Pass
	for (int i = 0; i < MaterialAmount; i++)
	{
		if (Materials[i].KaMap[0] != '\0')			//Let's see if the values are not '\0', witch by default means they are not NULL
		{
			Unique++;
		}
		if (Materials[i].KdMap[0] != '\0')
		{
			Unique++;
		}
		if (Materials[i].KsMap[0] != '\0')
		{
			Unique++;
		}
		if (Materials[i].NsMap[0] != '\0')
		{
			Unique++;
		}
		if (Materials[i].bumpMap[0] != '\0')
		{
			Unique++;
		}
		if (Materials[i].decalMap[0] != '\0')
		{
			Unique++;
		}
		if (Materials[i].dispMap[0] != '\0')
		{
			Unique++;
		}
		if (Materials[i].dMap[0] != '\0')
		{
			Unique++;
		}
	}
	//Data gathering
	//Upon dynamic creating of TextureFilename, when this function exits, will be a block of memory of (String * PredictedAmount) Fragmented!
	if(Unique > 0)
	{
		TextureFilename = new String64[Unique];
	}
	for (int i = 0, Unique = 0; i < MaterialAmount; i++)
	{
		if (Materials[i].KaMap[0] != '\0')			//Let's see if the values are not '\0', witch by default means they are not NULL
		{
			sprintf(TextureFilename[Unique].getString(), Materials[i].KaMap);
			Unique++;
		}
		if (Materials[i].KdMap[0] != '\0')
		{
			sprintf(TextureFilename[Unique].getString(), Materials[i].KdMap);
			Unique++;
		}
		if (Materials[i].KsMap[0] != '\0')
		{
			sprintf(TextureFilename[Unique].getString(), Materials[i].KsMap);
			Unique++;
		}
		if (Materials[i].NsMap[0] != '\0')
		{
			sprintf(TextureFilename[Unique].getString(), Materials[i].NsMap);
			Unique++;
		}
		if (Materials[i].bumpMap[0] != '\0')
		{
			sprintf(TextureFilename[Unique].getString(), Materials[i].bumpMap);
			Unique++;
		}
		if (Materials[i].decalMap[0] != '\0')
		{
			sprintf(TextureFilename[Unique].getString(), Materials[i].decalMap);
			Unique++;
		}
		if (Materials[i].dispMap[0] != '\0')
		{
			sprintf(TextureFilename[Unique].getString(), Materials[i].dispMap);
			Unique++;
		}
		if (Materials[i].dMap[0] != '\0')
		{
			sprintf(TextureFilename[Unique].getString(), Materials[i].dMap);
			Unique++;
		}
	}
	for (int j = 0; j < MaterialAmount; j++)
	{
		for (int i = 0; i < Unique; i++)
		{
			if (!strcmp(TextureFilename[i].getString(), Materials[j].bumpMap))		//strcmp return 0 for equal strings, and 0 is boolean false, then !0 = 1 = boolean true
			{
				Materials[j].TextureToGenerate++;
			}
			if (!strcmp(TextureFilename[i].getString(), Materials[j].decalMap))
			{
				Materials[j].TextureToGenerate++;
			}
			if (!strcmp(TextureFilename[i].getString(), Materials[j].dispMap))
			{
				Materials[j].TextureToGenerate++;;
			}
			if (!strcmp(TextureFilename[i].getString(), Materials[j].dMap))
			{
				Materials[j].TextureToGenerate++;;
			}
			if (!strcmp(TextureFilename[i].getString(), Materials[j].KaMap))
			{
				Materials[j].TextureToGenerate++;;
			}
			if (!strcmp(TextureFilename[i].getString(), Materials[j].KdMap))
			{
				Materials[j].TextureToGenerate++;;
			}
			if (!strcmp(TextureFilename[i].getString(), Materials[j].KsMap))
			{
				Materials[j].TextureToGenerate++;;
			}
			if (!strcmp(TextureFilename[i].getString(), Materials[j].NsMap))
			{
				Materials[j].TextureToGenerate++;;
			}
		}
	}
	return Unique;
}

int ObjLoader::patternChecker(char Buffer[])
{
	uint8 SlashCounter = 0;
	uint8 SpaceCount = 0;
	uint8 i;
	bool has_NumberBetween = false;			//Flag. If true, then between the slash, there is a number (X / Y / Z). If false, there isn't (X//Z) or (X/Y) or (X)
	bool is_NumPatSet = false;				//Flag. So we don't check if "there is number in between" zilions of times
	for (i = 0; Buffer[i] != '\0'; i++)
	{
		if (Buffer[i] == '/')
		{
			SlashCounter++;
			if (!is_NumPatSet)
			{
				if (Buffer[i + 1] == '/' || Buffer[i + 1] == ' ')	//Checks if after the / there is another / or a space (indicating the next value) if true, then it DOES'NT have a number in between
				{
					has_NumberBetween = false;
				}
				else
				{
					has_NumberBetween = true;
				}
				is_NumPatSet = true;
			}
		}
		else if (Buffer[i] == ' ')
		{
			SpaceCount++;
		}
	}
	if (SpaceCount == 2)					//If the sample only have 2 spaces, then we have 3 values -> it's not quadrilateral
	{
		is_quadrilateral = false;
		FaceSize = 3;
	}
	else									//If the sample have more than 2 spaces, then we have 4 values -> it's quadrilateral
	{
		is_quadrilateral = true;
		FaceSize = 4;
	}
	if (has_NumberBetween)					//Signature detected: X/Y/Z
	{
		Pattern = PATTERN_X_Y_Z;
	}
	else
	{
		if (SlashCounter == 0)				//Signature detected: X (This is also, a fail-safe)
		{
			Pattern = PATTERN_X;
		}
		else
		{
			if (SlashCounter % 2 == 0)		//Signature detected: X//Z
			{
				Pattern = PATTERN_X_Z;
			}
			else							//Signature detected: X/Y (and also, a fail-safe)
			{
				Pattern = PATTERN_X_Y;
			}
		}
	}
	return i;
}

void ObjLoader::resetMaxMin(void)
{
	ObjBBMax = -99999;
	ObjBBMin = 99999;
}

void ObjLoader::loadObject(char *Filename)
{
	if (!is_objectBuilt)
	{
		resetMaxMin();
		openFile(Filename, "r");
		if (m_stateCode == FileStatus::FS_SUCCESS)
		{
			sprintf(FilePath, "%s", Filename);
			is_fromFile = true;
			setData();
			is_objectBuilt = true;
		}
		else
		{
			is_fromFile = false;
		}
	}
}

void ObjLoader::loadObject(char *Filename, char *Texture)
{
	if (!is_objectBuilt)
	{
		TextureOverride = true;
		resetMaxMin();
		openFile(Filename, "r");
		if (m_stateCode == FileStatus::FS_SUCCESS)
		{
			sprintf(FilePath, "%s", Filename);
			sprintf(OverrideTexturePath, Texture);
			is_fromFile = true;
			setData();
			is_objectBuilt = true;
		}
		else
		{
			is_fromFile = false;
		}
	}
}

void ObjLoader::loadTexture(void)				//Loads a texture data from the complimentary .mtl file
{
	uint16 MaterialSeeker = 0;
	uint16 RemaningTexCounter = 0;
	char Filename[MAX_LEN_FILEPATH];
	if(TextureFilename == nullptr)				//No textures, abort!
	{
		return;
	}
	for (int i = 0; i < TextureAmount; i++)
	{
		if (TextureFilename[i].getString()[1] == ':')				//If the filename has ':', then it's the full path
		{
			sprintf(Filename, "%s", TextureFilename[i].getString());
		}
		else														//If not, then the file is supposed to be at the same directory of the .mtl
		{
			sprintf(Filename, "%s%s", DirectoryPath, TextureFilename[i].getString());
		}
		Texture[i] = MWTHLoadTexture(Filename);
		if (RemaningTexCounter < Materials[MaterialSeeker].TextureToGenerate)
		{
			Materials[MaterialSeeker].TextureIndex[RemaningTexCounter] = i;
			RemaningTexCounter++;
		}
		else
		{
			RemaningTexCounter = 0;
			MaterialSeeker++;
			Materials[MaterialSeeker].TextureIndex[RemaningTexCounter] = i;
			RemaningTexCounter++;
		}
	}
}

void ObjLoader::loadTexture(char *Filename)		//Loads a single texture from an external file
{
	Materials = new MaterialData[1];				//If we override the object with a texture, alloc only 1 slot for it
	Texture = new uint32[1];						//Same here
	Texture[0] = MWTHLoadTexture(Filename);

	//Texture[ Materials[Groups[j].MaterialID - 1].TextureIndex[0] ] Acess pointer mess, just for reference
	Materials[0].ID = 1;
	Materials[0].TextureIndex[0] = 0;			//Set this newly made texture to the only assigned slot for this object, 0

	for (int i = 0; i < GroupAmount; i++)		//Now set all the group's reference material to this
	{
		Groups[i].MaterialID = 1;
	}
}

void ObjLoader::unloadObject(void)
{
	if (is_objectBuilt)
	{
		//Unloading a object. First, free all data used, and then set all flags back to the original state
		glDeleteTextures(TextureAmount, Texture);	//Deleting the texture data
		delete[] Groups;
		Groups = NULL;
		delete[] VertexIndices;
		VertexIndices = NULL;
		delete[] NormalsIndices;
		NormalsIndices = NULL;
		delete[] UVIndices;
		UVIndices = NULL;
		delete[] VertexData;
		VertexData = NULL;
		delete[] UVData;
		UVData = NULL;
		delete[] NormalsData;
		NormalsData = NULL;
		delete[] Materials;
		Materials = NULL;
		delete[] TextureFilename;
		TextureFilename = NULL;
		delete[] Texture;
		Texture = NULL;
		Pattern = 0;
		AbsoluteFacesAmount = 0;
		TextureAmount = 0;
		FacesAmount = 0;
		VertexAmount = 0;
		UVAmount = 0;
		NormalsAmount = 0;
		GroupAmount = 0;
		MaterialAmount = 0;
		FaceSize = 0;
		resetMaxMin();
		ObjCenter = 0;
		ObjRotation = 0;
		ObjRotationAxis = 0;
		ObjectName[0] = NULL;
		MaterialFilename[0] = NULL;
		DirectoryPath[0] = NULL;
		FilePath[0] = NULL;
		OverrideTexturePath[0] = NULL;
		TextureOverride = false;
		is_quadrilateral = false;
		is_textured = false;
		is_fromFile = false;
		is_centerComputed = false;
		have_materials = false;
		have_normals = false;
		have_groups = false;
		generated_texture = false;
		is_shadingSmooth = false;
		is_objectBuilt = false;					//New object incoming! Must build it again
	}
}

void ObjLoader::replaceObject(char *Filename)
{
	unloadObject();
	if (!is_objectBuilt)
	{
		loadObject(Filename);
	}
}

void ObjLoader::replaceObject(char *Filename, char *Texture)
{
	unloadObject();
	if (!is_objectBuilt)
	{
		loadObject(Filename, Texture);
	}
}

#undef MAX_NAME
#undef MAX_LEN_FILEPATH
#undef MAX_BUFFER_SIZE