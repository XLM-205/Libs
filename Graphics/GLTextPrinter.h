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

// !! TODO !! CUSTOM FONT READER

#ifndef _H_GTP_
#define _H_GTP_

//Precision
#define GTP_PRECISION_NONE		1				//No numbers significant numbers will be written (display values as int)
#define GTP_PRECISION_BASIC		2				//2 significant numbers will be written
#define GTP_PRECISION_DOUBLE	3				//4 significant numbers will be written
#define GTP_PRECISION_QUAD		4				//8 significant numbers will be written
#define GTP_PRECISION_MAX		5				//16 significant numbers will be written (if possible)

//GTP -> Generic Text Printer

typedef unsigned char ub;

//Prints a text -> value -> text
void gtpComboPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, char *Text1, double Value, char *Text2, void* glutFont, int Precision);
//Prints a text -> value -> text
void gtpComboPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, unsigned char *Text1, double Value, char *Text2, void* glutFont, int Precision);
//Prints a text -> value -> text
void gtpComboPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, char *Text1, double Value, unsigned char *Text2, void* glutFont, int Precision);
//Prints a text -> value -> text
void gtpComboPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, unsigned char *Text1, double Value, unsigned char *Text2, void* glutFont, int Precision);

//Prints a simple text
void gtpTextPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, char *Text, void* glutFont);
//Prints a simple text
void gtpTextPrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, unsigned char *Text, void* glutFont);

//Prints a value
void gtpValuePrinter(float PosX, float PosY, float PosZ, ub Red, ub Green, ub Blue, double Value, void* glutFont, int Precision);
#endif;