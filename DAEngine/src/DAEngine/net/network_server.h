#pragma once

#include "network_interface.h"

class Portfwd;
struct _ENetHost;

namespace da
{

	class CNetworkServer : public INetwork
	{
	public:
		CNetworkServer(uint64_t id, const FNetworkSettings& settings, const std::function<void(bool)>& callback);
		~CNetworkServer();

		virtual void sendPacket(const char* data, size_t length, bool reliable) override;
		virtual void getPacket(uint32_t id, std::function<void(const char*, size_t)> func) override;
		inline virtual bool isValid() const { return m_isValid; }
		inline virtual bool isHost() const override { return true; }

	private:
		void setupServer();
		void serverListen();
		void clean();

	private:
		FNetworkSettings m_settings;
		FNetworkInfo m_info;
		std::mutex m_mutex;
		Portfwd* m_portFwd = nullptr;
		_ENetHost* m_host = nullptr;
		bool m_isValid = false;
		std::thread m_thread = {};
	};
}