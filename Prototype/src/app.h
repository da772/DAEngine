#pragma once

#include "daengine.h"

#include <iostream>

class ProtoTypeApp : public da::App {

public:
	virtual void Begin() override
	{
		std::cout << "Begin" << std::endl;
	}

	virtual void Run() override
	{
		std::cout << "Run" << std::endl;
	}

	virtual void End() override
	{
		std::cout << "End" << std::endl;
	}
};
