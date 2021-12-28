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
*	Version ---
*/

#ifndef _H_MWIMAGEPANEL_
#define _H_MWIMAGEPANEL_
//a.k.a MWIP -> Moon Wiz Image Panel

#include "..\Maths\Vec3d.h"
#include "..\Control\CustomTypes-SA.h"
//#include "..\Control\ThreadController.h"
#include "..\Control\FileStates.h"

//Faces & Sides
#define MWIP_FACE_SINGLE_SIDE				0		//Panel will have only one visible side (default)
#define MWIP_FACE_DOUBLE_SIDE				1		//Panel will have both sides visible
#define MWIP_SIDE_FRONT						0		//Panel's front side
#define MWIP_SIDE_BACK						1		//Panel's back side
#define MWIP_SIDE_BOTH						2		//Both sides of the panel. Used ONLY on deleteTextureData() and computeRatios()
//Panel Status
#define MWIP_STATUS_FAIL					0		//Panel could not be constructed
#define MWIP_STATUS_SUCCESS					1		//Panel was constructed and ready
//Colision detection
#define MWIP_HIT_FAIL					   -1		//Colision mouse x panel failed (mouse is outside panel/not over it)
//Panel Dimension
#define MWIP_2D								1		//Panel will be created for a 2D environment (default)
#define MWIP_3D								2		//Panel will be created for a 3D environment
//UV Modes
#define MWIP_UV_FULL						0		//Panel will map the entire texture (default)
#define MWIP_UV_MANUAL						1		//Panel will map the texture with a specific UV data (if no input was given, UV will be equivalent to full)
//Default Colors
#define MWIP_COLOR_WHITE			255,255,255,255	//White Panel BG color
#define MWIP_COLOR_RED			      255,0,0,255	//Red Panel BG Color
#define MWIP_COLOR_GREEN			  0,255,0,255	//Green Panel BG Color
#define MWIP_COLOR_BLUE				  0,0,255,255	//Blue Panel BG Color
#define MWIP_COLOR_YELLOW			 255,255,0,255	//Yellow Panel BG Color
#define MWIP_COLOR_BLACK			   0,0,0,255	//Black Panel BG Color
//Normal resolution
#define MWIP_NORMALS_OFF					0		//No normals, for 2D and no ilumination (default)
#define MWIP_NORMALS_ON						1		//Basic normal resolution (1 normal)
//Image related behaviors
#define MWIP_BEHAVE_ADAPT_RATIO				0		//Adapt the ratio to match the sprite sheet
#define MWIP_BEHAVE_KEEP_RATIO				1		//Keep the ratio, even for non power of 2 sprite sheets (default)
//Ratio Addaptation Mode
#define MWIP_ADDAPT_RATIO_NORMAL			0		//The X and Y will be changed both on X- / X+ and Y- / Y+, according to panel ratios (default)
#define MWIP_ADDAPT_RATIO_UV				1		//Will addapt X and Y following the UV rules (Only change X+ and Y+)
//Texture Orientation
#define MWIP_ORIENTATION_IGNORE			   -1		//Equivalent of null. Use this if you don't want to change a face's orientation
#define MWIP_ORIENTATION_UP					0		//Texture orientation will be the default (upward) (default)
#define MWIP_ORIENTATION_RIGHT				1		//Texture orientation will be the rightward
#define MWIP_ORIENTATION_DOWN				2		//Texture orientation will be the downwward
#define MWIP_ORIENTATION_LEFT				3		//Texture orientation will be the leftward
//File Warnings
#define MWIP_FILE_SUCESS					1		//File FOUND and READ
#define MWIP_ERROR_NO_PATH				   -1		//NO file path/name given
#define MWIP_ERROR_NO_FILE				   -2		//NO file found/read with given file name
#define MWIP_ERROR_INVALID_FILE			   -3		//File is not supported/not a valid image file
//File Message Warnings
#define MWIP_WR_SUCCESS					"FILE SUCCESSFULLY READ"
#define	MWIP_WR_ERROR_NO_PATH			"ERROR! NO PATH GIVEN"
#define MWIP_WR_ERROR_NO_FILE			"ERROR! NO FILE FOUND"
#define MWIP_WR_ERROR_INVALID_PATH		"ERROR! INVALID FILE"
#define MWIP_WR_ERROR_MULTIPLE			"ERROR! ONE OR BOTH PATH ARE INVALID"

// TODO: Convert macros to enums
//enum POrient
//{
//	MWIP_ORIENTATION_IGNORE = -1,	//Equivalent of null. Use this if you don't want to change a face's orientation
//	MWIP_ORIENTATION_UP,			//Texture orientation will be the default (upward) (default)
//	MWIP_ORIENTATION_RIGHT,			//Texture orientation will be the rightward
//	MWIP_ORIENTATION_DOWN,			//Texture orientation will be the downwward
//	MWIP_ORIENTATION_LEFT			//Texture orientation will be the leftward
//};
//enum PSide
//{
//	MWIP_SIDE_FRONT = 0,		//Panel's front side
//	MWIP_SIDE_BACK,				//Panel's back side
//	MWIP_SIDE_BOTH				//Both sides of the panel. Used ONLY on deleteTextureData() and computeRatios()
//};

class ImagePanel : public FileStates
{
public:
	ImagePanel(void);																							//Don't constructs, just prepares
	ImagePanel(mwVec3f &Point1, mwVec3f &Point2, char *ImagePath, uint8 Dimension);								//Basic construction and load a texture
	ImagePanel(mwVec3f &Point1, mwVec3f &Point2, uint32 LoadedTexture, uint8 Dimension);						//Basic construction using a pre-loaded texture
	ImagePanel(mwVec3f &Point1, mwVec3f &Point2, char *ImagePath, char *ImagePath2, uint8 Dimension);			//Constructs with 2 images (double face implicit) and load a texture
	ImagePanel(mwVec3f &Point1, mwVec3f &Point2, uint32 LoadedTexture, uint32 LoadedTexture2, uint8 Dimension);	//Constructs with 2 images (double face implicit) using a pre-loaded texture

	
	void copyBasicData(ImagePanel &PanelObject, uint8 Side);										//(Single Side Copy) Copy basic data from a reference panel, with except of position, rotation, size and behaviors flags (state flags copied). Only works if the target panel has already been built
	void copyBasicDataAllSides(ImagePanel &PanelObject);											//Copy basic data from a reference panel, with except of position, rotation, size and behaviors flags (state flags copied). Only works if the target panel has already been built
	void deleteTextureData(uint8 Side);																//Only deletes it's texture data, keeping all of it's data. After using this, you MUST call setNewTexturePointer(). Recommended only with the same texture
	void destroyPanel(void);																		//Clears the all the panel data (flush)
	bool detectClick(float MouseX, float MouseY);													//Returns true if the panel was clicked AND if is_clickActive is true
	bool detectClick(mwVec2f &MousePosition);														//Returns true if the panel was clicked AND if is_clickActive is true (vec2)
	void drawPanel(void);																			//Draw the panel
	void flipPanel(void);																			//Flips the panel, counter-clockwise
	mwVec2f getHitPosition(mwVec2f &MousePosition);													//Detects the position, in the panel, the mouse is over. If not over, -1 is returned (MWIP_HIT_FAIL)
	mwVec2f getHitPositionFree(mwVec2f &MousePosition);												//Detects the position, in the panel, the mouse is over. If not over, a mouse-representation value is returned
	mwVec2f getPanelRatios(void) { return mwVec2f(XRatio, YRatio); };								//Returns the X and Y ratios, for non-addapted textures
	mwVec2f getPanelSize(void) { return mwVec2f(PointA._X - PointB._X, PointA._Y - PointB._Y); };	//Returns the total size of the panel
	mwVec3f getPanelPosition(void) { return Position; };											//Returns the panel position (for translating)
	mwVec3f getPointA(void) { return PointA; };														//Returns the first point to create the panel (upper left)
	mwVec3f getPointB(void) { return PointB; };														//Returns the first point to create the panel (bottomo right)
	int getTexturePointer(uint8 Side) { return Texture[Side]; };									//Returns the texture pointer
	int getTextureWidth(uint8 Side) { return TextureWidth[Side]; };									//Return the texture width in pixels
	int getTextureHeigth(uint8 Side) { return TextureHeigth[Side]; };								//Return the texture heigth in pixels
	int getPanelBehaviorStatus(void){ return KeepRatio; };											//Return the status of the actual behavior
	int getPanelFaces(void) { return !is_panelSingleFace; };										//Return the panel face mode. MWIP_FACE_SINGLE_SIDE for one face, MWIP_FACE_DOUBLE_SIDE for two
	int getPanelOrientation(uint8 Side) { return PanelOrientation[Side]; };							//Returns the actual panel orientation pointed by "Side"
	char* getTextureRatio(uint8 Side);																//Return the texture ratio (e.g.: 1.77 (16:9))
	char* getTextureResolution(uint8 Side);															//Return the texture resolution (e.g.: 600 x 800
	mwVec2f getUVA(void) { return UVA; };															//Returns the UVA data
	mwVec2f getUVB(void) { return UVB; };															//returns the UVB data
	mwVec2f getUVHitPosition(mwVec2f &MousePosition);												//Detects the position, in the panel, the mouse is over, but return the value as UV coordinates (clamped from 0~1, bottom left to upper right). If not over, -1 is returned (MWIP_HIT_FAIL)
	mwVec2f getUVHitPositionFree(mwVec2f &MousePosition);											//Detects the position, in the panel, the mouse is over, but return the value as UV coordinates (clamped from 0~1, bottom left to upper right). If not over, a mouse-representation value is returned
	void incrementPosition(float XDiference, float YDiference, float ZDiference);					//Update the position
	void incrementPosition(mwVec3f &PositionDiference);												//Update the position (vec3)
	void incrementZoomEffect(float ZAxis) { Position._Z += ZAxis; };								//Uptade the Z axis, creating a zoom effect
	bool isAddaptModeUV(void) { return AddaptRatioUVMode; };										//Return true if the addaptation mode is UV, and false if is normal
	bool isPanelBuilt(void) { return is_panelBuilt; };												//Return the status of the panel. If false, the panel is not ready for reading (possible image path input mismatch)
	bool isPanelFlipped(void) { return is_flipped; };												//Return the flip status of the panel (if is flipped or not)
	bool isPanelHidden(void) { return is_hidden; };													//Return true if the panel is hidden, false for visible
	bool isTextureAlpha(void) { return is_textureAlpha; };											//Return true if the texture loaded contains an alpha channel
	bool isUVCustom(void) { return is_UVDataCustom; };												//Return true if the UV data is manual (custom) or full image)
	void loadPanelImage(char *ImagePath);															//Load a image on the single side of the panel
	void loadPanelImage(char *ImagePath, uint8 Side);												//Load a image for a specific side
	void loadPanelImage(char *ImagePath1, char *ImagePath2);										//Load 2 images for each side of the panel
	void preSetPanel(uint8 Dimension, uint8 Faces, uint8 Normal, uint8 Behavior);					//Set the basics values. Should be called before drawing
	void reloadPanelImage(char *ImagePath);															//Unload and load a image on the single side of the panel
	void reloadPanelImage(char *ImagePath, uint8 Side);												//Unload and load a image for a specific side
	void reloadPanelImage(char *ImagePath1, char *ImagePath2);										//Unload and load a image for each side of the panel
	void setClickAware(bool EnableClick) { is_clickActive = EnableClick; };							//Set the click hit-test on the panel
	void setDimension(uint8 Dimension);																//Set the dimension of the panel (for calculations)
	void setHidden(bool Hide) { is_hidden = Hide; };												//Hides the panel
	void setNormal(uint8 Normal);																	//Set the normals on or off
	void setOrientation(int8 Orientation1, int8 Orientation2);										//Set the orientation of both sides, even if the panel is single sided
	void setPanelBackColor(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha);						//Set the panel back color. Useful for blending and "filter effects"
	void setPanelBackColor(uint8 ColorArray[4]);													//Set the panel back color. Useful for blending and "filter effects" (array)
	void setPosition(float X, float Y, float Z);													//Set the panel global position
	void setPosition(mwVec3f &Position);															//Set the panel global position (vec3)
	void setRatioAddaptMode(uint8 AddaptMode);														//Set the panel ratio addaptation mode
	void setRatioMaster(uint8 Side);																//Set the ratio control reference (master)
	void setRatios(float X, float Y) { XRatio = X; YRatio = Y; };									//Set the panel ratios manually
	void setRatioBehavior(uint8 RatioBehavior);														//Set the ratio addaptation behavior
	void setRotation(float Yaw, float Roll, float Pitch);											//Set the panel global rotation
	void setRotation(mwVec3f &Rotation);															//Set the panel global rotation (vec3)
	void setRotationAxis(float Yaw, float Roll, float Pitch);										//Set the panel rotation axis
	void setRotationAxis(mwVec3f &Axis);															//Set the panel rotation axis (vec3)
	void setSize(mwVec3f &Point1, mwVec3f &Point2);													//Set the panel size (vec3)
	void setSize(uint16 XScale, uint16 YScale);														//Set the panel size
	void setTexturePointer(uint8 TexPointer, uint8 Side);											//Sets a new pointer to use as texture
	//void setThreadControllerPointer(ThreadController* TGT);											//Sets the thread controller instance pointer
	void setUVMap(mwVec2f &UVA, mwVec2f &UVB);														//Set the UV the panel will use to map the texture (vec2)
	void setUVMap(float UVAX, float UVAY, float UVBX, float UVBY);									//Set the UV the panel will use to map the texture
	void setUVMode(uint8 UVMode);																	//Set the current UV mode
	void setVisibleFaces(uint8 Faces);																//Define if the panel will be single ou double sided
	void setZoomEffect(float ZAxis) { Position._Z = ZAxis; };										//Set the Z axis, creating a zoom effect
	void switchHide(void);																			//Switches the panel hide or show states
	void switchRatioAddaptMode(void);																//Switches the ratio addaptation mode
	void switchRatioBehavior(void);																	//Switches the current behavior
	void switchRatioMaster(void);																	//Switches the ratio control reference (master)
	void switchUVMode(void);																		//Switches the UV state (Manual / Full image)
	void unloadPanelImage(void);																	//Unload all the images from the panel's texture data

protected:
	uint8 RatioMaster = 0;					//Defines wich texture will control the panel ratio. [0] for the front one, [1] for the back one
	uint8 PanelOrientation[2];				//Orientation of the panel. Slot [0] for front side and [1] for back
	uint8 PanelBackColor[4];				//Panel Background Color. Default color is white (255, 255, 255, 255)
	bool KeepRatio = true;					//Adapt the ratio for non power of 2 sprite sheets? False to addapt, true to keep
	bool AddaptRatioUVMode = false;			//If true, the ratio will addapt following the UV rules (Only change X+ and Y+). If false (default), X and Y will be changed both on X- / X+ and Y- / Y+
	bool have_normals = false;				//The panel have normals?
	bool is_child = false;					//If this panel is a copy of another, then set this to true
	bool is_flipped = false;				//Flip the panel?
	bool is_panel2D = true;					//Is the panel in 2D or 3D space? False for 3D, true for 2D
	bool is_panelSingleFace = true;			//The ammount of faces that will be drawn (One side or both sides). False for Double, true for single
	bool is_panelBuilt = false;				//Panel construction was OK?
	bool is_hidden = false;					//Panel is visible?
	bool is_clickActive = false;			//Panel should detected when clicked?
	bool is_normalsBuilt = false;			//Normals data was built?
	bool is_textured = false;				//The panel have a texture or is just blank?
	bool is_textureAlpha = false;			//The texture data have alpha?
	bool is_UVDataCustom = false;			//The texture will be manually mapped? (Atlas / Manual Mapping)
	uint16 NormalAmount;					//The amount of normals requested
	uint16 TextureWidth[2];					//The texture width
	uint16 TextureHeigth[2];				//The texture heigth
	char TextureResolution[2][16];			//The texture resolution
	char TextureRatio[2][16];				//The texture ratio
	float XRatio;							//The X ratio (e.g.: 16X / 9)
	float YRatio;							//The Y ratio (e.g.: 16 / 9Y)
	float Orient[4][4][2];					//UV Data holder
	uint32 Texture[2];						//Texture data. Slot [0] for front side and [1] for back
	//ThreadController* ExtThrdCtrl = NULL;	//External Thread Controller Pointer, to allow multi-thread reading on the ImageLoader. If != NULL, the pointer ill be sent
	mwVec2f UVA;							//UV Coordinates. Represent the bottom left of the image (min)
	mwVec2f UVB;							//UV Coordinates. Represent the top right of the image (max)
	mwVec3f PointA;							//Max position of the panel (left upper vertex)
	mwVec3f PointB;							//Min position of the panel (rigth bottom vertex)
	mwVec3f Position;						//The panel global position
	mwVec3f Rotation;						//The Panel's rotation. X -> Yaw | Y -> Roll |	Z -> Pitch
	mwVec3f RotationAxis;					//The Panel's rotation axis. X -> Yaw | Y -> Roll |	Z -> Pitch
	mwVec3f *PanelNormals;					//The normals, for lightning effects

	void allocNormals(void);
	void buildNormalData(void);
	void computeRatio(uint8 Side);
	void purgeNormalsData(void);
	void resetValues(void);
	void setOrientToCustom(void);
	void setOrientToDefault(void);
};

#endif;