////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Ogl.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a) = (b); (b) = _tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a) = (b); (b) = _tmp; }
#define MAT(m, r, c) (m)[(c) * 4 + (r)]

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	Quaternion *rotate;

	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(COgl*, HWND);
	void Shutdown();
	bool Frame();

	void MouseLDown(CPoint point);
	void MouseLUp(CPoint point);
	void MouseMove(CPoint point) ;
	void Update(UINT nChar, UINT nRepCnt, UINT nFlags);
	void Rotate(Vector3 n, float degress);

private:
	
	float oldX, oldY, oldZ;
	glm::vec3 tr, tmpTr;
	void MackeMouseCoordinat(CPoint point, VectorType& position) const;

	bool Render() const;

	COgl* m_OpenGL;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
	
	Vector3 rotationAxis;
	float angularSpeed;
};

#endif