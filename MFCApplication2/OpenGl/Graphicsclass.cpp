////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "graphicsclass.h"
#include <winuser.h>

#include "glm/ext/matrix_projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

GraphicsClass::GraphicsClass(): 
	m_Model(nullptr), 
	m_ColorShader(nullptr)
{
	m_OpenGL = 0;
	m_Camera = 0;

	tr = glm::vec3(0, 0, 0);
//	m_Model = 0;
//	m_ColorShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other): 
	m_OpenGL(nullptr)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(COgl* OpenGL, HWND hwnd)
{
	bool result;

	// Store a pointer to the OpenGL class object.
	m_OpenGL = OpenGL;

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f,0.0f, 1.0f);
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_OpenGL);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown(m_OpenGL);
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown(m_OpenGL);
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the pointer to the OpenGL class object.
	m_OpenGL = 0;
}


bool GraphicsClass::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::MouseLDown(CPoint point)
{
	VectorType position = {};

	MackeMouseCoordinat(point, position);
	//m_Camera->MouseLDown(position.x, position.y, position.z);

	oldX = position.x;
	oldY = position.y;
	oldZ = position.z;

}

void GraphicsClass::MouseLUp(CPoint point)
{
	tr = tmpTr;
}

void GraphicsClass::MouseMove(CPoint point) 
{
	VectorType position = {};
	float matrix[16];

	MackeMouseCoordinat(point, position);

	//m_Camera->UpdateMove(position.x, position.y, position.z);

	float x = 0.0f, y = 0.0f, z = 0.0f;
	glm::vec3 xlate;
	glm::mat4 tmpMatrix;

	x += position.x -oldX + tr.x;
	y += position.y -oldY + tr.y;
	//	x += xx;
	xlate = glm::vec3(x, y, 0);

	tmpMatrix = glm::mat4(1.0);
	tmpTr = xlate;
	tmpMatrix = glm::translate(tmpMatrix, xlate);

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			matrix[j + i * 4] = tmpMatrix[i][j];
		}
	}

	m_OpenGL->SetWorldMatrix(matrix);
	Render();
}

void GraphicsClass::Update(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_Camera->Update(nChar, nRepCnt, nFlags);
	Render();
}


void GraphicsClass::MackeMouseCoordinat(CPoint point, VectorType & position) const
{
	float projectionMatrix[16];
	float viewMatrix[16];
	float worldMatrix[16];
	GLfloat vx, vy, vz;

	m_OpenGL->GetProjectionMatrix(projectionMatrix);
	m_OpenGL->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);

	/*int i = m_OpenGL->GetScreenWidth();

	position.x = ((2.0f * point.x) / m_OpenGL->GetScreenWidth() - 1) / projectionMatrix[0];
	position.y = -(((2.0f * point.y) / m_OpenGL->GetScreenHeight()) - 1) / projectionMatrix[5];
	position.z = 1.0f;*/

	vx = point.x;
	vy = m_OpenGL->GetScreenHeight() - point.y - 1;
	glReadPixels(vx, vy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz);
	float *coord = new float[3];
	int *viewport = new int[4];

	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = m_OpenGL->GetScreenWidth();
	viewport[3] = m_OpenGL->GetScreenHeight();

	glm::vec4 viewport4 = glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]);
	glm::vec3 win = glm::vec3(vx, vy, vz);
	glm::mat4 model, proj;

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			proj[i][j] = projectionMatrix[j + i * 4];
			model[i][j] = viewMatrix[j + i * 4];
		}
	}

	glm::vec3 vv = glm::unProject(win, model, proj, viewport4);

	position.x = vv.x;
	position.y = vv.y;
	position.z = vv.z;
	delete[] coord;
	delete[] viewport;
}

bool GraphicsClass::Render() const
{
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];

	// Clear the buffers to begin the scene.
	m_OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
//	m_Camera->BuildNewViewMatrix();

	// Get the world, view, and projection matrices from the opengl and camera objects.
	m_OpenGL->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_OpenGL->GetProjectionMatrix(projectionMatrix);

	// Set the color shader as the current shader program and set the matrices that it will use for rendering.
	m_ColorShader->SetShader(m_OpenGL);
	m_ColorShader->SetShaderParameters(m_OpenGL, worldMatrix, viewMatrix, projectionMatrix);

	// Render the model using the color shader.
	m_Model->Render(m_OpenGL);

	// Clear the buffers to begin the scene.
	//m_OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 0.0f);

	// Present the rendered scene to the screen.
	m_OpenGL->EndScene();

	return true;
}


