#pragma once
#include <Component.h>
#include "EnemyAttributes.h"

struct EnemySpawningData final : public Bloodforge::Component<EnemySpawningData>
{
	float EnemyPointsPerSecond = 2.0f;
	float PointsAvailable = 0;
	EnemyType CurrentEnemyToSaveFor = EnemyType::ENEMYTYPE_END;
	float TimeSinceEnemyReachedTriggerRange = 0.0f;
	float TimeToIncreasePointsPerSecond = 2.0f;
	float PointIncreaseImmunityTimeAfterChange = 2.0f;
	float PointIncreaseImmunityTimer = 0.0f;
	float PointsPerSecondIncreaseAmount = 0;
	float SpawnRadius = 300.0f;
};