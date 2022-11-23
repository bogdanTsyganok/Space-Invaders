#pragma once
#include "globalThings.h"
#include "cInvaderManager.h"
#include "cExploder.h"

class cProjectileManager
{
	cInvaderManager* mInvaders;
	cMesh* mPlayer;
	std::vector<cMesh*> mPlayerProjectiles;
	std::vector<cMesh*> mAlienBombs;
	float mInnerTimer;
	std::vector<std::string> projectileMeshes;
	std::vector<glm::vec3> shields;
	cExploder exploder;
public:
	cProjectileManager() = delete;
	cProjectileManager(cMesh* player, cInvaderManager* manager);

	bool Integrate(float deltaTime);

	void PlayerFired();

	void AlienFired();
};