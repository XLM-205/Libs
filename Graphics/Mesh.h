/*-------Moon Wiz Mesh-------
*	--WIP
*
*	Copyright (c) 2018 Moon Wiz Studios
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version ?.?.?
*/

#ifndef _H_MWMESH_
#define _H_MWMESH_

#include "..\Maths\Vec3d.h"

enum MeshBuildCoords
{
	MESH_BUILD_XY,	//Default
	MESH_BUILD_XZ,
	MESH_BUILD_YZ
};

class Mesh
{
private:
	mwVec3d **m_meshVertexes;
	mwVec3d m_globalTrans;
	mwVec3d m_globalRot;
	uint32 m_width;
	uint32 m_height;

	void (Mesh::*BuildFunc)(mwVec3d*, double, double) = nullptr;

	void AssignBuildFunc(MeshBuildCoords Coords)
	{
		switch(Coords)
		{
			case MESH_BUILD_XY:
			default:
				BuildFunc = &Mesh::BuildXY;
				break;
			case MESH_BUILD_XZ:
				BuildFunc = &Mesh::BuildXZ;
				break;
			case MESH_BUILD_YZ:
				BuildFunc = &Mesh::BuildYZ;
				break;
		}
	}
	void BuildXY(mwVec3d *vertex, double A, double B)
	{
		vertex->setValues(A, B, 0);
	}
	void BuildXZ(mwVec3d *vertex, double A, double B)
	{
		vertex->setValues(A, 0, B);
	}
	void BuildYZ(mwVec3d *vertex, double A, double B)
	{
		vertex->setValues(0, A, B);
	}
	bool isValid(uint32 i, uint32 j)
	{
		return i < m_width && j < m_height;
	}

public:
	Mesh(uint32 size, double spacing, MeshBuildCoords Coords) : m_globalRot(), m_globalTrans()
	{
		AssignBuildFunc(Coords);
		size ? m_width = m_height = size : m_width = m_height = 1;
		m_meshVertexes = new mwVec3d*[m_width];
		for(uint32 i = 0; i < m_width; i++)
		{
			double iPos = i * spacing;
			m_meshVertexes[i] = new mwVec3d[m_height];
			for(uint32 j = 0; j < m_height; j++)
			{
				(this->*BuildFunc)(&m_meshVertexes[i][j], iPos, j * spacing);
			}
		}
	}

	~Mesh()
	{
		if(m_meshVertexes)
		{
			for(uint32 i = 0; i < m_width; i++)
			{
				delete[] m_meshVertexes[i];
				m_meshVertexes[i] = nullptr;
			}
			delete[] m_meshVertexes;
			m_meshVertexes = nullptr;
		}
	}

	uint32 getWidth(void)
	{
		return m_width;
	}
	uint32 getHeight(void)
	{
		return m_height;
	}

	void setVertex(mwVec3d &vertex, uint32 i, uint32 j)
	{
		if(isValid(i, j))
		{
			m_meshVertexes[i][j] = vertex;
		}
	}
	mwVec3d& getVertex(uint32 i, uint32 j)
	{
		if(isValid(i, j))
		{
			return m_meshVertexes[i][j];
		}
		return m_meshVertexes[0][0];
	}

	void setVertexX(double X, uint32 i, uint32 j)
	{
		if(isValid(i, j))
		{
			m_meshVertexes[i][j]._X = X;
		}
	}
	void setVertexY(double Y, uint32 i, uint32 j)
	{
		if(isValid(i, j))
		{
			m_meshVertexes[i][j]._Y = Y;
		}
	}
	void setVertexZ(double Z, uint32 i, uint32 j)
	{
		if(isValid(i, j))
		{
			m_meshVertexes[i][j]._Z = Z;
		}
	}

	mwVec3d getRotation(void)
	{
		return m_globalRot;
	}
	mwVec3d getTranslation(void)
	{
		return m_globalTrans;
	}
	void setRotation(mwVec3d &rot)
	{
		m_globalRot = rot;
	}
	void setTranslation(mwVec3d &trans)
	{
		m_globalTrans = trans;
	}
};

#endif
