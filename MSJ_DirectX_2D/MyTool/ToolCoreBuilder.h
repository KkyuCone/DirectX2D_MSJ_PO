#pragma once
#include <Core.h>

class ToolCoreBuilder : public Core::My_CoreBuilder
{
public:
	bool Build() override;
public:
	ToolCoreBuilder();
	~ToolCoreBuilder();
};

