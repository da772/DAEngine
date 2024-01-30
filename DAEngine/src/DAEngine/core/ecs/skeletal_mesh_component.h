#pragma once

#include "component.h"
#include "daengine/asset/asset.h"


namespace da::graphics
{
	class CSkeletalMesh;
	class CSkeletalAnimation;
	class CSkeletalAnimator;
}

namespace da::core
{
	class CSkeletalMeshComponent {
#ifdef DA_DEBUG
		COMPONENT_H_DEBUG(CSkeletalMeshComponent);
#else
		COMPONENT_H(CSkeletalMeshComponent);
#endif

	public:
		CSkeletalMeshComponent(const std::string& meshPath, const std::string& animPath, CEntity& parent);
		CSkeletalMeshComponent(const std::string& meshPath, const std::string& animPath, bool inverseNormals, CEntity& parent);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();
		da::graphics::CSkeletalMesh* getSkeletalMesh() const;
		da::graphics::CSkeletalAnimation* getSkeletalAnimation() const;
		da::graphics::CSkeletalAnimator* getSkeletalAnimator() const;
#ifdef DA_DEBUG
		void onDebugRender();
#endif

	private:
		da::graphics::CSkeletalMesh* m_skeletalmesh;
		da::graphics::CSkeletalAnimation* m_animation;
		da::graphics::CSkeletalAnimator* m_animator;
		bool m_inverseNormals;

	};
}