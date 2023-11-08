#include "dapch.h"
#include "entity.h"
#include "scene.h"

namespace da::core {

	CEntity::CEntity(const CGuid& guid, CScene* scene) : m_guid(guid), m_scene(scene)
	{

	}

	CEntity::CEntity(CScene* scene) : m_guid(CGuid::Generate()), m_scene(scene)
	{

	}



	const da::core::CGuid& CEntity::getId() const
	{
		return m_guid;
	}

}