#include "graphicalObj.h"
#include "../common/list.h"

extern CLinkedList<IGraphicalObj> gGraphicalObjList;

IGraphicalObj::IGraphicalObj()
{
	gGraphicalObjList.Add(this);
}

IGraphicalObj::~IGraphicalObj()
{
	gGraphicalObjList.RemoveEntry(gGraphicalObjList.FindEntry(this));
}

void IGraphicalObj::Render()
{
}

void IGraphicalObj::RenderAlpha()
{
}

void IGraphicalObj::RenderOrtho()
{
}

