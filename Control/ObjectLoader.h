/*-------Moon Wiz Object Loader-------
*	Custom Object loader
*	--.obj and .mtl reading, drawing, transformations, hit-box analyser, independent texture and .mtl override, multi-textured object handling
*		--Require "CustomTypes.h" for custom variables definitions
*		--Require "Vec3d.h" for vertex, normals and UV data handling
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
*	Version 0.8.3
*/

/*	-Version History
*	-Last updated: 01/06/2015-
*	0.8		-> Started version tracking
*	0.8.1	-> Set unload method for cleaning memory
*	0.8.3	-> Improved memory consumption and releasing on base classes (ImgLoader)
*
*/

#ifndef _H_MWOBJLOADER_
#define _H_MWOBJLOADER_
//MWOL a.k.a MWOBJLOADER = Moon Wiz Object Loader

#define MWOL_VERSION	83						//The actual version of the loading engine

#include <stdio.h>

//TODO: Physics Core Integration. If Core.h is included, comment Vec3d.h
#include "..\Maths\Vec3d.h"
//#include "..\Maths\Core.h"
#include "CustomTypes-SA.h"
#include "FileStates.h"

//File reading format (required for file checking)
#define MWOL_FORMAT_OBJ				  "obj"
//Reading modes
#define MWOL_READ_SIMPLE				1		//(Default) Just read the .obj, complimentary .mtl and textures
#define MWOL_READ_SAFE					2		//(Recommended) Try to fix bad formated .mtl first, then read it normally. If the file was fixed before, the engine will use "READ_SIMPLE"
//Draw modes
#define MWOL_DRAW_WIREFRAME				1		//Object will be drawn in wireframe mode
#define MWOL_DRAW_SOLID					2		//Object will be drawn without texture
#define MWOL_DRAW_TEXTURED				3		//Object will be drawn with texture
#define MWOL_DRAW_SOLID_HIGHLIGHT		4		//Object will be drawn without texture, highligthing it's vertexes lines
#define MWOL_DRAW_TEXTURED_HIGHLIGHT	5		//Object will be drawn with texture, highligthing it's vertexes lines
#define MWOL_DRAW_SOLID_QUAD			6		//Object will be drawn without texture using quads as primitives
#define MWOL_DRAW_TEXTURED_QUAD			7		//Object will be drawn with texture using quads as primitives
//Panel Drawing modes
#define MWOL_PANEL_ONE_SIDED			1		//Panel will only have one side (2 Triangles forming 1 quad)
#define MWOL_PANEL_TWO_SIDED			2		//Panel will have two sides (4 Triangles forming 2 quad)
//Warnings
#define MWOL_FILE_SUCESS				1		//File FOUND and READ
#define MWOL_ERROR_NO_PATH			   -1		//NO file path/name given
#define MWOL_ERROR_NO_FILE			   -2		//NO file found/read with given file name
#define MWOL_ERROR_INVALID_FILE		   -3		//File is NOT SUPPORTED/NOT a valid OBJ file
#define MWOL_WARN_NOT_LOADED		   -4		//Object was instantiated manually but NOT loaded
//File Message Warnings
#define MWOL_WR_FILE_SUCCESS		"FILE SUCCESSFULLY READ"			//<File successfully read>. Written form of "MWOL_FILE_SUCESS" state
#define MWOL_WR_WARN_NOT_LOADED		"WARNING! OBJECT DATA NOT LOADED"	//<Warning! Object data not loaded>. Written form of "MWOL_WARN_NOT_LOADED" state
#define	MWOL_WR_ERROR_NO_PATH		"ERROR! NO PATH GIVEN"				//<Error! No path given>. Written form of "MWOL_ERROR_NO_PATH" state
#define MWOL_WR_ERROR_NO_FILE		"ERROR! NO FILE FOUND"				//<Error! No file found>. Written form of " MWOL_ERROR_NO_FILE" state
#define MWOL_WR_ERROR_INVALID_PATH	"ERROR! INVALID/UNREADBLE FILE"		//<Error! Invalid/Unreadble file>. Written form of "MWOL_ERROR_INVALID_FILE" state
//Specific definitions (undef on end of library files)
#define MAX_BUFFER_SIZE				   256		//Max size reading buffer
#define MAX_LEN_FILEPATH			   256		//Max filepath length
#define MAX_NAME					   64		//Max name length
#define DEFAULT_ILLUM_MODEL			   1		//Default illumination model for the .mtl file. If the data wasn't found, use this instead

//-----------------------------------MTL Illum reference----------------------------------
//	0. Color on and Ambient off															//
//	1. Color on and Ambient on															//
//	2. Highlight on																		//
//	3. Reflection on and Ray trace on													//
//	4. Transparency: Glass on, Reflection: Ray trace on									//
//	5. Reflection: Fresnel on and Ray trace on											//
//	6. Transparency: Refraction on, Reflection: Fresnel off and Ray trace on			//
//	7. Transparency: Refraction on, Reflection: Fresnel on and Ray trace on				//
//	8. Reflection on and Ray trace off													//
//	9. Transparency: Glass on, Reflection: Ray trace off								//
//	10. Casts shadows onto invisible surfaces											//
//----------------------------------------------------------------------------------------
//For extra reference: http://en.wikipedia.org/wiki/Wavefront_.obj_file

//Pre-configurations are set here first
void ObjLoaderPreConfig(uint8 ReadingMode);
void ObjLoaderHighLigthColor(uint8 Red, uint8 Green, uint8 Blue);
void ObjLoaderBoundingBoxColor(uint8 Red, uint8 Green, uint8 Blue);
void ObjLoaderNormalColor(uint8 Red, uint8 Green, uint8 Blue);

class ObjLoader : public FileStates
{
protected:
	//Total memory usage: 232 Bytes. 58 chuncks of 4 bytes on x32 (x86) systems. No padding required
	struct ObjGroup
	{
		char GroupName[MAX_NAME];									//The group's name. Referenced in .obj (o <name>)
		char MaterialReference[MAX_NAME];							//The material reference that will be used from the .mtl file
		uint16 ID;													//Group numeric ID. The first group SHOULD NOT be 0. 0 is reserved for NULL groups
		uint16 MaterialID;											//ID to search when pulling data for the renderer
		uint32 RangeStart;											//When the face data will begin to be read
		uint32 RangeEnd;											//When the face data will end to be read
		mwVec3f GrpRotationAxis;									//Axis where the rotation will start from
		mwVec3f GrpRotation;										//The group's rotation. X -> Yaw | Y -> Roll |	Z -> Pitch
		mwVec3f GrpPosition;										//The group's position
		mwVec3f GrpBBMax;											//Group's Bounding Box Max
		mwVec3f GrpBBMin;											//Group's Bounding Box Min
		mwVec3f GrpCenter;											//Group's center. This is also the Group position

		ObjGroup(void)													//Preparing the max and min
		{
			GrpBBMax = -99999;
			GrpBBMin = 99999;
			ID = MaterialID = RangeStart = RangeEnd = 0;
			MaterialReference[0] = '\0';
			GroupName[0] = 'N';
			GroupName[1] = 'U';
			GroupName[2] = 'L';
			GroupName[3] = 'L';
			GroupName[4] = '\0';

		}
		ObjGroup(int id, const char *name)
		{
			ID = id;
			strcpy(GroupName, name);
			GrpBBMax = -99999;
			GrpBBMin = 99999;
			ID = MaterialID = RangeStart = RangeEnd = 0;
			MaterialReference[0] = '\0';
		}
		void resetMaxMin(void)
		{
			GrpBBMax = -99999;
			GrpBBMin = 99999;
		}
	};
	//Total memory usage: 692 Bytes (689 unpadded). 173 chuncks of 4 bytes on x32 (x86) systems. 3 bytes padding
	struct MaterialData
	{
		//NOTE: A full .mtl file contains much more variables. Add then as they appear
		//NOTE 2: "illum" put last so the padding will be done in the last memory block
		uint16 ID;													//Group numeric ID. The first material SHOULD NOT be 0. 0 is reserved for NULL materials
		uint16 TextureToGenerate;									//The amount of textures that will be generated. SHOULD NOT be 0. 0 is reserved for NO texture
		uint16 TextureIndex[4];										//The index that define which texture to use for this material. If the .mtl is correct, then this index is for actual texture, and the last for technical (bump/opacity)
		float Ka[3];												//a.k.a the ambient color
		float Kd[3];												//a.k.a the diffuse color
		float Ks[3];												//a.k.a the specular color
		float AmbientMaterial[4];									//Array that holds the Ka + d (Ambient color + alpha) to OpenGL
		float DiffuseMaterial[4];									//Array that holds the Kd + d (Diffuse color + alpha) to OpenGL
		float SpecularMaterial[4];									//Array that holds the Ks + d (Specular color + alpha) to OpenGL
		float Ns;													//a.k.a the specular exponent (or shininess)
		float Ni;													//Specifies the optical density for the surface
		float d;													//a.k.a the dissolve factor (alpha). Some implementations may use "Tr" instead
		float refl;													//a.k.a Reflection in .mtl
		char MaterialName[MAX_NAME];								//The material name
		char KaMap[MAX_NAME];										//The ambient texture map
		char KdMap[MAX_NAME];										//The diffuse texture map
		char KsMap[MAX_NAME];										//The specular color texture map
		char NsMap[MAX_NAME];										//The specular highlight component
		char dMap[MAX_NAME];										//The alpha texture map
		char bumpMap[MAX_NAME];										//The bump map (which by default uses the luminance channel of the image)
		char dispMap[MAX_NAME];										//The displacement map
		char decalMap[MAX_NAME];									//A stencil decal texture
		int8 illum;													//How the light will work (Illumination Model). Range: 0~10. See reference above

		MaterialData(void)
		{
			ID = 0;
			TextureToGenerate = 0;
			Ns = Ni = d = refl = 0;
			illum = DEFAULT_ILLUM_MODEL;
			//Seting the first value to NULL, to prevent garbage on (at least) the debugger
			KaMap[0] = '\0';
			KdMap[0] = '\0';
			KsMap[0] = '\0';
			NsMap[0] = '\0';
			dMap[0] = '\0';
			bumpMap[0] = '\0';
			dispMap[0] = '\0';
			decalMap[0] = '\0';
			MaterialName[0] = '\0';
		}
	};

	void setBB(float x, float y, float z, ObjGroup *group);
	void parseFaceX(List<uint32> *vertexIndexes, List<uint32> *unused, List<uint32> *unused2);				//'unused' so we can use function pointers to not use a if inside a loop everytime
	void parseFaceXY(List<uint32> *vertexIndexes, List<uint32> *uvIndexes, List<uint32> *unused2);
	void parseFaceXYZ(List<uint32> *vertexIndexes, List<uint32> *uvIndexes, List<uint32> *normalIndexes);
	void parseFaceXZ(List<uint32> *vertexIndexes, List<uint32> *normalIndexes, List<uint32> *unused);
public:	
	ObjLoader(void);																		//Don't construct, only prepares (Used for global object. MUST USE "load(ObjPath)" method)
	ObjLoader(char *Filename);																//Constructs, loading a object from a file
	ObjLoader(char *Filename, char *Texture);												//Constructs, loading a object but gives the choice to override the .mtl data, if any, for a single texture (May receive a .mtl as input and will be used (if compatible))
	~ObjLoader(void);
	mwVec3f* getVertData(void){ return VertexData; };										//Return a array of Vec3 that contains the vertex data
	mwVec3f getObjectBBMax(void){ return ObjBBMax; };										//Return the Object's Max Bounding Box values
	mwVec3f getObjectBBMin(void){ return ObjBBMax; };										//Return the Object's Min Bounding Box values
	mwVec3f getObjectBBMaxPosition(void){return ObjBBMax + ObjPosition;};					//Return the Object's Max Bounding Box positions
	mwVec3f getObjectBBMinPosition(void){return ObjBBMin + ObjPosition;};					//Return the Object's Min Bounding Box positions
	mwVec3f getGroupBBMax(uint16 GroupIndex){ return Groups[GroupIndex].GrpBBMax; };		//Return the Group's Max Bounding Box values
	mwVec3f getGroupBBMin(uint16 GroupIndex){ return Groups[GroupIndex].GrpBBMin; };		//Return the Group's Min Bounding Box values
	mwVec3f getObjectCenter(void){ return ObjCenter; };										//Return the object center (Vec3)
	mwVec3f getGroupCenter(uint16 GroupIndex){ return Groups[GroupIndex].GrpCenter; };		//Return the group center (Vec3)
	mwVec3f getObjectPosition(void){ return ObjPosition; };									//Return the Object position (Vec3)
	mwVec3f getObjectRotation(void){ return ObjRotation; };									//Return the Object rotation (Vec3)
	mwVec3f getGroupPosition(uint16 GroupIndex){ return Groups[GroupIndex].GrpPosition; };	//Return the Group position (Vec3)
	mwVec3f getGroupRotation(uint16 GroupIndex){ return Groups[GroupIndex].GrpRotation; };	//Return the Group rotation (Vec3)
	bool haveGroups(void){ return have_groups; };											//The object have groups?
	bool isPointInsideBB(mwVec3f Point);													//Returns true if 'Point' is inside the central Bounding Box of the object
	bool isPointInsideBBGroup(mwVec3f Point, uint16 GroupIndex);							//Returns true if 'Point' is inside the central Bounding Box of the object
	int getMaterialAmount(void){ return MaterialAmount; };									//Return the amount of materials this object have
	int getVertAmount(void){ return VertexAmount; };										//Return the amount of vertexes
	int getFaceAmount(void){ return FacesAmount; };											//Return the amount of faces
	int getGroupAmount(void){ return GroupAmount; };										//Return the amount of groups
	char* getGroupName(uint16 Index){ return Groups[Index].GroupName; };					//Return the name of the group
	MaterialData* getMaterialData(void){ return Materials; };								//Return all the material data
	void DrawLoadedObject(uint8 DrawMode, float Scale);										//Draw the object, using the values loaded
	void DrawLoadedObject(uint8 DrawMode, float Scale, uint16 GroupIndex);					//Draw the object, using the values loaded, but only using the group pointed by the "Index"
	void DrawLoadedBoundingBox(void);														//Draw the object's bounding box
	void DrawLoadedBoundingBox(uint16 GroupIndex);											//Draw the object group's bounding box
	void DrawLoadedNormals(void);															//Draw the object normals
	void DrawLoadedNormals(uint16 GroupIndex);												//Draw the object group's normals
	void DrawObject(mwVec3f &VertData, uint16 VertAmount, uint8 DrawMode, float Scale);		//Draw the object, supplied with the vertex data (Vec3 data) [WIP]
	void DrawObject(mwVec2f &VertData,uint16 VertAmount, uint8 DrawMode, float Scale);		//Draw the object, supplied with the vertex data (Vec2 data) [WIP]
	void DrawObject(float VertData[],uint16 VertAmount, uint8 DrawMode, float Scale);		//Draw the object, supplied with the vertex data (Custom data) [WIP]
	void loadObject(char *Filename);														//Loads a .obj file manually. Should only be called when the object was NULL constructed
	void loadObject(char *Filename, char *Texture);											//Loads a .obj manually, but gives the choice to override the .mtl data, if any, for a single texture  (May receive a .mtl as input and will be used (if compatible))
	void unloadObject(void);																//Unload all the data, keeping this object instance (NOT DESTRUCTOR)
	void replaceObject(char *Filename);														//Unload all the data, and immediately load another
	void replaceObject(char *Filename, char *Texture);										//Unload all the data, and immediately load another overwriting the texture
	void setGroupPosition(uint16 GroupIndex, mwVec3f &Position);							//Sets a group specific position (Vec3 data)
	void setGroupRotation(uint16 GroupIndex, mwVec3f &Rotation, mwVec3f &Axis);				//Sets a group specific rotation (Vec3 data)
	void setGroupPosition(uint16 GroupIndex, float Position[]);								//Sets a group specific position (Array data)
	void setGroupRotation(uint16 GroupIndex, float Rotation[], float Axis[]);				//Sets a group specific rotation (Array data)
	void setObjectPosition(mwVec3f &Position);												//Sets the object specific position (Vec3 data)
	void setObjectRotation(mwVec3f &Rotation, mwVec3f &Axis);								//Sets the object specific rotation (Vec3 data)
	void setObjectPosition(float Position[]);												//Sets the object specific position (Array data)
	void setObjectRotation(float Rotation[], float Axis[]);									//Sets the object specific rotation (Array data)
private:
	char OverrideTexturePath[MAX_LEN_FILEPATH];						//The override texture path
	char FilePath[MAX_LEN_FILEPATH];								//File's path
	char DirectoryPath[MAX_LEN_FILEPATH];							//The file's directory with the last '\'. With this, we can speed up .mtl reading
	char MaterialFilename[MAX_NAME];								//The file's material filename
	char ObjectName[MAX_NAME];										//The object's name. Get from the .obj file (reference o <name>)
	bool TextureOverride = false;									//If the object was constructed with a texture path, ignore it's mtl and use it instead
	bool is_objectBuilt = false;									//Every process and fail check have succeeded, then toogle this to true!
	bool is_quadrilateral = false;									//In the event the face indices contains 4 values...
	bool is_textured = false;										//Object have texture coordinate data?
	bool is_fromFile = false;										//Object data have come from a file?
	bool is_centerComputed = false;									//Object bounding boxes and center was computed?
	bool is_shadingSmooth = false;									//Object Smooth shading is toogle on?
	bool is_mtlSuccess = false;										//Complimentary .mtl was the read?
	bool have_materials = false;									//Object data have a complementary material file (.mtl)?
	bool have_normals = false;										//Object have normal data?
	bool have_groups = false;										//Object is divided in groups?
	bool generated_texture = false;									//The textures have been generated?
	uint8 FaceSize = 0;												//The amount of vertexes needed to make a face. 0 is NULL, 3 is for triangulated faces and 4 is for quadrilaterals
	uint8 Pattern = 0;												//To speed up the indices reading process
	int16 LastLoadedMaterial = 0;									//The last loaded material (to prevent loading the same material multiple times)
	uint32 MaterialAmount;											//The object's have how many materials?
	uint32 TextureAmount;											//The object's groups have how many textures, in total?
	uint32 GroupAmount;												//The object's made with how many groups?
	uint32 FacesAmount;												//The amout of faces
	uint32 AbsoluteFacesAmount;										//To keep track of the indices
	uint32 VertexAmount;											//The amount of vertexes
	uint32 NormalsAmount;											//The amount of normals
	uint32 UVAmount;												//The amount of texture cordinates
	mwVec3f ObjRotationAxis;										//Axis from where the rotation will start from
	mwVec3f ObjRotation;											//The Object's rotation. X -> Yaw | Y -> Roll |	Z -> Pitch
	mwVec3f ObjPosition;											//The object position
	mwVec3f ObjCenter;												//The center of the object
	mwVec3f ObjBBMax;												//Object's Bounding Box Max
	mwVec3f ObjBBMin;												//Object's Bounding Box Min
	ObjGroup *Groups;												//Stores the start, end, postion, rotation and name of the group
	uint32 *VertexIndices;											//Stores the indices for the VertexData array
	uint32 *NormalsIndices;											//Stores the indices for the NormalsData array
	uint32 *UVIndices;												//Stores the indices for the UVData array
	mwVec3f *VertexData;											//The actual vertex data
	mwVec3f *UVData;												//UV a.k.a texture coordinates
	mwVec3f *NormalsData;											//Face Normals
	MaterialData *Materials;										//Stores the material data
	String64 *TextureFilename;										//The textures path. Already checked against duplicates and used to gerenate them
	uint32 *Texture;												//Texture data
	int patternChecker(char Buffer[]);								//Detects the pattern to speed up reading process. Returns the amount of chars on that line (for fseek)
	int countUniqueTextures(uint16 PredictedAmount);				//Counts the amount of unique textures that will be generated in the texture loading pass
	void setData(void);												//Sets all the data from a .obj file
	void setMaterials(void);										//Sets all the materials, using a aditional .mtl file (if declared in .obj)
	void setCenter(float Scale);									//Finds the center of the object and it's groups, if any
	void loadTexture(char *Filename);								//Sets a texture to the object
	void loadTexture(void);											//Same as the first one, but this load the texture from a .mtl file
	void resetMaxMin(void);											//Prepares the max and min values for BB comparision
};

#undef DEFAULT_ILLUM_MODEL
#endif;	//_H_MWOBJLOADER_