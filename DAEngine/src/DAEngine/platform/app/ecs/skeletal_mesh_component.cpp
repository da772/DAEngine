

#include "skeletal_mesh_component.h"
#include "graphics/graphics_smesh.h"
#include "graphics/factory/factory_graphics_skmesh.h"

#include <Imgui.h>

namespace da::core {
#ifdef DA_REVIEW
	COMPONENT_CPP_NO_UPDATE_DEBUG(CSkeletalMeshComponent);
#else
	COMPONENT_CPP_NO_UPDATE(CSkeletalMeshComponent);
#endif

	CSkeletalMeshComponent::CSkeletalMeshComponent(const std::string& meshPath, const std::string& animPath, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_inverseNormals(false)
	{
		m_skeletalmesh = da::factory::CSkeletalMeshFactory::create(meshPath, false);
		m_animation = new da::graphics::CSkeletalAnimation(animPath, m_skeletalmesh);
		m_animator = new da::graphics::CSkeletalAnimator(m_animation);
	}

	CSkeletalMeshComponent::CSkeletalMeshComponent(const std::string& meshPath, const std::string& animPath, bool inverseNormals, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_inverseNormals(inverseNormals)
	{
		m_skeletalmesh = da::factory::CSkeletalMeshFactory::create(meshPath, inverseNormals);
		m_animation = new da::graphics::CSkeletalAnimation(animPath, m_skeletalmesh);
		m_animator = new da::graphics::CSkeletalAnimator(m_animation);
	}

	CSkeletalMeshComponent::CSkeletalMeshComponent(da::graphics::CSkeletalMesh* mesh, da::graphics::CSkeletalAnimation* anim, da::graphics::CSkeletalAnimator* animator, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent)
	{
		m_skeletalmesh = mesh;
		m_animation = anim;
		m_animator = animator;
	}

	CSkeletalMeshComponent::CSkeletalMeshComponent(da::graphics::CSkeletalAnimGraph* graph, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent)
	{
		m_animGraph = graph;
		m_animator = const_cast<da::graphics::CSkeletalAnimator*>(graph->getAnim());
		m_skeletalmesh = graph->getMesh();
	}

	void CSkeletalMeshComponent::onInitialize()
	{
		m_animator->playAnimation(m_animation);
		m_parent.getTransform().addOnTransform(BIND_EVENT_FN_2(da::core::CSkeletalMeshComponent, onTransform));
	}


	void CSkeletalMeshComponent::updateAnimation(float dt)
	{
		PROFILE()
		if (m_animGraph) {
			m_animGraph->update(dt);
			return;
		}

		m_animator->updateAnimation(dt);
	}

	void CSkeletalMeshComponent::render()
	{
		m_skeletalmesh->setBuffers(0, 0);
	}

	void CSkeletalMeshComponent::onShutdown()
	{
		if (m_skeletalmesh) da::factory::CSkeletalMeshFactory::remove(m_skeletalmesh);
		if (m_animation) delete m_animation;
		if (m_animator) delete m_animator;

		if (m_animGraph) delete m_animGraph;

		m_parent.getTransform().removeOnTransform(BIND_EVENT_FN_2(da::core::CSkeletalMeshComponent, onTransform));
	}

	da::graphics::CSkeletalMesh* CSkeletalMeshComponent::getSkeletalMesh() const
	{
		if (m_animGraph) return m_animGraph->getMesh();
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

	const glm::mat4& CSkeletalMeshComponent::getTransform()
	{
		if (m_parent.getTransform().isDirty()) {
			m_finalTransform = m_parent.getTransform().matrix() * m_transform;
		}
		
		return m_finalTransform;
	}

	void CSkeletalMeshComponent::setTransform(const glm::mat4& transform)
	{
		m_transform = transform;
		m_finalTransform = m_parent.getTransform().matrix() * m_transform;
	}

	void CSkeletalMeshComponent::onTransform(const glm::mat4& oldT, const glm::mat4& newT)
	{
		m_finalTransform = newT * m_transform;
	}

	bool CSkeletalMeshComponent::getBoneLocalTransform(CHashString name, glm::mat4& out)
	{
		ASSERT(m_animator);
		return m_animator->getBoneLocalTransform(name, out);
	}

	bool CSkeletalMeshComponent::getBoneWorldTransform(CHashString name,  glm::mat4& out)
	{
		ASSERT(m_animator);
		return m_animator->getBoneWorldTransform(name, getTransform(), out);
	}

	bool CSkeletalMeshComponent::getBoneWorldPosition(CHashString name,  glm::vec3& out)
	{
		ASSERT(m_animator);
		return m_animator->getBoneWorldPosition(name, getTransform(), out);
	}

	bool CSkeletalMeshComponent::getBoneWorldRotation(CHashString name,  glm::quat& out)
	{
		ASSERT(m_animator);
		return m_animator->getBoneWorldRotation(name, getTransform(), out);
	}


#ifdef DA_REVIEW
	void CSkeletalMeshComponent::onDebugRender()
	{
		if (ImGui::CollapsingHeader("Anim")) {
			ImGui::Indent();
			float playTime = m_animator->getPlayTime();

			ImGui::Text("Play time");
			ImGui::SameLine();
			if (ImGui::SliderFloat("##animTime", &playTime, 0.f, m_animator->getMaxPlayTime())) {
				m_animator->setPlayTime(playTime);
			}


			ImGui::Text("Time scale");
			ImGui::SameLine();
			float timeScale = m_animator->getTimeScale();
			if (ImGui::InputFloat("##timeScale", &timeScale, 0.1f, 1.f)) {
				m_animator->setTimeScale(timeScale);
			}

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Mesh")) {
			ImGui::Indent();
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


			if (ImGui::CollapsingHeader("Materials")) {
				for (size_t i = 0; i < m_skeletalmesh->getMaterialCount(); i++) {
					ImGui::Indent();
					if (ImGui::CollapsingHeader((std::string("Material: ") + std::to_string(i) + std::string("##") + std::string(m_guid.c_str())).c_str())) {
						ImGui::Text("Albedo Factor");
						ImGui::SameLine();
						ImGui::InputFloat4((std::string("##albedoFactor") + std::to_string(i) + std::string(m_guid.c_str())).c_str(), (float*)&m_skeletalmesh->getMaterial(i).baseColorFactor);

						ImGui::Text("Metallic Factor");
						ImGui::SameLine();
						ImGui::InputFloat((std::string("##metallic") + std::to_string(i) + std::string(m_guid.c_str())).c_str(), (float*)&m_skeletalmesh->getMaterial(i).metallicFactor);

						ImGui::Text("Roughness Factor");
						ImGui::SameLine();
						ImGui::InputFloat((std::string("##roughness") + std::to_string(i) + std::string(m_guid.c_str())).c_str(), (float*)&m_skeletalmesh->getMaterial(i).roughnessFactor);

						ImGui::Text("Emissive Factor");
						ImGui::SameLine();
						ImGui::InputFloat3((std::string("##emissiveFactor") + std::to_string(i) + std::string(m_guid.c_str())).c_str(), (float*)&m_skeletalmesh->getMaterial(i).emissiveFactor);
					}
					ImGui::Unindent();
				}
			}

			if (ImGui::Button(m_skeletalmesh->getHidden() ? "Show" : "Hide")) {
				m_skeletalmesh->hide(!m_skeletalmesh->getHidden());
			}

			if (ImGui::Button(!m_skeletalmesh->getCastShadows() ? "Cast Shadows" : "Hide Shadows")) {
				m_skeletalmesh->castShadows(!m_skeletalmesh->getCastShadows());
			}

			if (ImGui::Button("Reload Mesh")) {

				da::graphics::CSkeletalMesh* oldMesh = m_skeletalmesh;
				std::string path = oldMesh->getPath();

				m_skeletalmesh = da::factory::CSkeletalMeshFactory::create(path, m_inverseNormals);

				for (size_t i = 0; i < m_skeletalmesh->getMaterialCount() && i < oldMesh->getMaterials().size(); i++) {
					m_skeletalmesh->getMaterial(i) = oldMesh->getMaterial(i);
					oldMesh->getMaterial(i) = {};
				}

				da::factory::CSkeletalMeshFactory::remove(oldMesh);
			}
			ImGui::Unindent();
		}
	}
#endif

}