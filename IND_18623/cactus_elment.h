#pragma once
#include <atltypes.h>

namespace type_utils {

	enum class MetaType {
		GREEN_PART,
		YELLOW_PART
	};

	enum class ElementGroup {
		ALL,
		SINGLE
	};

	struct cactus_element {
		CPoint position;
		int sx; //scale
		int sy;
		float angle;
		MetaType type;
	};
}