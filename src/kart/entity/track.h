#ifndef _TRACK_H
#define _TRACK_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"

struct SVector3;
class CTexture;
class CModel;
class I3DVERTBUF;
class CTrack: public CEntity, public IGraphicalObj
{
public:
	CTrack();
	~CTrack();

    void BuildTrack();

	void Update( float dt );
	void Render();

protected:
	enum MONSTERTYPE
	{
		M_GREMLING,
		M_BAT,
		M_BOSS,
		M_GHOST
	};
	void	CreateMonster(SVector3 pos, MONSTERTYPE type);
	void	AddMonster(SVector3 pos, MONSTERTYPE type);

	float mPulse;
	CTexture* mTexture;
	CModel*	mTrackModel;
};

#endif
