#pragma once
#include <daengine.h>
#include <daengine/core.h>
#include <daengine/components.h>
#include <daengine/net.h>
#include "level.h"



class CVehicle;
#define PLAYER_JOIN_PACKET_ID 124
#define PLAYER_LEAVE_PACKET_ID 125


struct FPlayerJoinPacketInfo {
	uint32_t PacketId = PLAYER_JOIN_PACKET_ID;
	uint64_t  PlayerId;

	FPlayerJoinPacketInfo() {};
	FPlayerJoinPacketInfo(uint64_t id) : PlayerId(id) {

	}
};

struct FPlayerLeavePacketInfo {
	uint32_t PacketId = PLAYER_LEAVE_PACKET_ID;
	uint64_t  PlayerId;
	FPlayerLeavePacketInfo() {}
	FPlayerLeavePacketInfo(uint64_t id) : PlayerId(id) {

	}
};

class CTestBed02Level : public ILevel {

public:
	CTestBed02Level(CHashString name, da::modules::CGraphicsModule& graphics, da::modules::CWindowModule& window);
	void initialize();
	void update(float dt);
	void shutdown();


private:
	std::unordered_map<uint64_t, CVehicle*> m_vehicles;
	CVehicle* m_vehicle;
	da::core::CEntity* m_cube;
	da::modules::CWindowModule& m_window;
	da::net::INetwork* m_network;
	float m_timePassed = 0.f;
	uint64_t m_playerId;
	bool m_pushPlayerId = false;
	da::script::CScriptClass m_scrlevel;

};