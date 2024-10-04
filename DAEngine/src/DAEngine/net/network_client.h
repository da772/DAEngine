#pragma once

#include "network_interface.h"

struct _ENetPeer;
struct _ENetHost;

namespace da
{

	class CNetworkClient : public INetwork {
	public:
		CNetworkClient(uint64_t id ,const FConnectionSettings& settings, const std::function<void(bool)>& callback);
		~CNetworkClient();
		void sendPacket(const char* data, size_t length, bool reliable) override;
		inline virtual bool isValid() const { return m_isValid; }
		virtual void getPacket(uint32_t id, std::function<void(const char*, size_t)> func) override;
		inline virtual bool isHost() const override { return false; }


	private:
		bool createClient();
		void clientListen();
		void clean();

	private:
		_ENetPeer* m_peer;
		_ENetHost* m_host;
		FConnectionSettings m_settings;
		FNetworkInfo m_info;
		std::mutex m_mutex;
		bool m_isValid;
		std::thread m_thread;
	};
}