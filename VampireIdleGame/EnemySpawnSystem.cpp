#include "EnemySpawnSystem.h"
#include "EnemySpawningData.h"
#include <EntityManager.h>
#include <BloodTime.h>
#include "EnemyAttributes.h"
#include "EnemyData.h"
#include "Health.h"
#include <GlobalEventHandler.h>
#include <SpriteComponent.h>
#include <SpriteAnimatorComponent.h>
#include <IdCreator.h>
#include <ResourceManager.h>
#include "PlayerHeart.h"
#include <TransformComponent.h>
#include <random>
#include <numbers>
#include <RectColliderComponent.h>

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
			if (enemyAttributes.Attributes.contains(randEnemyType))
			{
				enemySpawningData.CurrentEnemyToSaveFor = randEnemyType;
			}
		}
		if (enemyAttributes.Attributes[enemySpawningData.CurrentEnemyToSaveFor].PointCost > enemySpawningData.PointsAvailable) // Cant afford
		{
			shouldKeepSpawning = false;
			continue;
		}

		EntityView<PlayerHeart, TransformComponent> playerHeartView = entityManager.GetFirstEntityWithComponents<PlayerHeart, TransformComponent>().value();
		TransformComponent& playerHeartTransform = playerHeartView.GetComponent<TransformComponent>();

		// Spawn enemy
		Entity& enemyEntity = entityManager.CreateEntity();
		enemyEntity.Tag = CreateId("Enemy");
		int enemyEntityId = enemyEntity.Id;
		IndividualEnemyAttributes& attributes = enemyAttributes.Attributes[enemySpawningData.CurrentEnemyToSaveFor];

		EnemyData* enemyData = entityManager.AddComponent<EnemyData>(enemyEntityId);
		enemyData->EnemyType = enemySpawningData.CurrentEnemyToSaveFor;
		enemyData->TargetPosition = playerHeartTransform.GetWorldPosition();

		Health* health = entityManager.AddComponent<Health>(enemyEntityId);
		health->MaxHealth = attributes.Health;
		health->CurrentHealth = attributes.Health;
		health->OnDeath.AddListener([](int entityId)
			{
				EntityManager::GetInstance().DestroyEntity(entityId);
			});

		SpriteComponent* spriteComp = entityManager.AddComponent<SpriteComponent>(enemyEntityId);
		SpriteAnimatorComponent* animator = entityManager.AddComponent<SpriteAnimatorComponent>(enemyEntityId);

		AnimationData movementAnimationData;
		movementAnimationData.FrameTime = 0.18f;
		movementAnimationData.NumberOfFrames = 10;
		movementAnimationData.ShouldLoop = true;
		movementAnimationData.Texture = ResourceManager::GetInstance().LoadTexture("Enemies/Eye/EyeMovement.png");
		animator->AddAnimation(CreateId("EyeMovement"), movementAnimationData);
		animator->PlayAnimation(CreateId("EyeMovement"));

		TransformComponent* enemyTransform = entityManager.GetComponent<TransformComponent>(enemyEntityId);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dist(0.0, 2 * std::numbers::pi);
		
		double angle = dist(gen);
		float x = enemySpawningData.SpawnRadius * static_cast<float>(std::cos(angle)) + playerHeartTransform.GetWorldPosition().X;
		float y = enemySpawningData.SpawnRadius * static_cast<float>(std::sin(angle)) + playerHeartTransform.GetWorldPosition().Y;

		enemyTransform->SetLocalPosition({x, y});
		enemyTransform->SetLocalScale({ 0.75f, 0.75f });
		spriteComp = entityManager.AddComponent<SpriteComponent>(enemyEntityId);

		if (playerHeartTransform.GetWorldPosition().X < enemyTransform->GetWorldPosition().X)
		{
			spriteComp->FlipHorizontal = true;
		}

		RectColliderComponent* rectCollider = entityManager.AddComponent<RectColliderComponent>(enemyEntityId);
		rectCollider->SetSize({19.0f, 19.0f});

		GlobalEventHandler::GetInstance().InvokeEvent<EnemySpawnEventData>({ enemyEntityId });

		enemySpawningData.PointsAvailable -= enemyAttributes.Attributes[enemySpawningData.CurrentEnemyToSaveFor].PointCost;
		enemySpawningData.CurrentEnemyToSaveFor = EnemyType::ENEMYTYPE_END;
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
