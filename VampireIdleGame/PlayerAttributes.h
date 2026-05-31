#pragma once
#include <Component.h>

struct PlayerAttributes final : Bloodforge::Component<PlayerAttributes>
{
	float FireRatePerSecond;
	float Health;
	float RegenerateDelay;
	float RegenerateAmount;

	// Player equipment?
	// PLAYER BLOOD
	// Different blood that gives different stat boosts, dropped by enemies, can be fused together to upgrade quality

};