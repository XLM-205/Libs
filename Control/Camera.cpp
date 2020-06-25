#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>

Camera::Camera(uint8 Type, float PosX, float PosY, float PosZ)
{
	CamX = PosX;
	CamY = PosY;
	CamZ = PosZ;
	if (Type == CAMERA_3D_MODE)
	{
		is_3D = true;
	}
	else if (Type == CAMERA_2D_MODE)
	{
		is_3D = false;
	}
	else
	{ 
		printf("INVALID CAMERA MODE!\a\n");
		system("Pause");
	}
}