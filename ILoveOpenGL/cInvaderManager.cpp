#include "cInvaderManager.h"

cInvaderManager::cInvaderManager(std::vector<std::string> meshNames)
{
	cInvaderLine* line1 = new cInvaderLine(meshNames[2], meshNames[3], 200.f, 0.5f, 0.6f);
	cInvaderLine* line2 = new cInvaderLine(meshNames[1], meshNames[0], 100.f, 0.4f, 0.6f);
	cInvaderLine* line3 = new cInvaderLine(meshNames[0], meshNames[1], 0.f, 0.3f, 0.6f);
	cInvaderLine* line4 = new cInvaderLine(meshNames[4], meshNames[5], -100.f, 0.2f, 0.6f);
	cInvaderLine* line5 = new cInvaderLine(meshNames[5], meshNames[4], -200.f, 0.1f, 0.6f);
	line1->initialize();
	line2->initialize();
	line3->initialize();
	line4->initialize();
	line5->initialize();

	invaderMatrix.push_back(line1);
	invaderMatrix.push_back(line2);
	invaderMatrix.push_back(line3);
	invaderMatrix.push_back(line4);
	invaderMatrix.push_back(line5);

}

bool cInvaderManager::Iterate(float delta)
{
	float randFactor = (rand() % 100) / 100.f;
	bool gameOver = false;
	for (int i = 0; i < invaderMatrix.size(); i++)
	{
		invaderMatrix[i]->iterate(delta);
		UFOTimer += delta * randFactor;
		if (invaderMatrix[i]->isExhausted())
			gameOver = true;

		if (UFO != nullptr)
		{
			UFOMove += delta;
			if (UFOMove > 0.2f)
			{
				UFO->positionXYZ.x += 25.f;
				UFOMove = 0.f;
			}

			if (UFO->positionXYZ.x > 1100.f)
			{

				::g_vec_pMeshes.erase(std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), UFO));
				UFOTimer = 0.f;
				UFO = nullptr;
			}
		}

	}
	if (gameOver)
	{
		for (int i = 0; i < invaderMatrix.size(); i++)
		{
			invaderMatrix[i]->exhaust();
		}

	}

	if (UFOTimer > 20.f && UFO == nullptr)
	{
		UFOTimer = 0.f;
		UFO = new cMesh();

		UFO->bDontLight = true;
		UFO->bUseWholeObjectDiffuseColour = true;
		UFO->wholeObjectDiffuseRGBA = glm::vec4(0.9f, 0.2f, 0.2f, 1.0f);
		UFO->meshName = "SpaceInvader_UFO_block.ply";
		UFO->positionXYZ = glm::vec3(-1100.f, 350.f, 0.f);

		g_vec_pMeshes.push_back(UFO);
	}

	return gameOver;
}

cInvaderManager::~cInvaderManager()
{
	for (int i = 0; i < invaderMatrix.size(); i++)
	{
		delete invaderMatrix[i];
	}
}

bool cInvaderManager::getCollisionInfo(float x, float y, int& xout, int& yout)
{
	for (int i = 0; i < invaderMatrix.size(); i++)
	{
		int invader = -1;
		invader = invaderMatrix[i]->hasInvader(x, y);
		if (invader >= 0)
		{
			xout = invader;
			yout = i;
			return true;
		}
	}
	if (UFO != nullptr)
	{
		if (x < (UFO->positionXYZ.x + 80.f) && x >(UFO->positionXYZ.x - 80.f) && y > 300.f)
		{
			xout = -1;
			yout = -1;
			return true;
		}
	}
	return false;
}

std::vector<cInvaderLine*> cInvaderManager::getLines()
{
	return invaderMatrix;
}
