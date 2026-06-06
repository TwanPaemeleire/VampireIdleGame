#include "PlayerHeartSystem.h"
#include <EntityManager.h>
#include "PlayerHeart.h"
#include <BloodTime.h>
#include <RectColliderComponent.h>
#include "PlayerAttributes.h"
#include <GlobalEventHandler.h>
#include "EnemySpawnSystem.h"
#include "Health.h"
#include <SpriteAnimatorComponent.h>
#include <SpriteComponent.h>
#include <ResourceManager.h>
#include "BloodSplat.h"

using namespace Bloodforge;

void PlayerHeartSystem::OnStart()
{
	// Hook into enemy death event so that they can be removed from list when dead
	GlobalEventHandler::GetInstance().AddListener<EnemySpawnEventData>([this](const EnemySpawnEventData& eventData)
		{
			Entity& enemyEntity = EntityManager::GetInstance().GetEntity(eventData.EnemyEntityId);
			Health* enemyHealth = EntityManager::GetInstance().GetComponent<Health>(enemyEntity);
			enemyHealth->OnDeath.AddListener([](int enemyEntityId)
				{
					EntityView<PlayerHeart> playerHeartView = EntityManager::GetInstance().GetFirstEntityWithComponents<PlayerHeart>().value();
					PlayerHeart& playerHeart = playerHeartView.GetComponent<PlayerHeart>();
					playerHeart.EnemyEntityIdsInRange.erase(playerHeart.EnemyEntityIdIteratorsInRange[enemyEntityId]);
					playerHeart.EnemyEntityIdIteratorsInRange.erase(enemyEntityId);
				});			
		});

	// Hook into on collision enter event, so that enemies can be added to the list when entering the heart range
	EntityView<PlayerHeart, RectColliderComponent> playerHeartView = EntityManager::GetInstance().GetFirstEntityWithComponents<PlayerHeart, RectColliderComponent>().value();
	RectColliderComponent& playerHeartCollider = playerHeartView.GetComponent<RectColliderComponent>();
	playerHeartCollider.OnCollisionEnterEvent.AddListener([](int, int otherEntityId)
		{
			if (EntityManager::GetInstance().GetEntity(otherEntityId).Tag != CreateId("Enemy")) return;
			EntityView<PlayerHeart> playerHeartView = EntityManager::GetInstance().GetFirstEntityWithComponents<PlayerHeart>().value();
			PlayerHeart& playerHeart = playerHeartView.GetComponent<PlayerHeart>();
			if (playerHeart.EnemyEntityIdIteratorsInRange.contains(otherEntityId)) return;
			playerHeart.EnemyEntityIdsInRange.push_back(otherEntityId);
			playerHeart.EnemyEntityIdIteratorsInRange[otherEntityId] = std::prev(playerHeart.EnemyEntityIdsInRange.end());
		});

	playerHeartCollider.OnCollisionExitEvent.AddListener([](int, int otherEntityId)
		{
			if (EntityManager::GetInstance().GetEntity(otherEntityId).Tag != CreateId("PlayerProjectile")) return;
			EntityManager::GetInstance().DestroyEntity(otherEntityId);
		});
}

void PlayerHeartSystem::OnUpdate()
{
	EntityView<PlayerAttributes> playerAttributesView = EntityManager::GetInstance().GetOrCreateFirstEntityWithComponents<PlayerAttributes>();
	PlayerAttributes& playerAttributes = playerAttributesView.GetComponent<PlayerAttributes>();

	EntityView<PlayerHeart> playerHeartview = EntityManager::GetInstance().GetFirstEntityWithComponents<PlayerHeart>().value();
	PlayerHeart& playerHeart = playerHeartview.GetComponent<PlayerHeart>();
	for (auto& [weaponType, cooldownLeft] : playerHeart.WeaponsNextAvailableTime)
	{
		if (cooldownLeft < 0.0f && playerHeart.EnemyEntityIdsInRange.size() == 0) return;
		cooldownLeft -= BloodTime::GetInstance().DeltaTime;
		if (cooldownLeft < 0.0f && playerHeart.EnemyEntityIdsInRange.size() != 0)
		{
			switch (weaponType)
			{
			case HeartWeaponType::BloodSplat:
				cooldownLeft += playerAttributes.Attributes[CreateId("BloodSplatCooldown")].CurrentValue;
				DoBloodSplat(playerHeart);
				break;
			default:
				break;
			}
		}
	}
}

void PlayerHeartSystem::DoBloodSplat(PlayerHeart& playerHeart)
{
	EntityManager& entityManager = EntityManager::GetInstance();
	int enemyEntityId = playerHeart.EnemyEntityIdsInRange.front();

	TransformComponent* enemyTransform = entityManager.GetComponent<TransformComponent>(enemyEntityId);

	Entity& bloodSplatEntity = entityManager.CreateEntity();
	bloodSplatEntity.Tag = CreateId("PlayerProjectile");
	int bloodSplatEntityId = bloodSplatEntity.Id;

	entityManager.AddComponent<SpriteComponent>(bloodSplatEntityId);
	SpriteAnimatorComponent* animatorComp = entityManager.AddComponent<SpriteAnimatorComponent>(bloodSplatEntityId);

	AnimationData movementAnimData;
	movementAnimData.FrameTime = 0.06f;
	movementAnimData.ShouldLoop = true;
	movementAnimData.NumberOfFrames = 8;
	movementAnimData.Texture = ResourceManager::GetInstance().LoadTexture("Heart/Projectiles/BloodSplatMovement.png");
	animatorComp->AddAnimation(CreateId("Movement"), movementAnimData);
	animatorComp->PlayAnimation(CreateId("Movement"));

	TransformComponent* bloodSplatTransform = entityManager.GetComponent<TransformComponent>(bloodSplatEntityId);
	Vector2 projectilePos = entityManager.GetComponent<TransformComponent>(playerHeart.OwnerEntityId)->GetWorldPosition();
	bloodSplatTransform->SetLocalPosition(projectilePos);
	bloodSplatTransform->SetLocalScale({ 0.3f, 0.3f });

	BloodSplat* bloodSplatComp = entityManager.AddComponent<BloodSplat>(bloodSplatEntityId);
	bloodSplatComp->Direction = enemyTransform->GetWorldPosition() - projectilePos;
	bloodSplatComp->Direction.Normalize();

	RectColliderComponent* rectColliderComp = entityManager.AddComponent<RectColliderComponent>(bloodSplatEntityId);
	rectColliderComp->IgnoreTags.insert(CreateId("PlayerProjectile"));
	rectColliderComp->OnCollisionEnterEvent.AddListener([](int selfId, int otherId)
		{
			Entity& otherEntity = EntityManager::GetInstance().GetEntity(otherId);
			if (otherEntity.Tag != CreateId("Enemy")) return;

			EntityView<PlayerAttributes> playerAttributesView = EntityManager::GetInstance().GetOrCreateFirstEntityWithComponents<PlayerAttributes>();
			PlayerAttributes& playerAttributes = playerAttributesView.GetComponent<PlayerAttributes>();
			EntityManager::GetInstance().GetComponent<Health>(otherId)->Damage(playerAttributes.Attributes[CreateId("BloodSplatDamage")].CurrentValue);
			EntityManager::GetInstance().DestroyEntity(selfId);
		});
}