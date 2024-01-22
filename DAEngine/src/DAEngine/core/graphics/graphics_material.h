#pragma once
#include "DAEngine/core/core.h"

namespace da::graphics
{
	class CStaticMesh;

	class CMaterial
	{
	public:
		virtual void initialize() {};
		virtual void update(int frame) {};
		virtual void shutdown() {};
		
		Vector3f Position;
		Vector3f Scale;
		Vector3f Rotation;
		Vector3f CamPosition;
		Vector3f CamRot;
		float RotationSpeed = 0.f;

	};
}