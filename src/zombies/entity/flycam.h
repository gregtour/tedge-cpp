#ifndef _FLYCAM_H
#define _FLYCAM_H

#include "../../engine/entity/camera.h"

class CFlyCam : public CCamera, public IInputtingObj
{
public:
    CFlyCam(SVector3 pos);

    void Update(float);
    void Render();
};

#endif // _FLYCAM_H

