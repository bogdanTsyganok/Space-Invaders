#pragma once
#include "globalThings.h"

class cExploder
{
	std::vector<std::pair<cMesh*, float>> meshTimerPairs;
public:

	void Explode(float x, float y);

	void IntegrateExplosions(float delta);
};