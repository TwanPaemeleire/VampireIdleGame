#pragma once
#include <System.h>

class UpgradeMenuSystem final : public Bloodforge::System
{
public:
	virtual void OnStart() override;
	virtual void OnRender() override;
};