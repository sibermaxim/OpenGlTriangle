////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "cameraclass.h"

#include <glm/gtc/matrix_transform.hpp>

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
	angle = 0;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

void CameraClass::Render()
{
	Vector3 m_xAxis;
	Vector3 m_yAxis;
	Vector3 m_zAxis;
	Vector3 m_eye(0, 0, 0);

	glm::vec3 center = glm::vec3(0, 0, -10), eye = glm::vec3(0.0f, 0.0f, 1.0f), up = glm::vec3(0, 1, 0);
	glm::mat4 tmp = glm::lookAt(eye, center, up);

	m_translMatrix = glm::mat4(1.0);
	m_rotateMatrix = glm::mat4(1.0);

	/*Vector3 m_target(m_positionX, m_positionY, m_positionZ);

	m_zAxis = m_eye - m_target;
	m_zAxis.normalize();

	Vector3 Up(0, 1, 0);
	m_xAxis = Vector3::cross(Up, m_zAxis);
	m_xAxis.normalize();

	m_yAxis = Vector3::cross(m_zAxis, m_xAxis);
	m_yAxis.normalize();
	m_xAxis.normalize();

	m_viewMatrix[0] = m_xAxis.x;
	m_viewMatrix[1] = m_xAxis.y;
	m_viewMatrix[2] = m_xAxis.z;
	m_viewMatrix[3] = -Vector3::dot(m_xAxis, m_eye);;

	m_viewMatrix[4] = m_yAxis.x;
	m_viewMatrix[5] = m_yAxis.y;
	m_viewMatrix[6] = m_yAxis.z;
	m_viewMatrix[7] = -Vector3::dot(m_yAxis, m_eye);;

	m_viewMatrix[8] = m_zAxis.x;
	m_viewMatrix[9] = m_zAxis.y;
	m_viewMatrix[10] = m_zAxis.z;
	m_viewMatrix[11] = -Vector3::dot(m_zAxis, m_eye);

	m_Translate.set(-m_positionX, -m_positionY, -m_positionZ);
	m_viewMatrix[12] = m_Translate.x;
	m_viewMatrix[13] = m_Translate.y;
	m_viewMatrix[14] = m_Translate.z;
	m_viewMatrix[15] = 1.0f;*/

	for(int i=0; i<4; i++)
	{
		for (int j = 0; j < 4; j++)
			m_viewMatrix[j + i * 4] = tmp[i][j];
	}
}

void CameraClass::MatrixRotationYawPitchRoll(float* matrix, float yaw, float pitch, float roll)
{
	float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;


	// Get the cosine and sin of the yaw, pitch, and roll.
	cYaw = cosf(yaw);
	cPitch = cosf(pitch);
	cRoll = cosf(roll);

	sYaw = sinf(yaw);
	sPitch = sinf(pitch);
	sRoll = sinf(roll);

	// Calculate the yaw, pitch, roll rotation matrix.
	matrix[0] = (cRoll * cYaw) + (sRoll * sPitch * sYaw);
	matrix[1] = (sRoll * cPitch);
	matrix[2] = (cRoll * -sYaw) + (sRoll * sPitch * cYaw);

	matrix[3] = (-sRoll * cYaw) + (cRoll * sPitch * sYaw);
	matrix[4] = (cRoll * cPitch);
	matrix[5] = (sRoll * sYaw) + (cRoll * sPitch * cYaw);

	matrix[6] = (cPitch * sYaw);
	matrix[7] = -sPitch;
	matrix[8] = (cPitch * cYaw);

	return;
}

void CameraClass::TransformCoord(VectorType& vector, float* matrix)
{
	float x, y, z;


	// Transform the vector by the 3x3 matrix.
	x = (vector.x * matrix[0]) + (vector.y * matrix[3]) + (vector.z * matrix[6]);
	y = (vector.x * matrix[1]) + (vector.y * matrix[4]) + (vector.z * matrix[7]);
	z = (vector.x * matrix[2]) + (vector.y * matrix[5]) + (vector.z * matrix[8]);

	// Store the result in the reference.
	vector.x = x;
	vector.y = y;
	vector.z = z;
}

void CameraClass::BuildViewMatrix(VectorType position, VectorType lookAt, VectorType up)
{
	VectorType zAxis, xAxis, yAxis;
	float length, result1, result2, result3;


	// zAxis = normal(lookAt - position)
	zAxis.x = lookAt.x - position.x;
	zAxis.y = lookAt.y - position.y;
	zAxis.z = lookAt.z - position.z;
	length = sqrt((zAxis.x * zAxis.x) + (zAxis.y * zAxis.y) + (zAxis.z * zAxis.z));
	zAxis.x = zAxis.x / length;
	zAxis.y = zAxis.y / length;
	zAxis.z = zAxis.z / length;

	// xAxis = normal(cross(up, zAxis))
	xAxis.x = (up.y * zAxis.z) - (up.z * zAxis.y);
	xAxis.y = (up.z * zAxis.x) - (up.x * zAxis.z);
	xAxis.z = (up.x * zAxis.y) - (up.y * zAxis.x);
	length = sqrt((xAxis.x * xAxis.x) + (xAxis.y * xAxis.y) + (xAxis.z * xAxis.z));
	xAxis.x = xAxis.x / length;
	xAxis.y = xAxis.y / length;
	xAxis.z = xAxis.z / length;

	// yAxis = cross(zAxis, xAxis)
	yAxis.x = (zAxis.y * xAxis.z) - (zAxis.z * xAxis.y);
	yAxis.y = (zAxis.z * xAxis.x) - (zAxis.x * xAxis.z);
	yAxis.z = (zAxis.x * xAxis.y) - (zAxis.y * xAxis.x);

	// -dot(xAxis, position)
	result1 = ((xAxis.x * position.x) + (xAxis.y * position.y) + (xAxis.z * position.z)) * -1.0f;

	// -dot(yaxis, eye)
	result2 = ((yAxis.x * position.x) + (yAxis.y * position.y) + (yAxis.z * position.z)) * -1.0f;

	// -dot(zaxis, eye)
	result3 = ((zAxis.x * position.x) + (zAxis.y * position.y) + (zAxis.z * position.z)) * -1.0f;

	// Set the computed values in the view matrix.
	m_viewMatrix[0] = xAxis.x;
	m_viewMatrix[1] = yAxis.x;
	m_viewMatrix[2] = zAxis.x;
	m_viewMatrix[3] = 0.0f;

	m_viewMatrix[4] = xAxis.y;
	m_viewMatrix[5] = yAxis.y;
	m_viewMatrix[6] = zAxis.y;
	m_viewMatrix[7] = 0.0f;

	m_viewMatrix[8] = xAxis.z;
	m_viewMatrix[9] = yAxis.z;
	m_viewMatrix[10] = zAxis.z;
	m_viewMatrix[11] = 0.0f;

	m_viewMatrix[12] = result1;
	m_viewMatrix[13] = result2;
	m_viewMatrix[14] = result3;
	m_viewMatrix[15] = 1.0f;

	for(int i=0; i<3; i++)
	{
		for (int j = 0; j < 3; j++)
			m_Rotate[i][j] = m_viewMatrix[j + 4 * i];
	}
}

void CameraClass::BuildNewViewMatrix()
{
	m_viewMatrix[12] = m_Translate.x;
	m_viewMatrix[13] = m_Translate.y;
	m_viewMatrix[14] = m_Translate.z;
}

void CameraClass::Update(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	Vector3 xlate;

	switch(nChar)
	{
	case 'l':
		y -= 3.0f*0.002;
		break;

	case 'j':
		y += 3.0f*0.002;
		break;
	}
	xlate.set(x, y, z);
	m_Translate += xlate;

}

void CameraClass::UpdateMove(float xx, float yy, float zz)
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	Vector3 xlate;

	x += xx - oldX;
	y += yy - oldY;
//	x += xx;

	xlate.set(x, y, z);
	m_Translate += xlate;

//	oldX = xx;
//	oldY = yy;
}

void CameraClass::MouseLDown(float xx, float yy, float zz)
{
	oldX = xx;
	oldY = yy;
	oldZ = zz;

	/*Vector3 yym(0, 1, 0);

	//Matrix4 m(m_viewMatrix[0], m_viewMatrix[4], m_viewMatrix[8], m_viewMatrix[12],
	//		  m_viewMatrix[1], m_viewMatrix[5], m_viewMatrix[9], m_viewMatrix[13],
	//		m_viewMatrix[2], m_viewMatrix[6], m_viewMatrix[10], m_viewMatrix[14],
	//		m_viewMatrix[3], m_viewMatrix[7], m_viewMatrix[11], m_viewMatrix[15]);

	angle += 0.001;
//	m.rotate(yym, angle);
	MatrixRotationYawPitchRoll(m_viewMatrix,  0,angle, 0);
	/*for(int i=0; i<4; i++)
	{
		for (int j = 0; j < 4; j++)
			m_viewMatrix[j + i * 4] = m[i][j];
	}*/
}

void CameraClass::GetViewMatrix(float* matrix)
{
	matrix[0] = m_viewMatrix[0];
	matrix[1] = m_viewMatrix[1];
	matrix[2] = m_viewMatrix[2];
	matrix[3] = m_viewMatrix[3];

	matrix[4] = m_viewMatrix[4];
	matrix[5] = m_viewMatrix[5];
	matrix[6] = m_viewMatrix[6];
	matrix[7] = m_viewMatrix[7];

	matrix[8] = m_viewMatrix[8];
	matrix[9] = m_viewMatrix[9];
	matrix[10] = m_viewMatrix[10];
	matrix[11] = m_viewMatrix[11];

	matrix[12] = m_viewMatrix[12];
	matrix[13] = m_viewMatrix[13];
	matrix[14] = m_viewMatrix[14];
	matrix[15] = m_viewMatrix[15];

	return;
}
