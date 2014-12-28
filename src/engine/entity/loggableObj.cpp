#include "loggableObj.h"

ILoggableObj::ILoggableObj()
{

}

ILoggableObj::~ILoggableObj()
{

}

void ILoggableObj::LogState()
{
	gLog.LogItem( new CLogMessage("Logging state of this object") );
}
