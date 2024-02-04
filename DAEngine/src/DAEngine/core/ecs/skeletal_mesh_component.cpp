#include "dapch.h"

#include "skeletal_mesh_component.h"
#include "core/graphics/graphics_smesh.h"
#include "platform/graphics/bgfx/bgfx_graphics_material.h"
#include "platform/graphics/bgfx/bgfx_skeletal_mesh.h"
#include <core/graphics/skeletal_animation.h>
#include <core/graphics/skeletal_animator.h>

#include <bx/bx.h>
#include <bgfx/bgfx.h>

namespace da::core {
#ifdef DA_DEBUG
	COMPONENT_CPP_NO_UPDATE_DEBUG(CSkeletalMeshComponent);
#else
	COMPONENT_CPP_NO_UPDATE(CSkeletalMeshComponent);
#endif

	CSkeletalMeshComponent::CSkeletalMeshComponent(const std::string& meshPath, const std::string& animPath, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_inverseNormals(false)
	{
		m_skeletalmesh = new da::platform::CBgfxSkeletalMesh(meshPath, false);
		m_animation = new da::graphics::CSkeletalAnimation(animPath, (da::graphics::FSkeletalMesh*)&m_skeletalmesh->getMeshes()[0]);
		m_animator = new da::graphics::CSkeletalAnimator(m_animation);
	}

	CSkeletalMeshComponent::CSkeletalMeshComponent(const std::string& meshPath, const std::string& animPath, bool inverseNormals, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_inverseNormals(inverseNormals)
	{
		m_skeletalmesh = new da::platform::CBgfxSkeletalMesh(meshPath, inverseNormals);
		m_animation = new da::graphics::CSkeletalAnimation(animPath, (da::graphics::FSkeletalMesh*)&m_skeletalmesh->getMeshes()[0]);
		m_animator = new da::graphics::CSkeletalAnimator(m_animation);
	}

	CSkeletalMeshComponent::CSkeletalMeshComponent(da::graphics::CSkeletalMesh* mesh, da::graphics::CSkeletalAnimation* anim, da::graphics::CSkeletalAnimator* animator, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent)
	{
		m_skeletalmesh = mesh;
		m_animation = anim;
		m_animator = animator;
	}

	void CSkeletalMeshComponent::onInitialize()
	{
		m_animator->playAnimation(m_animation);
	}


	void CSkeletalMeshComponent::updateAnimation(float dt)
	{
		m_animator->updateAnimation(dt);
	}

	void CSkeletalMeshComponent::render()
	{
		((da::platform::CBgfxSkeletalMesh*)m_skeletalmesh)->setBuffers(0, 0);
	}

	void CSkeletalMeshComponent::onShutdown()
	{
		delete m_skeletalmesh;
		delete m_animation;
		delete m_animator;
	}

	da::graphics::CSkeletalMesh* CSkeletalMeshComponent::getSkeletalMesh() const
	{
		return m_skeletalmesh;
	}

	da::graphics::CSkeletalAnimator* CSkeletalMeshComponent::getSkeletalAnimator() const
	{
		return m_animator;
	}

	da::graphics::CSkeletalAnimation* CSkeletalMeshComponent::getSkeletalAnimation() const
	{
		return m_animation;
	}

#ifdef DA_DEBUG
	void CSkeletalMeshComponent::onDebugRender()
	{
		char buffer[4096];
		sprintf_s(buffer, sizeof(buffer), "Mesh: %s", m_skeletalmesh->getPath().c_str());
		ImGui::Text(buffer);

		sprintf_s(buffer, sizeof(buffer), "Mesh Count: %d", m_skeletalmesh->getMeshes().size());
		ImGui::Text(buffer);
		sprintf_s(buffer, sizeof(buffer), "Material Count: %d", m_skeletalmesh->getMaterialCount());
		ImGui::Text(buffer);

		size_t vertCount = 0;
		for (size_t i = 0; i < m_skeletalmesh->getMeshes().size(); i++) {
			vertCount += m_skeletalmesh->getMeshes()[i].Vertices.size();
		}

		ImGui::Text("Vertices: %d", vertCount);

		size_t indCount = 0;
		for (size_t i = 0; i < m_skeletalmesh->getMeshes().size(); i++) {
			indCount += m_skeletalmesh->getMeshes()[i].Indices.size();
		}
		ImGui::Text("Indices:  %d", indCount);


		if (ImGui::Button(m_skeletalmesh->getHidden() ? "Show" : "Hide")) {
			m_skeletalmesh->hide(!m_skeletalmesh->getHidden());
		}

		if (ImGui::Button(!m_skeletalmesh->getCastShadows() ? "Cast Shadows" : "Hide Shadows")) {
			m_skeletalmesh->castShadows(!m_skeletalmesh->getCastShadows());
		}

		if (ImGui::Button("Reload Mesh")) {
			std::string path = m_skeletalmesh->getPath();
			std::vector<da::graphics::FMaterialData> materials = m_skeletalmesh->getMaterials();
			delete m_skeletalmesh;
			m_skeletalmesh = new da::platform::CBgfxSkeletalMesh(path, m_inverseNormals);

			for (size_t i = 0; i < m_skeletalmesh->getMaterialCount() && i < materials.size(); i++) {
				m_skeletalmesh->getMaterial(i) = materials[i];
			}

		}
	}
#endif

}