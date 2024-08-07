#pragma once

#include "component.h"
#include "daengine/asset/asset.h"
#include "daengine/core/graphics/skeletal_animation.h"
#include "daengine/core/graphics/skeletal_animator.h"
#include "daengine/core/graphics/skeletal_anim_graph.h"

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
		CSkeletalMeshComponent(da::graphics::CSkeletalAnimGraph* graph, CEntity& parent);
		CSkeletalMeshComponent(const std::string& meshPath, const std::string& animPath, bool inverseNormals, CEntity& parent);

		void onInitialize();
		void render();
		void updateAnimation(float dt);
		void onShutdown();
		void setTransform(const glm::mat4& transform);
		const glm::mat4& getTransform();
		bool getBoneLocalTransform(CHashString name, glm::mat4& out);
		bool getBoneWorldTransform(CHashString name,  glm::mat4& out);
		bool getBoneWorldPosition(CHashString name, glm::vec3& out);
		bool getBoneWorldRotation(CHashString name, glm::quat& out);
		da::graphics::CSkeletalMesh* getSkeletalMesh() const;
		da::graphics::CSkeletalAnimation* getSkeletalAnimation() const;
		da::graphics::CSkeletalAnimator* getSkeletalAnimator() const;
#ifdef DA_REVIEW
		void onDebugRender();
#endif

	private:
		void onTransform(const glm::mat4& oldT, const glm::mat4& newT);
	private:
		da::graphics::CSkeletalMesh* m_skeletalmesh = nullptr;
		da::graphics::CSkeletalAnimation* m_animation = nullptr;
		da::graphics::CSkeletalAnimator* m_animator = nullptr;
		da::graphics::CSkeletalAnimGraph* m_animGraph = nullptr;
		bool m_inverseNormals = false;
		glm::mat4 m_transform = glm::mat4(1.f);;
		glm::mat4 m_finalTransform;

	};
}