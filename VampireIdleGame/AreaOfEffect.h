#pragma once
#include <Component.h>
#include <vector>
#include "StatusEffects.h"

struct AreaOfEffect final : public Bloodforge::Component<AreaOfEffect>
{
	float TimeToExpand;
	float MaxExpandScale;

	float DamagePerTick;
	float TimeBetweenTicks;

	std::vector<StatusEffectTypes> StatusEffectsToApply;
};