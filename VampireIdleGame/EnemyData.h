#pragma once
#include <Component.h>
#include <vector>
#include "StatusEffects.h"
#include <Vector2.h>
#include "EnemyAttributes.h"

struct EnemyData final : public Bloodforge::Component<EnemyData>
{
	EnemyType EnemyType;
	std::vector<StatusEffectData> AppliedStatusEffects;
	Bloodforge::Vector2 TargetPosition;
};