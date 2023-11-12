#pragma once
#include "scene.h"

#define COMPONENT_H(x) public: static void registerComponent(); inline static const CHashString getTypeHash() {return HASHSTR(#x);}; \
inline const CGuid& getId() const {return m_guid;}; protected: CGuid m_guid; public: inline void initialize() { onInitialize(); }; inline void update(float dt) { onUpdate(dt);}; \
inline void shutdown() { onShutdown(); };

#define COMPONENT_CPP(x) static void register_init(void* p) { ((x*)p)->initialize();} static void register_update(void* p, float dt) { ((x*)p)->update(dt);} static void register_shutdown(void* p) { ((x*)p)->shutdown();}\
void x::registerComponent() { CScene::registerComponentLifeCycle<x>({ register_init, register_update, register_shutdown }); }

namespace da::core
{
	class CComponent {
	public:
		CComponent(const CGuid& guid);
		const CGuid& getId() const;

	protected:
		static const CHashString& getTypeHash();

	protected:
		CGuid m_guid;
	};
}
