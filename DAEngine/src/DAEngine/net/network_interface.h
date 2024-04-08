#pragma once

namespace da::net
{
	struct FNetworkInfo
	{
		std::string ExternalIP;
		std::string InternalIP;
	};

	struct FConnectionSettings
	{
		std::string IP = "127.0.0.1";
		uint16_t Port = 27001;
		uint16_t ListenRate = 16;
		uint16_t Timeout = 5000;
	};

	struct FNetworkSettings
	{
		uint16_t Port = 27001;
		uint16_t ListenRate = 16;
		uint8_t MaxClients = 32;
		uint8_t Channels = 2;
		uint32_t IncomingBroadcast = 0;
		uint32_t OutgoingBroadcast = 0;
	};

	class INetwork
	{
	public:
		inline INetwork(uint64_t id) : m_id(id) {}
		inline virtual ~INetwork() {};
		virtual void sendPacket(const char* data, size_t length, bool reliable = true) = 0;
		virtual bool isValid() const = 0;
		virtual void getPacket(uint32_t id, std::function<void(const char*, size_t)> func) = 0;
		virtual bool isHost() const = 0;
		inline void clearPackets() { m_funcs = {}; };

		template<typename T>
		inline void getPacket(uint32_t id, std::function<void(T)> func) {
			getPacket(id, [func](const char* data, size_t len) {
				if (sizeof(T) != len) return;
				T d;
				memcpy(&d, data, len);
				func(d);
			});
		}

		template<typename T>
		inline void sendPacket(T data, bool reliable = true) {
			sendPacket((const char*) & data, sizeof(data), reliable);
		}

		inline uint64_t getId() const { return m_id; }

	protected:
		std::unordered_map<uint32_t, std::function<void(const char*, size_t)>> m_funcs;
		
	private:
		uint64_t m_id;
	};
}