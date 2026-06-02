#include "EnemySpawnSystem.h"
#include "EnemySpawningData.h"
#include <EntityManager.h>
#include <BloodTime.h>
#include "EnemyAttributes.h"
#include "EnemyData.h"
#include "Health.h"

using namespace Bloodforge;

void EnemySpawnSystem::OnUpdate()
{
	UpdateAvailablePoints();
	EntityManager& entityManager = EntityManager::GetInstance();

	EntityView<EnemySpawningData> spawnDataView = entityManager.GetOrCreateFirstEntityWithComponents<EnemySpawningData>();
	EnemySpawningData& enemySpawningData = spawnDataView.GetComponent<EnemySpawningData>();
	enemySpawningData.PointsAvailable += enemySpawningData.EnemyPointsPerSecond * BloodTime::GetInstance().DeltaTime;

	EntityView<EnemyAttributes> enemyAttributesView = entityManager.GetOrCreateFirstEntityWithComponents<EnemyAttributes>();
	EnemyAttributes& enemyAttributes = enemyAttributesView.GetComponent<EnemyAttributes>();

	bool shouldKeepSpawning = true;
	while (shouldKeepSpawning)
	{
		if (enemySpawningData.CurrentEnemyToSaveFor == EnemyType::ENEMYTYPE_END) // We're not currently saving for an enemy, so pick a random one
		{
			int randomEnemyType = rand() % static_cast<int>(EnemyType::ENEMYTYPE_END);
			EnemyType randEnemyType = static_cast<EnemyType>(randomEnemyType);
			if (enemyAttributes.EnemyAttributes.contains(randEnemyType))
			{
				enemySpawningData.CurrentEnemyToSaveFor = randEnemyType;
			}
		}
		if (enemyAttributes.EnemyAttributes[enemySpawningData.CurrentEnemyToSaveFor].PointCost < enemySpawningData.PointsAvailable) // Cant afford
		{
			shouldKeepSpawning = false;
			continue;
		}
		enemySpawningData.PointsAvailable -= enemyAttributes.EnemyAttributes[enemySpawningData.CurrentEnemyToSaveFor].PointCost;
		enemySpawningData.CurrentEnemyToSaveFor = EnemyType::ENEMYTYPE_END;

		// Spawn enemy
		Entity& enemyEntity = entityManager.CreateEntity();
		IndividualEnemyAttributes& attributes = enemyAttributes.EnemyAttributes[enemySpawningData.CurrentEnemyToSaveFor];

		EnemyData* enemyData = entityManager.AddComponent<EnemyData>(enemyEntity);
		enemyData->MovementSpeed = attributes.MovementSpeed;
		enemyData->AmountOfXPToDrop = attributes.AmountOfXPToDrop;

		Health* health = entityManager.AddComponent<Health>(enemyEntity);
		health->MaxHealth = attributes.Health;
		health->CurrentHealth = attributes.Health;

	}
}

void EnemySpawnSystem::UpdateAvailablePoints()
{
	EntityManager& entityManager = EntityManager::GetInstance();

	EntityView<EnemySpawningData> spawnDataView = entityManager.GetOrCreateFirstEntityWithComponents<EnemySpawningData>();
	EnemySpawningData& enemySpawningData = spawnDataView.GetComponent<EnemySpawningData>();
	if(enemySpawningData.PointIncreaseImmunityTimer > 0.0f) // If there's immunity time after last points increase, decrease this first
	{
		enemySpawningData.PointIncreaseImmunityTimer -= BloodTime::GetInstance().DeltaTime;
		if (enemySpawningData.PointIncreaseImmunityTimer > 0.0f) return;
		enemySpawningData.PointIncreaseImmunityTimer = 0.0f;
	}

	if (enemySpawningData.TimeSinceEnemyReachedTriggerRange >= enemySpawningData.TimeToIncreasePointsPerSecond)
	{
		enemySpawningData.EnemyPointsPerSecond += enemySpawningData.PointsPerSecondIncreaseAmount;
		enemySpawningData.TimeSinceEnemyReachedTriggerRange = 0.0f;
		enemySpawningData.PointIncreaseImmunityTimer = enemySpawningData.PointIncreaseImmunityTimeAfterChange;
	}
	else
	{
		enemySpawningData.TimeSinceEnemyReachedTriggerRange += BloodTime::GetInstance().DeltaTime;
	}
}
