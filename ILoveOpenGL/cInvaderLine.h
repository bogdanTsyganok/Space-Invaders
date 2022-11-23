#pragma once

#include "globalThings.h";

enum direction {LEFT, RIGHT};

class cInvaderLine
{
	const float LEFT_BOUND = 200;
	const float RIGHT_BOUND = -200;
	const float INVADER_WIDTH = 110;
	const float INVADER_HEIGHT = 80;
	const float EMPTY_WIDTH = 50;
	std::string mFirstMesh;
	std::string mSecondMesh;
	float mStartHeight;
	float mInteranlCounter = 0.f;


	bool mJustFlipped = false;
	bool mWentInCycle = false;
	bool exhausted = false;
	int mLineCenter = 0;

	direction mCurrentDirection = RIGHT;
	std::vector<cMesh*> mInvaders;

	//Amount of time in a cycle
	float mTimeCycle = 0.f;

	//Point in the cycle when "step" will happen
	float mStepTimePoint = 0.f;
public:
	cInvaderLine() = delete;
	cInvaderLine(std::string fMesh, std::string sMesh, float height, float stepTime, float resetTime);

	//Call when ready to draw
	void initialize();

	bool iterate(float delta);

	//cleanup
	bool end();

	int hasInvader(float x, float y);

	std::vector<cMesh*> getMeshes();

	void deleteInvader(int index);

	bool isExhausted();

	void exhaust();
};