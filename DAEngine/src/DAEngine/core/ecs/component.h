#pragma once
#include "scene.h"
#include "entity.h"

#define COMPONENT_H_INTERNAL(x) public: static void registerComponent(); inline static const CHashString getTypeHash() {return HASHSTR(#x);};\
inline const CGuid& getId() const {return m_guid;}; protected: CGuid m_guid; CEntity& m_parent; public: inline void initialize() { onInitialize(); }; inline void update(float dt) { onUpdate(dt);}; \
inline void shutdown() { onShutdown(); }; inline CEntity& getParent() const { return m_parent;};

#ifdef DA_DEBUG
#define COMPONENT_H_DEBUG(x) COMPONENT_H_INTERNAL(x) public: inline void debugRender() { onDebugRender(); }
#endif

#define COMPONENT_H(x) COMPONENT_H_INTERNAL(x)

#define COMPONENT_REGISTER(x) void x::registerComponent() { CScene::registerComponentLifeCycle<x>({ register_init, register_update, register_shutdown }); }
#ifdef DA_DEBUG
#define COMPONENT_REGISTER_DEBUG(x) void x::registerComponent() { CScene::registerComponentLifeCycle<x>({ register_init, register_update, register_shutdown, register_debug_render }); }
#endif

#define COMPONENT_CPP_INTERNAL(x) static void register_init(void* p) { ((x*)p)->initialize();} static void register_update(void* p, float dt) { ((x*)p)->update(dt);} static void register_shutdown(void* p) { ((x*)p)->shutdown();}

#define COMPONENT_CPP(x) COMPONENT_CPP_INTERNAL(x) COMPONENT_REGISTER(x)

#ifdef DA_DEBUG
#define COMPONENT_CPP_DEBUG(x) COMPONENT_CPP_INTERNAL(x)  static void register_debug_render(void* p) { ((x*)p)->debugRender();}  COMPONENT_REGISTER_DEBUG(x) 
#endif


namespace da::core
{
	class CComponent {
	public:
		CComponent(const CGuid& guid, CEntity& parent);
		const CGuid& getId() const;

	protected:
		static const CHashString& getTypeHash();

	protected:
		CGuid m_guid;
		CEntity& m_parent;
	};
}
