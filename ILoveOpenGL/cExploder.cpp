#include "cExploder.h"

class cExmploder
{

};

void cExploder::Explode(float x, float y)
{
	cMesh* projectileMesh = new cMesh();
	projectileMesh->bDontLight = true;
	projectileMesh->bUseWholeObjectDiffuseColour = true;
	projectileMesh->wholeObjectDiffuseRGBA = glm::vec4(0.2f, 0.9f, 0.2f, 1.0f);
	projectileMesh->meshName = "SpaceInvader_Explosion.ply";
	projectileMesh->positionXYZ = glm::vec3(x, y, 0.f);

	::g_vec_pMeshes.push_back(projectileMesh);

	meshTimerPairs.push_back(std::make_pair(projectileMesh, 0.f));
}

void cExploder::IntegrateExplosions(float delta)
{
	std::vector<cMesh*> deletingPointers;
	for (int i = 0; i < meshTimerPairs.size(); i++)
	{
		meshTimerPairs[i].second += delta;
		if (meshTimerPairs[i].second > 0.6f)
		{
			cMesh* tempPointer = meshTimerPairs[i].first;
			deletingPointers.push_back(tempPointer);
			meshTimerPairs[i].second = -1000000000000.f;
		}
	}
	for (cMesh* deleteThis : deletingPointers)
	{
		::g_vec_pMeshes.erase(std::find(::g_vec_pMeshes.begin(), ::g_vec_pMeshes.end(), deleteThis));
	}
}
