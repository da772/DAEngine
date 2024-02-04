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
		COMPONENT_H_NO_UPDATE_DEBUG(CSkeletalMeshComponent);
#else
		COMPONENT_H_NO_UPDATE(CSkeletalMeshComponent);
#endif

	public:
		CSkeletalMeshComponent(const std::string& meshPath, const std::string& animPath, CEntity& parent);
		CSkeletalMeshComponent(da::graphics::CSkeletalMesh* mesh, da::graphics::CSkeletalAnimation* anim, da::graphics::CSkeletalAnimator* animator,CEntity& parent);
		CSkeletalMeshComponent(const std::string& meshPath, const std::string& animPath, bool inverseNormals, CEntity& parent);

		void onInitialize();
		void render();
		void updateAnimation(float dt);
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