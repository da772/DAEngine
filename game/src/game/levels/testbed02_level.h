#pragma once
#include <daengine.h>
#include <daengine/core.h>
#include <daengine/components.h>
#include <daengine/net.h>
#include "level.h"



class CVehicle;

#define TRANSFORM_PACKET_ID 123
#define PLAYER_JOIN_PACKET_ID 124
#define PLAYER_LEAVE_PACKET_ID 125

struct FTransformPacketInfo {
	uint32_t PacketId = TRANSFORM_PACKET_ID;
	uint64_t  PlayerId;
	glm::vec3 Position;
	glm::quat Rotation;

	FTransformPacketInfo() {};
	FTransformPacketInfo(uint64_t id, const glm::vec3& pos, const glm::quat& rot) : PlayerId(id), Position(pos), Rotation(rot) {

	}
	
};

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
	CTestBed02Level(CHashString name, da::modules::CWindowModule& window);
	void initialize();
	void update(float dt);
	void shutdown();

private:
	void playerMovePacket(FTransformPacketInfo data);

private:
	std::unordered_map<uint64_t, CVehicle*> m_vehicles;
	CVehicle* m_vehicle;
	da::core::CEntity* m_cube;
	da::modules::CWindowModule& m_window;
	da::net::INetwork* m_network;
	float m_timePassed = 0.f;
	uint64_t m_playerId;
	bool m_pushPlayerId = false;

};