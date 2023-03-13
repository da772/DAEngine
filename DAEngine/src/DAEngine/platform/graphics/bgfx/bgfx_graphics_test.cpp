#include "dapch.h"

#include "bgfx_graphics_test.h"

#ifdef DA_GRAPHICS_BGFX

#include <imgui.h>
#include <functional>
#include "bgfx_graphics_test_01.h"

namespace da::platform {

    struct FTestFunc {
        std::function<void*()> Create;
        std::function<void(void*, da::core::CWindow* window)> Init;
        std::function<void(void*)> Render;
        std::function<void(void*)> Shutdown;
    };

    const char* s_testNames[] = { "Test1" };

    FTestFunc s_testCreate[] = { 
        /// Test1
        {[] () {return new CBgfxGraphicsTest01();}
        ,[] (void* p, da::core::CWindow* w) {((CBgfxGraphicsTest01*)p)->Initialize(w);}
        ,[] (void* p) {((CBgfxGraphicsTest01*)p)->Render();}
        ,[] (void* p) {((CBgfxGraphicsTest01*)p)->Shutdown();} }


         };

    void CBgfxGraphicsTest::Initialize(class da::core::CWindow* window) {
        m_window = window;
    }

    void CBgfxGraphicsTest::Render() {
        
        if (ImGui::Begin("Tests", &m_show)) {
            if (!m_test && ImGui::BeginCombo("##name", s_testNames[m_testIndex])) {
                for (int i = 0; i < sizeof(s_testNames)/sizeof(uintptr_t); i++) {
                    bool selected = m_testIndex == i;
                    if (ImGui::Selectable(s_testNames[i], selected)) {
                        m_testIndex = i;
                    }

                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (!m_test) {
                if (ImGui::Button("Start")) {
                    m_test = s_testCreate[m_testIndex].Create();
                    s_testCreate[m_testIndex].Init(m_test, m_window);
                }
            }

            if (m_test) {
                ImGui::Text(("Running: " + std::string(s_testNames[m_testIndex])).c_str());
                if (ImGui::Button("Stop")) {
                    s_testCreate[m_testIndex].Shutdown(m_test);
                    delete m_test;
                    m_test = nullptr;
                }
            }

            ImGui::End();
        }

        if (m_test) s_testCreate[m_testIndex].Render(m_test);
    }

    void CBgfxGraphicsTest::Shutdown() {
        if (m_test) s_testCreate[m_testIndex].Shutdown(m_test);
        if (m_test) delete m_test;
        m_test = nullptr;
    }
}

#endif
