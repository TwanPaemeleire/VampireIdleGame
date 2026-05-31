#pragma once
#include <Component.h>

struct StatusEffectsAttributes final : public Bloodforge::Component<StatusEffectsAttributes>
{
	// Burn
	float BurnDamagePerTick;
	float BurnTickDelay;
	float BurnDuration;

	// Stun
	float StunDuration;

	// Poison
	float PoisonDamagePerTick;
	float PoisonTickDelay;
	float PoisonDuration;

	// Slow
	float SlowDuration;
};