#pragma once
#include <Component.h>

struct PlayerAttributes final : Bloodforge::Component<PlayerAttributes>
{
	float FireRatePerSecond;
	float Health;
	float RegenerateDelay;
	float RegenerateAmount;

	// BLOOD STONES FOR HEART
	// Different blood stones that gives different stat boosts, dropped by enemies, can be fused together to upgrade quality

};