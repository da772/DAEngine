#pragma once
namespace da::core
{
	struct mat4
	{
		float Mtx[16];
	};

	class CCamera {

	public:
		CCamera(const Vector3f& position, const Vector3f& rotation);
		CCamera();

		void setPosition(const Vector3f& pos);
		void setRotation(const Vector3f& rot);
		Vector3f getPosition() const;
		Vector3f getRotation() const;
		Vector3f getRightVector() const;
		Vector3f getForwardVector() const;
		const mat4* getMatrix() const;

	private:
		void calculateMatrix(const Vector3f& position, const Vector3f& rotation);
		mat4 m_mtx;

	};
}