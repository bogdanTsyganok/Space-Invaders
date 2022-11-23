#include "ProjectileManager.h"

cProjectileManager::cProjectileManager(cMesh* player, cInvaderManager* manager)
{
	mPlayer = player;
	mInvaders = manager;
	projectileMeshes.push_back("SpaceInvader_Bomb_pose_1.ply");
	projectileMeshes.push_back("SpaceInvader_Bomb_pose_2.ply");
	projectileMeshes.push_back("SpaceInvader_Bomb_pose_3.ply");
	mInnerTimer = 0.f;

	shields.push_back(glm::vec3(-900.f, -900.f, 0.f));
	shields.push_back(glm::vec3(-300.f, -900.f, 0.f));
	shields.push_back(glm::vec3(300.f, -900.f, 0.f));
	shields.push_back(glm::vec3(900.f, -900.f, 0.f));
}

bool cProjectileManager::Integrate(float deltaTime)
{
	std::vector<cMesh*> deletedProjectiles;
	std::vector<std::pair<int, int>> deletedInvaders;
	mInnerTimer += deltaTime;
	exploder.IntegrateExplosions(deltaTime);
	bool shouldWiggle = false;
	if (mInnerTimer >= 0.3f)
	{
		mInnerTimer = 0.f;
		shouldWiggle = true;
	}

	for (cMesh* playerProjectile : mPlayerProjectiles)
	{
		playerProjectile->positionXYZ.y += 600.f * deltaTime;

		if (shouldWiggle)
		{
			if (playerProjectile->meshName == projectileMeshes[0])
				playerProjectile->meshName = projectileMeshes[1];

			else if (playerProjectile->meshName == projectileMeshes[1])
				playerProjectile->meshName = projectileMeshes[2];

			if (playerProjectile->meshName == projectileMeshes[2])
				playerProjectile->meshName = projectileMeshes[0];
		}

		for (glm::vec3 shieldPos : shields)
		{
			if (playerProjectile->positionXYZ.y > (shieldPos.y - 50.f) && playerProjectile->positionXYZ.x > (shieldPos.x - 65.f) && playerProjectile->positionXYZ.x < (shieldPos.x + 65.f))
			{

				deletedProjectiles.push_back(playerProjectile);
				std::vector<cMesh*>::iterator projIt = std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), playerProjectile);
				::g_vec_pMeshes.erase(projIt);
				continue;
			}
		}

		int row, column;
		float positionX, positionY;
		if (playerProjectile->positionXYZ.y > 500.f)
		{
			exploder.Explode(playerProjectile->positionXYZ.x, playerProjectile->positionXYZ.y);
			deletedProjectiles.push_back(playerProjectile);
			std::vector<cMesh*>::iterator projIt = std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), playerProjectile);
			::g_vec_pMeshes.erase(projIt);
		}

		if (mInvaders->getCollisionInfo(playerProjectile->positionXYZ.x, playerProjectile->positionXYZ.y, row, column))
		{
			deletedProjectiles.push_back(playerProjectile);
			std::vector<cMesh*>::iterator projIt = std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), playerProjectile);
			::g_vec_pMeshes.erase(projIt);

			if (row == -1 && column == -1)
			{
				::g_vec_pMeshes.erase(std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), mInvaders->UFO));
				exploder.Explode(mInvaders->UFO->positionXYZ.x, mInvaders->UFO->positionXYZ.y);
				delete mInvaders->UFO;
				mInvaders->UFO = nullptr;
				mInvaders->UFOTimer = 0.f;
				continue;
			}

			deletedInvaders.push_back(std::make_pair(row, column));
			std::vector<cMesh*>::iterator invaderIt = std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), mInvaders->getLines()[column]->getMeshes()[row]);
			positionX = (*invaderIt)->positionXYZ.x;
			positionY = (*invaderIt)->positionXYZ.y;
			::g_vec_pMeshes.erase(invaderIt);

			exploder.Explode(positionX, positionY);

			if (::timeDilation < 1.75f)
			{
				::timeDilation *= 1.10f;
			}
			else
			{
				::timeDilation *= 1.0000001f;
			}

			continue;
		}
	}

	for (cMesh* alienProjectile : mAlienBombs)
	{
		alienProjectile->positionXYZ.y -= 600.f * deltaTime;
		
		if(shouldWiggle)
		{
			if (alienProjectile->meshName == projectileMeshes[0])
				alienProjectile->meshName = projectileMeshes[1];

			else if (alienProjectile->meshName == projectileMeshes[1])
				alienProjectile->meshName = projectileMeshes[2];

			if (alienProjectile->meshName == projectileMeshes[2])
				alienProjectile->meshName = projectileMeshes[0];
		}

		for (glm::vec3 shieldPos : shields)
		{
			if (alienProjectile->positionXYZ.y < (shieldPos.y + 50.f) && alienProjectile->positionXYZ.x > (shieldPos.x - 95.f) && alienProjectile->positionXYZ.x < (shieldPos.x + 95.f))
			{

				deletedProjectiles.push_back(alienProjectile);
				std::vector<cMesh*>::iterator projIt = std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), alienProjectile);
				::g_vec_pMeshes.erase(projIt);
				continue;
			}
		}

		if (alienProjectile->positionXYZ.y < -1000.f)
		{
			exploder.Explode(alienProjectile->positionXYZ.x, alienProjectile->positionXYZ.y);
			deletedProjectiles.push_back(alienProjectile);
			std::vector<cMesh*>::iterator projIt = std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), alienProjectile);
			::g_vec_pMeshes.erase(projIt);
		}

		if (alienProjectile->positionXYZ.y < -900.f && alienProjectile->positionXYZ.x < (::g_PlayerTankPosition.x + 100.f) && alienProjectile->positionXYZ.x > (::g_PlayerTankPosition.x - 100.f))
		{			
			std::vector<cMesh*>::iterator projIt = std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), alienProjectile);
			::g_vec_pMeshes.erase(projIt);
			std::vector<cMesh*>::iterator deljIt = std::find(mAlienBombs.begin(), mAlienBombs.end(), alienProjectile);
			mAlienBombs.erase(deljIt);

			exploder.Explode(::g_PlayerTankPosition.x, ::g_PlayerTankPosition.y);
			return true;
		}
	}


	for (int i = 0; i < deletedProjectiles.size(); i++)
	{
		std::vector<cMesh*>::iterator projIt = std::find(mPlayerProjectiles.begin(), mPlayerProjectiles.end(), deletedProjectiles[i]);
		if (projIt == mPlayerProjectiles.end())
		{
			projIt = std::find(mAlienBombs.begin(), mAlienBombs.end(), deletedProjectiles[i]);
			mAlienBombs.erase(projIt);
		}
		else
		{
			mPlayerProjectiles.erase(projIt);
		}
	}
	for (int i = 0; i < deletedInvaders.size(); i++)
	{
		mInvaders->getLines()[deletedInvaders[i].second]->deleteInvader(deletedInvaders[i].first);
	}

	return false;
}

void cProjectileManager::PlayerFired()
{
	if (mPlayerProjectiles.empty())
	{
		cMesh* projectileMesh = new cMesh();
		projectileMesh->bDontLight = true;
		projectileMesh->bUseWholeObjectDiffuseColour = true;
		projectileMesh->wholeObjectDiffuseRGBA = glm::vec4(0.2f, 0.9f, 0.2f, 1.0f);
		projectileMesh->meshName = projectileMeshes[0];
		projectileMesh->positionXYZ = mPlayer->positionXYZ;
		projectileMesh->positionXYZ.y += 20.f;

		::g_vec_pMeshes.push_back(projectileMesh);
		mPlayerProjectiles.push_back(projectileMesh);
	}
}

void cProjectileManager::AlienFired()
{	
	int randomNumber = rand() % 100;
	int amountOfRows = mInvaders->getLines().size();
	int amountOfShips = mInvaders->getLines()[0]->getMeshes().size();
	int selectedRow = 0;
	int selectedInvader = 0;
	cMesh* selectedShipMesh = nullptr;
	int totalAmountOfShips = 0;
	for (int i = 0; i < amountOfRows; i++)
	{
		totalAmountOfShips += mInvaders->getLines()[0]->getMeshes().size();
	}
	if (totalAmountOfShips == 0)
	{
		return;
	}

	for (int i = 0; i < randomNumber; i++)
	{
		selectedInvader++;
		if (selectedRow >= amountOfRows)
		{
			selectedRow = 0;
			amountOfShips = mInvaders->getLines()[selectedRow]->getMeshes().size();
			selectedInvader = 0;
		}
		if (selectedInvader >= amountOfShips)
		{
			selectedRow++;
			selectedInvader = 0;
		}
	}
	if (selectedRow == 5)
		selectedRow = 4;
	if (mInvaders->getLines()[selectedRow]->isExhausted())
	{
		return;
	}
	std::vector<cMesh*> temp = mInvaders->getLines()[selectedRow]->getMeshes();
	if (selectedInvader >= temp.size())
		return;
	selectedShipMesh = temp[selectedInvader];

	cMesh* projectileMesh = new cMesh();
	projectileMesh->bDontLight = true;
	projectileMesh->bUseWholeObjectDiffuseColour = true;
	projectileMesh->wholeObjectDiffuseRGBA = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	projectileMesh->meshName = projectileMeshes[0];
	projectileMesh->positionXYZ = selectedShipMesh->positionXYZ;

	::g_vec_pMeshes.push_back(projectileMesh);
	mAlienBombs.push_back(projectileMesh);
}
