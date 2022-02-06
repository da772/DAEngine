#pragma once

namespace da
{
	class App
	{
	public:
		virtual void Begin();
		virtual void Run();
		virtual void End();
	};
	
	App* CreateApp();
}
