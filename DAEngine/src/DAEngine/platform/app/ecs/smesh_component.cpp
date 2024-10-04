

#include "smesh_component.h"
#include "graphics/graphics_smesh.h"
#include "graphics/graphics_material_data.h"
#include "graphics/graphics_texture2d.h"
#include "graphics/factory/factory_graphics_smesh.h"

namespace da {
#ifdef DA_REVIEW
	COMPONENT_CPP_NO_UPDATE_DEBUG(CSmeshComponent);
#else
	COMPONENT_CPP_NO_UPDATE(CSmeshComponent);
#endif

	CSmeshComponent::CSmeshComponent(const std::string& meshPath, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_inverseNormals(false)
	{
		m_staticMesh = da::CStaticMeshFactory::create(meshPath, false);
	}

	CSmeshComponent::CSmeshComponent(const std::string& meshPath, bool inverseNormals, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_inverseNormals(inverseNormals)
	{
		m_staticMesh = da::CStaticMeshFactory::create(meshPath, inverseNormals);
	}

	CSmeshComponent::CSmeshComponent(da::CStaticMesh* mesh, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent)
	{
		m_staticMesh = mesh;
		ASSERT(m_staticMesh);
	}

	void CSmeshComponent::onInitialize()
	{
		
	}

	void CSmeshComponent::onShutdown()
	{
		da::CStaticMeshFactory::remove(m_staticMesh);
	}

	da::CStaticMesh* CSmeshComponent::getStaticMesh() const
	{
		return m_staticMesh;
	}

	const std::vector<da::FInstance>& CSmeshComponent::getInstances() const
	{
		return m_instances;
	}

	void CSmeshComponent::removeInstance(uint32_t id)
	{
		for (uint32_t i = 0; i < m_instances.size(); i++) {
			if (m_instances[i].Id == id) {
				m_instances.erase(m_instances.begin() + i);
				return;
			}
		}
	}

	uint32_t CSmeshComponent::addInstance(const glm::mat4& transform)
	{
		m_instances.push_back({ transform, ++m_instanceCounter });
		return m_instanceCounter;
	}

#ifdef DA_REVIEW
	void CSmeshComponent::onDebugRender()
	{
		char buffer[4096];
		sprintf_s(buffer, sizeof(buffer), "Mesh: %s", m_staticMesh->getPath().c_str());
		ImGui::Text(buffer);

		sprintf_s(buffer, sizeof(buffer), "Mesh Count: %d", m_staticMesh->getMeshes().size());
		ImGui::Text(buffer);
		sprintf_s(buffer, sizeof(buffer), "Material Count: %d", m_staticMesh->getMaterialCount());
		ImGui::Text(buffer);

		size_t vertCount = 0;
		for (size_t i = 0; i < m_staticMesh->getMeshes().size(); i++) {
			vertCount += m_staticMesh->getMeshes()[i].Vertices.size();
		}

		ImGui::Text("Vertices: %d", vertCount);

		size_t indCount = 0;
		for (size_t i = 0; i < m_staticMesh->getMeshes().size(); i++) {
			indCount += m_staticMesh->getMeshes()[i].Indices.size();
		}
		ImGui::Text("Indices:  %d", indCount);



		if (ImGui::CollapsingHeader("Materials")) {
			for (size_t i = 0; i < m_staticMesh->getMaterialCount(); i++) {
				ImGui::Indent();
				if (ImGui::CollapsingHeader((std::string("Material: ") + std::to_string(i) + std::string("##") + std::string(m_guid.c_str())).c_str())) {
					ImGui::Text("Albedo Factor");
					ImGui::SameLine();
					ImGui::InputFloat4((std::string("##albedoFactor") + std::to_string(i) + std::string(m_guid.c_str())).c_str(), (float*)&m_staticMesh->getMaterial(i).baseColorFactor);

					ImGui::Text("Metallic Factor");
					ImGui::SameLine();
					ImGui::InputFloat((std::string("##metallic") + std::to_string(i) + std::string(m_guid.c_str())).c_str(), (float*)&m_staticMesh->getMaterial(i).metallicFactor);

					ImGui::Text("Roughness Factor");
					ImGui::SameLine();
					ImGui::InputFloat((std::string("##roughness") + std::to_string(i) + std::string(m_guid.c_str())).c_str(), (float*)&m_staticMesh->getMaterial(i).roughnessFactor);

					ImGui::Text("Emissive Factor");
					ImGui::SameLine();
					ImGui::InputFloat3((std::string("##emissiveFactor") + std::to_string(i) + std::string(m_guid.c_str())).c_str(), (float*)&m_staticMesh->getMaterial(i).emissiveFactor);

					ImGui::Text("UV Scale");
					ImGui::SameLine();
					ImGui::InputFloat2((std::string("##uvFactor") + std::to_string(i) + std::string(m_guid.c_str())).c_str(), (float*)&m_staticMesh->getMaterial(i).uvScale);

					ImGui::Text("Albedo Texture: %s", m_staticMesh->getMaterial(i).getBaseColorTexture() ? m_staticMesh->getMaterial(i).getBaseColorTexture()->getPath().c_str() : "None");
					ImGui::Text("Normal Texture: %s", m_staticMesh->getMaterial(i).getNormalTexture() ? m_staticMesh->getMaterial(i).getNormalTexture()->getPath().c_str() : "None");
					ImGui::Text("Rough/Metal Texture: %s", m_staticMesh->getMaterial(i).getMetallicRoughnessTexture() ? m_staticMesh->getMaterial(i).getMetallicRoughnessTexture()->getPath().c_str() : "None");
					ImGui::Text("Occlusion Texture: %s", m_staticMesh->getMaterial(i).getOcclusionTexture() ? m_staticMesh->getMaterial(i).getOcclusionTexture()->getPath().c_str() : "None");
					ImGui::Text("Emissive Texture: %s", m_staticMesh->getMaterial(i).getEmissiveTexture() ? m_staticMesh->getMaterial(i).getEmissiveTexture()->getPath().c_str() : "None");

				}
				ImGui::Unindent();
			}
		}

		if (ImGui::Button(m_staticMesh->getHidden() ? "Show" : "Hide")) {
			m_staticMesh->hide(!m_staticMesh->getHidden());
		}

		if (ImGui::Button(!m_staticMesh->getCastShadows() ? "Cast Shadows" : "Hide Shadows")) {
			m_staticMesh->castShadows(!m_staticMesh->getCastShadows());
		}

		if (ImGui::Button("Reload Mesh")) {
			da::CStaticMesh* oldMesh = m_staticMesh;
			std::string path = oldMesh->getPath();

			da::CStaticMeshFactory::remove(m_staticMesh);
			m_staticMesh = da::CStaticMeshFactory::create(path, m_inverseNormals);

			for (size_t i = 0; i < m_staticMesh->getMaterialCount() && i < oldMesh->getMaterials().size(); i++) {
				m_staticMesh->getMaterial(i) = oldMesh->getMaterial(i);
				oldMesh->getMaterial(i) = {};
			}

			da::CStaticMeshFactory::remove(oldMesh);
		}
	}
#endif

}