#include "dapch.h"
#include "network_server.h"

#include "DAEngine/core/threading/worker_pool.h"
#include "network_util.h"
#include <enet/enet.h>
#include <miniupnpc/portfwd.h>




namespace da::net
{
	CNetworkServer::CNetworkServer(uint64_t id, const FNetworkSettings& settings, const std::function<void(bool)>& callback) : INetwork(id), m_settings(settings)
	{
		m_thread = std::thread([this, callback] {
			setupServer();
			da::core::CWorkerPool::addMainJob([callback, this] { callback(m_isValid); });
			serverListen();
			});
	}

	void CNetworkServer::setupServer()
	{
		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = m_settings.Port;

		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_portFwd = new Portfwd();
			if (!m_portFwd->init(2000)) {
				LOG_ERROR(ELogChannel::Net, "Failed to initialize UPNP, Port: %d", m_settings.Port);
				delete m_portFwd;
				m_portFwd = nullptr;
			}

			if (m_portFwd) {
				//m_portFwd->remove(m_settings.Port);
				m_portFwd->add(m_settings.Port);
			}

			m_host = enet_host_create(&address, m_settings.MaxClients, m_settings.Channels, m_settings.IncomingBroadcast, m_settings.OutgoingBroadcast);
			ASSERT(m_host);
			if (m_portFwd) {
				m_info.ExternalIP = m_portFwd->external_ip();
				m_info.InternalIP = m_portFwd->lan_ip();
			}

			LOG_INFO(ELogChannel::Net, "Server Created");
			LOG_INFO(ELogChannel::Net, "Server Internal IP: %s", m_info.InternalIP.c_str());
			LOG_INFO(ELogChannel::Net, "Server External IP: %s", m_info.ExternalIP.c_str());
			LOG_INFO(ELogChannel::Net, "Server Port: %d", m_settings.Port);
			m_isValid = true;
		}
	}

	void CNetworkServer::serverListen()
	{
		if (!m_isValid) {
			LOG_ERROR(ELogChannel::Net, "Failed server failed to listen, setup invalid");
			return;
		}

		ASSERT(m_host);

		ENetEvent event;
		long eventType = enet_host_service(m_host, &event, m_settings.ListenRate);

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
						if (it != m_funcs.end()) {
							std::function<void(const char*, size_t)> func = it->second;
							da::core::CWorkerPool::addMainJob([func, event] {
								func((const char*)event.packet->data, event.packet->dataLength);
							});
						}

						{
							for (size_t i = 0; i < host->connectedPeers; i++) {

								ENetPeer* peer = &host->peers[i];

								if (peer->connectID != event.peer->connectID) {
									ENetPacket* packet = enet_packet_create(event.packet->data, event.packet->dataLength, 0);
									enet_peer_send(peer, 0x0001, packet);
									enet_host_flush(m_host);
								}
							}
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
			if (!m_host) {
				return;
			}
				
			eventType = enet_host_service(m_host, &event, 0);
		}
		

	}

	void CNetworkServer::sendPacket(const char* data, size_t length, bool reliable)
	{
		std::lock_guard<std::mutex> guard(m_mutex);

		if (!m_host) return;

		ENetPacket* packet = enet_packet_create(data, length, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
		enet_host_broadcast(m_host, 0, packet);
		enet_host_flush(m_host);
	}

	CNetworkServer::~CNetworkServer()
	{
		clean();
	}

	void CNetworkServer::clean()
	{
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			if (m_host) {
				enet_host_flush(m_host);
				enet_host_destroy(m_host);
				m_host = nullptr;
			}

			if (m_portFwd) {
				m_portFwd->remove(m_settings.Port);
				delete m_portFwd;
				m_portFwd = nullptr;
			}

			m_isValid = false;
		}

		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

	void CNetworkServer::getPacket(uint32_t id, std::function<void(const char*, size_t)> func)
	{
		m_funcs[id] = func;
	}

}