#pragma once
#include <System.h>

class BloodSplatSystem final : public Bloodforge::System
{
public:
	virtual void OnUpdate() override;
};