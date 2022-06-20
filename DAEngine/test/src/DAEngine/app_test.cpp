#include "app_test.h"
#include <DAEngine/app.h>

#include <DAEngine/logger.h>

class CTestApp : public da::CApp
{
public:
	CTestApp() : da::CApp(0, 0) { }

	bool m_initalized, m_updated, m_shutdown;
protected:
	void onInitalize() override
	{
		m_initalized = true;
	}

	void onUpdate() override
	{
		m_updated = true;
		forceEnd();
	}

	void onShutdown() override
	{
		m_shutdown = true;
	}

};

CAppTest::CAppTest()
{

}

bool CAppTest::RunTests()
{
	TEST_BEGIN();
	TEST_FUNC(AppTests);
	TEST_END();
}

bool CAppTest::AppTests()
{
	CTestApp* app = new CTestApp();

	app->initalize();
	TEST_ASSERT(app->m_initalized);
	app->update();
	TEST_ASSERT(app->m_updated);
	app->shutdown();
	TEST_ASSERT(app->m_shutdown);

	delete app;

	return true;
}
