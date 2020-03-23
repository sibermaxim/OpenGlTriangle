////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_
#include "mathlib.h"

#include "glm/glm.hpp"

//////////////
// INCLUDES //
//////////////

	struct VectorType
	{
		float x, y, z;
	};

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{

public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void Render();
	void GetViewMatrix(float*);
	void BuildNewViewMatrix();
	void Update(UINT nChar, UINT nRepCnt, UINT nFlags);
	void UpdateMove(float, float, float);
	void MouseLDown(float xx, float yy, float zz);

private:
	
	float oldX, oldY, oldZ;
	
	float angle;

	static void MatrixRotationYawPitchRoll(float*, float, float, float);
	void TransformCoord(VectorType&, float*);
	void BuildViewMatrix(VectorType, VectorType, VectorType);

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_viewMatrix[16];

	glm::mat4 m_translMatrix, m_rotateMatrix;

	Vector3 m_Translate;
	Matrix3 m_Rotate;
};

#endif
