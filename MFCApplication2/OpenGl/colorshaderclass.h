////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Ogl.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ColorShaderClass
////////////////////////////////////////////////////////////////////////////////
class ColorShaderClass
{
public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	bool Initialize(COgl*, HWND);
	void Shutdown(COgl*);
	void SetShader(COgl*);
	bool SetShaderParameters(COgl*, float*, float*, float*);

private:
	bool InitializeShader(char*, char*, COgl*, HWND);
	char* LoadShaderSourceFile(char*);
	void OutputShaderErrorMessage(COgl*, HWND, unsigned int, char*);
	void OutputLinkerErrorMessage(COgl*, HWND, unsigned int);
	void ShutdownShader(COgl*);

private:
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_shaderProgram;
};

#endif