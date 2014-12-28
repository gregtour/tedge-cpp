#ifndef _LOGGABLEOBJ_H
#define _LOGGABLEOBJ_H

#include "../engine.h"

class ILoggableObj
{
public:
	ILoggableObj();
	virtual ~ILoggableObj();

	virtual void LogState();
};

#endif
