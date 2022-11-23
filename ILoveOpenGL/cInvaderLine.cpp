#include "cInvaderLine.h"
#include <iostream>

cInvaderLine::cInvaderLine(std::string fMesh, std::string sMesh, float height, float timeStep, float timeCycle)
{
	mFirstMesh = fMesh;
	mSecondMesh = sMesh;
	mStartHeight = height;
	mTimeCycle = timeCycle;
	mStepTimePoint = timeStep;
}

void cInvaderLine::initialize()
{
	for (int i = 0; i < 6; i++)
	{
		cMesh* invaderMesh = new cMesh();
		if((i % 2) == 0)
			invaderMesh->meshName = mFirstMesh;
		else
			invaderMesh->meshName = mSecondMesh;	

		invaderMesh->bDontLight = true;
		invaderMesh->wholeObjectDiffuseRGBA = glm::vec4(0.9f, 0.9f, 0.9f, 1.f);
		invaderMesh->bUseWholeObjectDiffuseColour = true;
		invaderMesh->positionXYZ = glm::vec3((EMPTY_WIDTH + INVADER_WIDTH) * i, mStartHeight, 0.f);

		::g_vec_pMeshes.push_back(invaderMesh);
		mInvaders.push_back(invaderMesh);
	}
	for (int i = 1; i < 6; i++)
	{
		cMesh* invaderMesh = new cMesh();
		if ((i % 2) == 0)
			invaderMesh->meshName = mFirstMesh;
		else
			invaderMesh->meshName = mSecondMesh;

		invaderMesh->bDontLight = true;
		invaderMesh->wholeObjectDiffuseRGBA = glm::vec4(0.9f, 0.9f, 0.9f, 1.f);
		invaderMesh->bUseWholeObjectDiffuseColour = true;
		invaderMesh->positionXYZ = glm::vec3(-(EMPTY_WIDTH + INVADER_WIDTH) * i, mStartHeight, 0.f);

		::g_vec_pMeshes.push_back(invaderMesh);
		mInvaders.push_back(invaderMesh);
	}
}

bool cInvaderLine::iterate(float delta)
{
	if (isExhausted())
		return false;
	mInteranlCounter += delta * ::timeDilation;
	if (mInteranlCounter >= mTimeCycle)
	{
		mWentInCycle = false;
		mInteranlCounter -= mTimeCycle;
	}
	if ((mInteranlCounter >= mStepTimePoint) && !mWentInCycle)
	{
		mWentInCycle = true;
		if ((mLineCenter < RIGHT_BOUND || mLineCenter > LEFT_BOUND) &&  !mJustFlipped)
		{
			for (cMesh* inv : mInvaders)
			{
				inv->positionXYZ.y -= INVADER_HEIGHT / 2;

				if (inv->positionXYZ.y < -800.f)
					exhaust();

				if (inv->meshName == mFirstMesh)
					inv->meshName = mSecondMesh;
				else
					inv->meshName = mFirstMesh;

				mJustFlipped = true;
			}
			if (mCurrentDirection == LEFT)
			{
				mCurrentDirection = RIGHT;
			}
			else
			{
				mCurrentDirection = LEFT;
			}
		}
		else
		{
			mJustFlipped = false;
			if (mCurrentDirection == RIGHT)
			{
				mLineCenter -= EMPTY_WIDTH / 2;
			}
			else
			{
				mLineCenter += EMPTY_WIDTH / 2;

			}
			for (cMesh* inv : mInvaders)
			{

				if (mCurrentDirection == RIGHT)
				{
					inv->positionXYZ.x -= EMPTY_WIDTH / 2;
				}
				else
				{
					inv->positionXYZ.x += EMPTY_WIDTH / 2;

				}

				if (inv->meshName == mFirstMesh)
					inv->meshName = mSecondMesh;
				else
					inv->meshName = mFirstMesh;
			}
		}
		std::cout << "Iterated" << std::endl;
	}
	return false;
}

bool cInvaderLine::end()
{
	return true;
}

int cInvaderLine::hasInvader(float x, float y)
{
	for (int i = 0; i < mInvaders.size(); i++)
	{
		glm::vec3 pos = mInvaders[i]->positionXYZ;
		if (x < (pos.x + INVADER_WIDTH/2) && x >(pos.x - INVADER_WIDTH/2))
			if (y < (pos.y + (INVADER_HEIGHT / 2) ) && y >(pos.y - (INVADER_HEIGHT / 2)))
				return i;
	}
	return -1;
}

std::vector<cMesh*> cInvaderLine::getMeshes()
{
	return mInvaders;
}

void cInvaderLine::deleteInvader(int index)
{
	cMesh* invaderPointer = mInvaders[index];
	

	std::vector<cMesh*>::iterator meshIt = std::find(mInvaders.begin(), mInvaders.end(), invaderPointer);
	mInvaders.erase(meshIt);

}

bool cInvaderLine::isExhausted()
{
	return exhausted;
}

void cInvaderLine::exhaust()
{
	exhausted = true;
}
