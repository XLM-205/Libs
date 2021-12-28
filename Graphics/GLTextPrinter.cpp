/*-------OpenGL Text Printer-------
*	OpenGL assisted text printer
*	--Prints text on the screen, if it is only text, only numbers, or a combination of them
*
*
*	Moon Wiz Studios (c) - 02/06/2015
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 1.0.0
*/

#include "GLTextPrinter.h"

#ifdef _WIN32
	#include <sstream>
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
#endif;

#include <GL\freeglut.h>

#define MAX_BUFFER_SIZE 128

void gtpComboPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, char *Text1, double Value, char *Text2, void* glutFont, int Precision)
{
	int Length;
	char Text[MAX_BUFFER_SIZE];
	glColor3ub(Red, Green, Blue);
	glRasterPos3f(PosX, PosY, PosZ);
	switch (Precision)
	{
	case GTP_PRECISION_NONE:
	default:
		sprintf(Text, "%s%0.0f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_BASIC:
		sprintf(Text, "%s%.2f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_DOUBLE:
		sprintf(Text, "%s%.4f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_QUAD:
		sprintf(Text, "%s%.8f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_MAX:
		sprintf(Text, "%s%.16f%s", Text1, Value, Text2);
		break;
	}
	Length = strlen(Text);
	for (int i = 0; i < Length; i++)
	{
		glutBitmapCharacter(glutFont, Text[i]);
	}
}

void gtpComboPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, unsigned char *Text1, double Value, char *Text2, void* glutFont, int Precision)
{
	int Length;
	char Text[MAX_BUFFER_SIZE];
	glColor3ub(Red, Green, Blue);
	glRasterPos3f(PosX, PosY, PosZ);
	switch (Precision)
	{
	case GTP_PRECISION_NONE:
	default:
		sprintf(Text, "%s%0.0f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_BASIC:
		sprintf(Text, "%s%.2f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_DOUBLE:
		sprintf(Text, "%s%.4f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_QUAD:
		sprintf(Text, "%s%.8f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_MAX:
		sprintf(Text, "%s%.16f%s", Text1, Value, Text2);
		break;
	}
	Length = strlen(Text);
	for (int i = 0; i < Length; i++)
	{
		glutBitmapCharacter(glutFont, Text[i]);
	}
}

void gtpComboPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, char *Text1, double Value, unsigned char *Text2, void* glutFont, int Precision)
{
	int Length;
	char Text[MAX_BUFFER_SIZE];
	glColor3ub(Red, Green, Blue);
	glRasterPos3f(PosX, PosY, PosZ);
	switch (Precision)
	{
	case GTP_PRECISION_NONE:
	default:
		sprintf(Text, "%s%0.0f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_BASIC:
		sprintf(Text, "%s%.2f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_DOUBLE:
		sprintf(Text, "%s%.4f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_QUAD:
		sprintf(Text, "%s%.8f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_MAX:
		sprintf(Text, "%s%.16f%s", Text1, Value, Text2);
		break;
	}
	Length = strlen(Text);
	for (int i = 0; i < Length; i++)
	{
		glutBitmapCharacter(glutFont, Text[i]);
	}
}

void gtpComboPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, unsigned char *Text1, double Value, unsigned char *Text2, void* glutFont, int Precision)
{
	int Length;
	char Text[MAX_BUFFER_SIZE];
	glColor3ub(Red, Green, Blue);
	glRasterPos3f(PosX, PosY, PosZ);
	switch (Precision)
	{
	case GTP_PRECISION_NONE:
	default:
		sprintf(Text, "%s%0.0f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_BASIC:
		sprintf(Text, "%s%.2f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_DOUBLE:
		sprintf(Text, "%s%.4f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_QUAD:
		sprintf(Text, "%s%.8f%s", Text1, Value, Text2);
		break;
	case GTP_PRECISION_MAX:
		sprintf(Text, "%s%.16f%s", Text1, Value, Text2);
		break;
	}
	Length = strlen(Text);
	for (int i = 0; i < Length; i++)
	{
		glutBitmapCharacter(glutFont, Text[i]);
	}
}

void gtpTextPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, char *Text, void* glutFont)
{
	int Length = strlen(Text);
	glColor3ub(Red, Green, Blue);
	glRasterPos3f(PosX, PosY, PosZ);
	for (int i = 0; i < Length; i++)
	{
		glutBitmapCharacter(glutFont, Text[i]);
	}
}

void gtpTextPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, unsigned char *Text, void* glutFont)
{
	int Length = strlen((char*)Text);
	glColor3ub(Red, Green, Blue);
	glRasterPos3f(PosX, PosY, PosZ);
	for (int i = 0; i < Length; i++)
	{
		glutBitmapCharacter(glutFont, Text[i]);
	}
}

void gtpValuePrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, double Value, void* glutFont, int Precision)
{
	int Length;
	char Text[MAX_BUFFER_SIZE];
	glColor3ub(Red, Green, Blue);
	glRasterPos3f(PosX, PosY, PosZ);
	switch (Precision)
	{
	case GTP_PRECISION_NONE:
	default:
		sprintf(Text, "%.0f", Value);
		break;
	case GTP_PRECISION_BASIC:
		sprintf(Text, "%.2f", Value);
		break;
	case GTP_PRECISION_DOUBLE:
		sprintf(Text, "%.4f", Value);
		break;
	case GTP_PRECISION_QUAD:
		sprintf(Text, "%.8f", Value);
		break;
	case GTP_PRECISION_MAX:
		sprintf(Text, "%.16f", Value);
		break;
	}
	Length = strlen(Text);
	for (int i = 0; i < Length; i++)
	{
		glutBitmapCharacter(glutFont, Text[i]);
	}
}