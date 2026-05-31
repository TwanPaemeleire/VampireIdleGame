#pragma once
#include <Component.h>
#include <vector>
#include "StatusEffects.h"

struct EnemyData final : public Bloodforge::Component<EnemyData>
{
	float MovementSpeed;
	float AmountOfXPToDrop;
	std::vector<StatusEffectTypes> AppliedStatusEffects;
};