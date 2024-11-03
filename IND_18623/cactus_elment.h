#pragma once
#include <atltypes.h>
#include<vector>


namespace type_utils {
	enum class ObjectType {
		GREEN_PART,
		YELLOW_PART,
		CIRCLE
	};


	class cactus_element {
	public:
		CPoint position;
		float sx; //scale
		float sy;
		float angle;
		ObjectType type;
	};

	struct element_group {
		std::vector<cactus_element> elementi;
		CPoint rotation_point;
	};
}