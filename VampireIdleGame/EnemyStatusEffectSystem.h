#pragma once
#include <System.h>

class EnemyStatusEffectSystem final : public Bloodforge::System
{
public:
	virtual void OnUpdate() override;
};