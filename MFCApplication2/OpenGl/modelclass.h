////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Ogl.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:

	struct VertexType
	{
		float x, y, z;
		float r, g, b;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(COgl*);
	void Shutdown(COgl*);
	void Render(COgl*);

private:
	bool InitializeBuffers(COgl*);
	void ShutdownBuffers(COgl*);
	void RenderBuffers(COgl*);

	int m_vertexCount, m_indexCount;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
};

#endif