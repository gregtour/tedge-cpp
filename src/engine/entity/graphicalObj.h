#ifndef _GRAPHICALOBJ_H
#define _GRAPHICALOBJ_H

class IGraphicalObj
{
public:
	IGraphicalObj();
	virtual ~IGraphicalObj();

	virtual void Render();
	virtual void RenderAlpha();
	virtual void RenderOrtho();
};

#endif
