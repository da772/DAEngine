#include "dapch.h"

#include "smesh_component.h"
#include "core/graphics/graphics_smesh.h"
#include "platform/graphics/bgfx/bgfx_graphics_material.h"
#include "platform/graphics/bgfx/bgfx_static_mesh.h"
#include <bx/bx.h>
#include <bgfx/bgfx.h>

namespace da::core {
	COMPONENT_CPP(CSmeshComponent);

	CSmeshComponent::CSmeshComponent(const std::string& meshPath, CMaterial* material, CEntity& parent) : m_guid(CGuid::Generate()), m_material(material), m_parent(parent)
	{
		m_staticMesh = new da::platform::CBgfxStaticMesh(meshPath);
	}

	void CSmeshComponent::onInitialize()
	{
		m_material->initialize();
	}

	void CSmeshComponent::onUpdate(float dt)
	{
		glm::mat4 m = m_parent.getTransform().getMat();
		::bgfx::setTransform(&m);
		((da::platform::CBgfxStaticMesh*) m_staticMesh)->setBuffers(0);
	}

	void CSmeshComponent::onShutdown()
	{
		m_material->shutdown();
		delete m_material;
	}

	da::core::CStaticMesh* CSmeshComponent::getStaticMesh() const
	{
		return m_staticMesh;
	}

	da::core::CMaterial* CSmeshComponent::getMaterial() const
	{
		return m_material;
	}
}