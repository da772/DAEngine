#pragma once
#include "test_component.h"
#include "script_component.h"
#include "smesh_component.h"
#include "skeletal_mesh_component.h"
#include "rigid_body_component.h"
#include "dynamic_light_component.h"
#include "networked_transform_component.h"

namespace da::core {
	class CComponents {
	public:
		static void registerComponents();
	};
}