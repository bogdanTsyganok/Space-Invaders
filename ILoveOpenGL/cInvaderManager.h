#pragma once

#include "cInvaderLine.h"
#include "cExploder.h"

class cInvaderManager
{
	std::vector<cInvaderLine*> invaderMatrix;
	float UFOMove = 0.f;
public:
	cInvaderManager(std::vector<std::string> meshNames);

	bool Iterate(float delta);

	~cInvaderManager();

	bool getCollisionInfo(float x, float y, int& xout, int& yout);
	cMesh* UFO = nullptr;
	float UFOTimer = 0.f;
	std::vector<cInvaderLine*> getLines();
};