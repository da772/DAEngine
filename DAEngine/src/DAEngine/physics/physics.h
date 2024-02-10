#pragma once



namespace da::physics
{
	class CPhysicsType
	{
	public:
		virtual void initialize() = 0;
		virtual void update(float dt) = 0;
		virtual void shutdown() = 0;
	};

	class CPhysics
	{
	public:
		static void initialize();
		static void update(float dt);
		static void shutdown();

		static CPhysicsType* getPhysicsType();

	private:

		static CPhysicsType* s_physicsType;
	};
}