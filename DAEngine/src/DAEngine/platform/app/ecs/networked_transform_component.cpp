
#include "networked_transform_component.h"
#include "net/network_manager.h"
#include "stl/time.h"

#define TRANSFORM_PACKET_ID HASH("FTransformPacketInfo")

struct FTransformPacketInfo {
	uint32_t PacketId = TRANSFORM_PACKET_ID;
	uint64_t  PlayerId;
	glm::vec3 Position;
	glm::quat Rotation;

	FTransformPacketInfo() {};
	FTransformPacketInfo(uint32_t id, const glm::vec3& pos, const glm::quat& rot) : PlayerId(id), Position(pos), Rotation(rot) {

	}

};

namespace da {
	COMPONENT_CPP(CNetworkedTransformComponent)


	CNetworkedTransformComponent::CNetworkedTransformComponent(uint32_t tickRate, bool isOwner, CEntity& parent) :m_isOwner(isOwner), m_guid(CGuid::Generate()), m_parent(parent), m_tickSpeed(1.f/(float)tickRate), m_id(m_parent.getId().c_str())
	{

	}

	CNetworkedTransformComponent::CNetworkedTransformComponent(bool isOwner, CEntity& parent) : m_isOwner(isOwner), m_guid(CGuid::Generate()), m_parent(parent), m_id(m_parent.getId().c_str())
	{

	}

	void CNetworkedTransformComponent::onInitialize()
	{
		if (m_network = da::CNetworkManager::getNetwork()) {

			if (m_isOwner)
			{
				return;
			}

			m_network->getPacket<FTransformPacketInfo>(TRANSFORM_PACKET_ID, [this](FTransformPacketInfo data) {
				
				if (data.PlayerId != m_id)
				{
					return;
				}

				da::CTransform& transform = m_parent.getTransform();

				m_targetPosition = data.Position;
				m_targetRotation = data.Rotation;
				m_hasPacket = true;
			});
		}

	}

	void CNetworkedTransformComponent::onUpdate(float dt)
	{
		if (!m_network) return;

		if (!m_isOwner)
		{
			if (!m_hasPacket) return;

			da::CTransform& transform = m_parent.getTransform();

			if (!shouldUpdate())
			{
				m_hasPacket = false;
				return;
			}
			

			transform.setPosition(glm::mix(transform.position(), m_targetPosition, m_lerpSpeed * dt));
			transform.setRotation(glm::slerp(transform.rotation(), m_targetRotation, m_lerpSpeed * dt));
			return;
		}

		m_timeSinceUpdate += dt;

		if (m_timeSinceUpdate < m_tickSpeed) return;
		m_timeSinceUpdate = 0.f;

		da::CTransform& transform = m_parent.getTransform();
		m_targetPosition = transform.position();
		m_targetRotation = transform.rotation();

		if (shouldUpdate()) {
			m_network->sendPacket<FTransformPacketInfo>(FTransformPacketInfo(m_id.hash(), transform.position(), transform.rotation()), false);
		}
	}

	void CNetworkedTransformComponent::onShutdown()
	{
		if (m_network)
		{
			m_network->clearPackets();
		}
		m_network = nullptr;
	}

	bool CNetworkedTransformComponent::shouldUpdate()
	{
		return true;
		if (glm::distance2(m_parent.getTransform().position(), m_targetPosition) < .001f) {
			return false;
		}


		return true;
	}

}