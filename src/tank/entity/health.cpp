#include "health.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include "../../engine/sound/wavesound.h"
#include "tank.h"

const char* HEALTH_TEXTURE_FILE = "health.png";
const char* HEALTH_MODEL_FILE = "health.ms3d";
const char* HEALTH_SOUND_FILE = "pickup.wav";

CHealth::CHealth()
{
	mTexture = CManagedTexture::Load(&gResourceManager, HEALTH_TEXTURE_FILE);
	mModel = CManagedModel::Load(&gResourceManager, HEALTH_MODEL_FILE);
	mPickupSound = CManagedWaveSound::Load(&gResourceManager, HEALTH_SOUND_FILE);
	mType = TYPE_HEALTH;
}

CHealth::~CHealth()
{
}

void CHealth::PickedUp(CTank* by)
{
	CPickup::PickedUp(by);
	by->AddHealth();
}


