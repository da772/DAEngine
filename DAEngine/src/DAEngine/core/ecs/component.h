#pragma once
#include "core/ecs/scene.h"
#include "core/ecs/entity.h"

#define COMPONENT_H_INTERNAL(x) public: static void registerComponent(); inline static const CHashString getTypeHash() {return HASHSTR(#x);};\
inline const da::CGuid& getId() const {return m_guid;}; protected: da::CGuid m_guid; da::CEntity& m_parent; public: inline void initialize() { onInitialize(); }; inline void update(float dt) { onUpdate(dt);}; \
inline void shutdown() { onShutdown(); }; inline da::CEntity& getParent() const { return m_parent;};

#define COMPONENT_H_INTERNAL_NO_UPDATE(x) public: static void registerComponent(); inline static const CHashString getTypeHash() {return HASHSTR(#x);};\
inline const da::CGuid& getId() const {return m_guid;}; protected: da::CGuid m_guid; da::CEntity& m_parent; public: inline void initialize() { onInitialize(); }; \
inline void shutdown() { onShutdown(); }; inline da::CEntity& getParent() const { return m_parent;};

#ifdef DA_REVIEW
#define COMPONENT_H_DEBUG(x) COMPONENT_H_INTERNAL(x) public: inline void debugRender() { onDebugRender(); }
#define COMPONENT_H_NO_UPDATE_DEBUG(x) COMPONENT_H_INTERNAL_NO_UPDATE(x) public: inline void debugRender() { onDebugRender(); }
#endif

#define COMPONENT_H(x) COMPONENT_H_INTERNAL(x)
#define COMPONENT_H_NO_UPDATE(x) COMPONENT_H_INTERNAL_NO_UPDATE(x)

#define COMPONENT_REGISTER(x) void x::registerComponent() { da::CScene::registerComponentLifeCycle<x>({ register_init, register_update, register_shutdown }); }
#define COMPONENT_REGISTER_NO_UPDATE(x) void x::registerComponent() { da::CScene::registerComponentLifeCycle<x>({ register_init, nullptr, register_shutdown }); }
#ifdef DA_REVIEW
#define COMPONENT_REGISTER_DEBUG(x) void x::registerComponent() { da::CScene::registerComponentLifeCycle<x>({ register_init, register_update, register_shutdown, register_debug_render }); }
#define COMPONENT_REGISTER_NO_UPDATE_DEBUG(x) void x::registerComponent() { da::CScene::registerComponentLifeCycle<x>({ register_init, nullptr, register_shutdown, register_debug_render }); }
#endif

#define COMPONENT_CPP_INTERNAL(x) static void register_init(void* p) { ((x*)p)->initialize();} static void register_update(void* p, float dt) { PROFILE(#x"::update") ((x*)p)->update(dt);} static void register_shutdown(void* p) { ((x*)p)->shutdown();}
#define COMPONENT_CPP_INTERNAL_NO_UPDATE(x) static void register_init(void* p) { ((x*)p)->initialize();} static void register_shutdown(void* p) { ((x*)p)->shutdown();}

#define COMPONENT_CPP(x) COMPONENT_CPP_INTERNAL(x) COMPONENT_REGISTER(x)
#define COMPONENT_CPP_NO_UPDATE(x) COMPONENT_CPP_INTERNAL_NO_UPDATE(x) COMPONENT_REGISTER_NO_UPDATE(x)
#ifdef DA_REVIEW
#define COMPONENT_CPP_DEBUG(x) COMPONENT_CPP_INTERNAL(x)  static void register_debug_render(void* p) { ((x*)p)->debugRender();}  COMPONENT_REGISTER_DEBUG(x) 
#define COMPONENT_CPP_NO_UPDATE_DEBUG(x) COMPONENT_CPP_INTERNAL_NO_UPDATE(x)  static void register_debug_render(void* p) { ((x*)p)->debugRender();}  COMPONENT_REGISTER_NO_UPDATE_DEBUG(x) 
#endif

namespace da
{
	class CComponent {
	public:
		CComponent(const da::CGuid& guid, da::CEntity& parent);
		const da::CGuid& getId() const;

	protected:
		static const CHashString& getTypeHash();

	protected:
		da::CGuid m_guid;
		da::CEntity& m_parent;
	};
}
