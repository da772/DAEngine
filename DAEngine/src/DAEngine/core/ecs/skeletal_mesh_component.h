#pragma once

#include "component.h"
#include "daengine/asset/asset.h"
#include "core/graphics/skeletal_animation.h"
#include "core/graphics/skeletal_animator.h"

namespace da::core
{
	class CSkeletalMeshComponent {
#ifdef DA_REVIEW
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
		void setTransform(const glm::mat4& transform);
		glm::mat4 getTransform();
		da::graphics::CSkeletalMesh* getSkeletalMesh() const;
		da::graphics::CSkeletalAnimation* getSkeletalAnimation() const;
		da::graphics::CSkeletalAnimator* getSkeletalAnimator() const;
#ifdef DA_REVIEW
		void onDebugRender();
#endif

	private:
		void onTransform(const glm::mat4& oldT, const glm::mat4& newT);
	private:
		da::graphics::CSkeletalMesh* m_skeletalmesh;
		da::graphics::CSkeletalAnimation* m_animation;
		da::graphics::CSkeletalAnimator* m_animator;
		bool m_inverseNormals;
		glm::mat4 m_transform = glm::mat4(1.f);;
		glm::mat4 m_finalTransform;

	};
}