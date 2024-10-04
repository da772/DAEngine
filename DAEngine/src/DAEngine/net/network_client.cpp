
#include "network_client.h"
#include "core/logger.h"
#include "core/threading/worker_pool.h"
#include "network_util.h"
#include <enet/enet.h>

namespace da
{

	CNetworkClient::CNetworkClient(uint64_t id, const FConnectionSettings& settings, const std::function<void(bool)>& callback) : INetwork(id), m_settings(settings)
	{
		m_thread = std::thread([this, callback]() {
			m_isValid = createClient();
			da::CWorkerPool::addMainJob([callback, this] { callback(m_isValid); });
			clientListen();
		});
	}

	bool CNetworkClient::createClient()
	{
		std::lock_guard<std::mutex> guard(m_mutex);

		ENetAddress address;
		ENetEvent event;
		int service;
		enet_address_set_host_ip(&address, m_settings.IP.c_str());
		address.port = m_settings.Port;

		m_host = enet_host_create(NULL, 1, 2, 0, 0);

		m_peer = enet_host_connect(m_host, &address, 2, 0);

		ASSERT(m_peer);

		service = enet_host_service(m_host, &event, m_settings.Timeout);
		if (service <= 0) {
			LOG_ERROR(ELogChannel::Net, "Failed to connect to %s:%d", m_settings.IP.c_str(), m_settings.Port);
			return false;
		}
		ASSERT(m_host);
		LOG_INFO(ELogChannel::Net, "Connected to server %s:%d", m_settings.IP.c_str(), m_settings.Port);		
		return true;
	}

	void CNetworkClient::clientListen()
	{
		ENetEvent event;
		long eventType = 0;
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			ENetHost* host = m_host;
			

			long eventType = enet_host_service(host, &event, m_settings.ListenRate);

			if (!m_isValid) {
				LOG_ERROR(ELogChannel::Net, "Failed server failed to listen, setup invalid");
				return;
			}
		}

		while (true) {
			
			{
				std::lock_guard<std::mutex> guard(m_mutex);
				ENetHost* host = m_host;

				if (!host || eventType < 0) {
					return;
				}
				switch (event.type) {
					// empty
				case 0:
					break;
				case ENET_EVENT_TYPE_CONNECT: {
					LOG_INFO(ELogChannel::Net, "A new client connected from %s:%d, id: %d", CNetworkUtil::IPV4ToString(event.peer->address.host).c_str(), event.peer->address.port, event.peer->connectID);
					break;
				}
				case ENET_EVENT_TYPE_RECEIVE:
				{
					//LOG_DEBUG(ELogChannel::Net, "Recieved packet of length %d, from %d on channel %d", event.packet->dataLength, event.peer->connectID, event.channelID);
					//LOG_DEBUG(ELogChannel::Net, "Data: %s", event.packet->data);

					uint32_t id = 0;
					memcpy(&id, event.packet->data, sizeof(uint32_t));
					const std::unordered_map<uint32_t, std::function<void(const char*, size_t)>>::iterator& it = m_funcs.find(id);
					char* data = new char[event.packet->dataLength];
					memcpy(data, event.packet->data, event.packet->dataLength);
					size_t dataSize = event.packet->dataLength;
					if (it != m_funcs.end()) {
						std::function<void(const char*, size_t)> func = it->second;
						da::CWorkerPool::addMainJob([func, data, dataSize] {
							func(data, dataSize);
							delete data;
						});
					}
					break;
				}
				case ENET_EVENT_TYPE_DISCONNECT: {
					LOG_INFO(ELogChannel::Net, "A client has disconnected, %s:%d, id: %d", CNetworkUtil::IPV4ToString(event.peer->address.host).c_str(), event.peer->address.port, event.peer->connectID);
					break;
				}
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(m_settings.ListenRate));
			std::lock_guard<std::mutex> guard(m_mutex);
			if (m_host)
				eventType = enet_host_service(m_host, &event, m_settings.ListenRate);
		}
	}

	void CNetworkClient::sendPacket(const char* data, size_t length, bool reliable)
	{
		std::lock_guard<std::mutex> guard(m_mutex);

		if (!m_host) return;

		ENetPacket* packet = enet_packet_create(data, length, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
		enet_host_broadcast(m_host, 0, packet);
		enet_host_flush(m_host);
	}

	CNetworkClient::~CNetworkClient()
	{
		clean();
	}

	void CNetworkClient::clean()
	{
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			if (m_peer) {
				enet_peer_disconnect(m_peer, 0);
				enet_peer_reset(m_peer);
				m_peer = nullptr;
			}

			if (m_host)
			{
				enet_host_destroy(m_host);
				m_host = nullptr;
			}

			m_isValid = false;
		}

		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

	void CNetworkClient::getPacket(uint32_t id, std::function<void(const char*, size_t)> func)
	{
		m_funcs[id] = func;
	}

}