#include "dapch.h"

#include "smesh_component.h"
#include "core/graphics/graphics_smesh.h"
#include "platform/graphics/bgfx/bgfx_graphics_material.h"
#include "platform/graphics/bgfx/bgfx_static_mesh.h"
#include <bx/bx.h>
#include <bgfx/bgfx.h>

namespace da::core {
#ifdef DA_REVIEW
	COMPONENT_CPP_NO_UPDATE_DEBUG(CSmeshComponent);
#else
	COMPONENT_CPP_NO_UPDATE(CSmeshComponent);
#endif

	CSmeshComponent::CSmeshComponent(const std::string& meshPath, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_inverseNormals(false)
	{
		m_staticMesh = new da::platform::CBgfxStaticMesh(meshPath, false);
	}

	CSmeshComponent::CSmeshComponent(const std::string& meshPath, bool inverseNormals, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_inverseNormals(inverseNormals)
	{
		m_staticMesh = new da::platform::CBgfxStaticMesh(meshPath, inverseNormals);
	}

	void CSmeshComponent::onInitialize()
	{
		
	}

	void CSmeshComponent::onShutdown()
	{
		delete m_staticMesh;
	}

	da::graphics::CStaticMesh* CSmeshComponent::getStaticMesh() const
	{
		return m_staticMesh;
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
			std::string path = m_staticMesh->getPath();
			std::vector<da::graphics::FMaterialData> materials = m_staticMesh->getMaterials();
			delete m_staticMesh;
			m_staticMesh = new da::platform::CBgfxStaticMesh(path, m_inverseNormals);

			for (size_t i = 0; i < m_staticMesh->getMaterialCount() && i < materials.size(); i++) {
				m_staticMesh->getMaterial(i) = materials[i];
			}

		}
	}
#endif

}