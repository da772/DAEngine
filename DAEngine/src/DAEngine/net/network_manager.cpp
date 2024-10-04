
#include "network_manager.h"
#include "network_server.h"
#include "network_client.h"
#include "core/logger.h"
#include <enet/enet.h>

#ifdef DA_REVIEW
#include "debug/debug_menu_bar.h"
#include <imgui.h>
#endif

namespace da
{
#ifdef DA_REVIEW
	da::INetwork* CNetworkManager::sm_network = nullptr;
	bool CNetworkManager::sm_debug = false;
#endif
	std::unordered_map<uint64_t, INetwork*> CNetworkManager::sm_networks;
	uint64_t CNetworkManager::sm_count;

	void CNetworkManager::initialize()
	{
		enet_initialize();
#ifdef DA_REVIEW
		da::CDebugMenuBar::register_debug(HASHSTR("Network"), HASHSTR("Network Manager"), &sm_debug, []() {debugRender(); });
#endif
	}

	void CNetworkManager::shutdown()
	{
		enet_deinitialize();
		for (const std::pair<uint64_t, INetwork*>& kv : sm_networks) {
			delete kv.second;
		}
		sm_networks = {};
#ifdef DA_REVIEW
		da::CDebugMenuBar::unregister_debug(HASHSTR("Network"), HASHSTR("Network Manager"));
#endif
	}

	void CNetworkManager::createServer(const FNetworkSettings& settings, const std::function<void(INetwork*)>& callback)
	{
		uint64_t id = ++sm_count;
		CNetworkServer* server = new CNetworkServer(id, settings, [id, callback](bool b) {
			const std::unordered_map<uint64_t, INetwork*>::iterator& it = sm_networks.find(id);

			INetwork* ptr = it->second;

			if (!b) {
				delete it->second;
				sm_networks.erase(it);
				ptr = nullptr;
			}

			callback(ptr);
		});
		sm_networks[id] = server;
	}

	void CNetworkManager::joinServer(const FConnectionSettings& settings, const std::function<void(INetwork*)>& callback)
	{
		uint64_t id = ++sm_count;
		CNetworkClient* client = new CNetworkClient(id ,settings, [id, callback](bool b) {
			const std::unordered_map<uint64_t, INetwork*>::iterator& it = sm_networks.find(id);

			INetwork* ptr = it->second;

			if (!b) {
				delete it->second;
				sm_networks.erase(it);
				ptr = nullptr;
			}

			callback(ptr);
		});
		sm_networks[id] = client;
	}

	void CNetworkManager::destroy(INetwork* network)
	{
		ASSERT(network);
		
		const std::unordered_map<uint64_t, INetwork*>::iterator& it = sm_networks.find(network->getId());

		if (it != sm_networks.end()) {
			sm_networks.erase(it);
		}
		
		delete network;
	}

	da::INetwork* CNetworkManager::getNetwork()
	{
		for (const std::pair<uint64_t, INetwork*>& kv : sm_networks) {
			return kv.second;
		}

		return nullptr;
	}

#ifdef DA_REVIEW
	void CNetworkManager::debugRender()
	{
		static bool loading = false;

		if (ImGui::Begin("Network Manager", &sm_debug)) {

			if (loading) {
				ImGui::Text("Connecting...");
				ImGui::End();
				return;
			}

			if (sm_network) {
				ImGui::Text("Connected...");
				static std::string packet = "hello";
				char packetBuffer[255];
				packetBuffer[0] = '\0';
				memcpy(packetBuffer, packet.c_str(), packet.size() + 1);

				if (ImGui::InputText("##packetBuffer", packetBuffer, 255)) {
					packet = std::string(packetBuffer);
				}

				ImGui::SameLine();
				if (ImGui::Button("Send Packet")) {
					sm_network->sendPacket(packet.data(), packet.size() + 1, true);
				}

				if (ImGui::Button("Disconnect")) {
					destroy(sm_network);
					sm_network = nullptr;
				}
			}
			else {
				if (ImGui::Button("Create server")) {
					FNetworkSettings settings;
					createServer(settings, [](INetwork* net) {
						loading = false;
						sm_network = net;
						});
					loading = true;
				}

				static std::string ip = "127.0.0.1";
				char ipBuffer[255];
				ipBuffer[0] = '\0';
				memcpy(ipBuffer, ip.c_str(), ip.size() + 1);

				if (ImGui::InputText("###ip", ipBuffer, 255)) {
					ip = std::string(ipBuffer);
				}
				ImGui::SameLine();
				if (ImGui::Button("Connect")) {
					FConnectionSettings settings;
					settings.IP = ip;
					joinServer(settings, [](INetwork* net) {
						loading = false;
						sm_network = net;
						});
					loading = true;
				}

			}

		}

		ImGui::End();
	}
#endif

}