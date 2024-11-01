#pragma once
#include <atltypes.h>

enum META_TYPE {
	GREEN_PART,
	YELLOW_PART
};


struct cactus_element {
	CPoint position;
	int sx; //scale
	int sy;
	float angle;
	META_TYPE type;
};