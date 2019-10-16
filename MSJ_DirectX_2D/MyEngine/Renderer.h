#pragma once
//#include "Component.h"
#include "SubTransform.h"
#include "DXHeader.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
#include "Blend.h"
#include "Sprite.h"

class Renderer : public SubTransform
{
private:
	friend class RenderManager;

private:
	int m_RenderGroup;

protected:
	virtual bool Init(int _RenderGroup = 0);

private:
	virtual void Render(const Matrix& _ViewProjection) = 0;

public:
	Renderer();
	~Renderer();
};

