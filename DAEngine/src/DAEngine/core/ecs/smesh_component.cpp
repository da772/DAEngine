#include "dapch.h"

#include "smesh_component.h"
#ifndef DA_TEST
#include "core/graphics/graphics_smesh.h"
#include "platform/graphics/bgfx/bgfx_graphics_material.h"
#include "platform/graphics/bgfx/bgfx_static_mesh.h"
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#endif

namespace da::core {
	COMPONENT_CPP(CSmeshComponent);

#ifndef DA_TEST
	CSmeshComponent::CSmeshComponent(const std::string& meshPath, const std::string& materialPathVS, const std::string& materialPathFS, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent)
	{
		m_staticMesh = new da::platform::CBgfxStaticMesh(meshPath);
		m_material = new da::platform::CBgfxGraphicsMaterial(materialPathVS, materialPathFS);
	}
#endif

	void CSmeshComponent::onInitialize()
	{
#ifndef DA_TEST
		m_material->initialize();
#endif
	}

	void CSmeshComponent::onUpdate(float dt)
	{
#ifndef DA_TEST
		glm::mat4 m = m_parent.getTransform().getMat();
		::bgfx::setTransform(&m);
		((da::platform::CBgfxStaticMesh*) m_staticMesh)->setBuffers(0);
		m_material->update(0);
#endif
	}

	void CSmeshComponent::onShutdown()
	{
#ifndef DA_TEST
		m_material->shutdown();
#endif
	}
}
