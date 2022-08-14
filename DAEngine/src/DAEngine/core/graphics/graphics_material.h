#pragma once

namespace da::core
{
	class CMaterial
	{
	public:
		virtual void initialize() {};
		virtual void update(int frame) {};
		virtual void shutdown() {};
		
		Vector3f Position;
		Vector3f CamPosition;
		Vector3f CamRot;

	};
}