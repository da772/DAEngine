#pragma once

#include "network_interface.h"

namespace da::net
{
	class CNetworkManager
	{
	public:
		static void initialize();
		static void shutdown();

		static void createServer(const FNetworkSettings& settings, const std::function<void(INetwork*)>& callback);
		static void joinServer(const FConnectionSettings& settings, const std::function<void(INetwork*)>& callback);

		static INetwork* getNetwork();

		static void destroy(INetwork* network);

	private:
#ifdef DA_REVIEW
		static void debugRender();
#endif

	private:
		static std::unordered_map<uint64_t, INetwork*> sm_networks;
		static uint64_t sm_count;
#ifdef DA_REVIEW
		static bool sm_debug;
		static INetwork* sm_network;
#endif
	};
}