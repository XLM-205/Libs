#ifndef CAMERA_H_
#define CAMERA_H_

#define CAMERA_3D_MODE 1
#define CAMERA_2D_MODE 2

#include "CustomTypes-SA.h"

class Camera
{
public:
	Camera(uint8 Type, float PosX, float PosY, float PosZ);
	float getCamX(void) { return CamX; }
	float getCamY(void) { return CamY; }
	float getCamZ(void) { return CamZ; }
protected:
	float CamX;			//Camera's X position
	float CamY;			//Camera's Y position
	float CamZ;			//Camera's Z position
	bool is_3D = false;	//Defines the camera mode. True for 3D, false for 2D
};

#endif;	//CAMERA_H_