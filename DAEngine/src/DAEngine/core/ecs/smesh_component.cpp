#include "dapch.h"

#include "smesh_component.h"
#include "core/graphics/graphics_smesh.h"
#include "platform/graphics/bgfx/bgfx_graphics_material.h"
#include "platform/graphics/bgfx/bgfx_static_mesh.h"
#include <bx/bx.h>
#include <bgfx/bgfx.h>

namespace da::core {
#ifdef DA_DEBUG
	COMPONENT_CPP_DEBUG(CSmeshComponent);
#else
	COMPONENT_CPP(CSmeshComponent);
#endif

	CSmeshComponent::CSmeshComponent(const std::string& meshPath, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent)
	{
		m_staticMesh = new da::platform::CBgfxStaticMesh(meshPath);
	}

	void CSmeshComponent::onInitialize()
	{
		
	}

	void CSmeshComponent::onUpdate(float dt)
	{
		glm::mat4 m = m_parent.getTransform().matrix();
		::bgfx::setTransform(&m);
		((da::platform::CBgfxStaticMesh*) m_staticMesh)->setBuffers(0, 0);
	}

	void CSmeshComponent::onShutdown()
	{
		delete m_staticMesh;
	}

	da::graphics::CStaticMesh* CSmeshComponent::getStaticMesh() const
	{
		return m_staticMesh;
	}

#ifdef DA_DEBUG
	void CSmeshComponent::onDebugRender()
	{
		char pathNameBuffer[4096];
		sprintf_s(pathNameBuffer, sizeof(pathNameBuffer), "Mesh: %s", m_staticMesh->getPath().c_str());
		ImGui::Text(pathNameBuffer);
		//ImGui::Text("Vertices: %d", m_staticMesh->getVertices().size());
		//ImGui::Text("Indices:  %d", m_staticMesh->getIndices().size());
		//ImGui::Text("VertexBuffer: 0x%p", m_staticMesh->getNativeVB());
		//ImGui::Text("IndexBuffer: 0x%p", m_staticMesh->getNativeIB());

		if (ImGui::Button("Reload Mesh")) {
			std::string path = m_staticMesh->getPath();
			delete m_staticMesh;
			m_staticMesh = new da::platform::CBgfxStaticMesh(path);
		}
	}
#endif

}