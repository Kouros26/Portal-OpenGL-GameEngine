#pragma once
#include "../include/Singleton.h"

class Map
{
private:
	Model* model;
	GameObject* floor, * wallFront, * wallLeft,
		 * wallRight, * wallBack, *roofL,
	     * columnFR, * columnFL, * columnBL,
		 * columnBR, * roofR;

	void InitWalls();
	void InitFloorNRoof();
	void InitColumns();
	void PushGraph();

public:
	Map();
};
