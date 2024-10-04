#pragma once

#include "core/ecs/component.h"

namespace da {
	class INetwork;
}

namespace da
{
	class CNetworkedTransformComponent {
		COMPONENT_H(CNetworkedTransformComponent)
	public:
		CNetworkedTransformComponent(uint32_t tickRate, bool isOwner, CEntity& parent);
		CNetworkedTransformComponent(bool isOwner, CEntity& parent);

	private:
		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();

		bool shouldUpdate();

	private:
		da::INetwork* m_network = nullptr;
		const float m_tickSpeed = 1.f/30.f;
		float m_timeSinceUpdate = 0.f;
		float m_lerpSpeed = 10.f;

		CHashString m_id;

		glm::vec3 m_targetPosition = {0.f,0.f, 0.f};
		glm::quat m_targetRotation = {};

		bool m_isOwner : 1;
		bool m_hasPacket : 1 = false;
	};
}