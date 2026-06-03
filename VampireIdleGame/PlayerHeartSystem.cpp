#include "PlayerHeartSystem.h"
#include <EntityManager.h>
#include "PlayerHeart.h"
#include <BloodTime.h>
#include <RectColliderComponent.h>
#include "PlayerAttributes.h"
#include <GlobalEventHandler.h>
#include "EnemySpawnSystem.h"
#include "Health.h"

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
	playerHeartCollider.OnCollisionEnterEvent.AddListener([this](int, int otherEntityId)
		{
			if (EntityManager::GetInstance().GetEntity(otherEntityId).Tag != CreateId("Enemy")) return;
			EntityView<PlayerHeart> playerHeartView = EntityManager::GetInstance().GetFirstEntityWithComponents<PlayerHeart>().value();
			PlayerHeart& playerHeart = playerHeartView.GetComponent<PlayerHeart>();
			if (playerHeart.EnemyEntityIdIteratorsInRange.contains(otherEntityId)) return;
			playerHeart.EnemyEntityIdsInRange.push_back(otherEntityId);
			playerHeart.EnemyEntityIdIteratorsInRange[otherEntityId] = std::prev(playerHeart.EnemyEntityIdsInRange.end());
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
		cooldownLeft -= BloodTime::GetInstance().DeltaTime;
		if (cooldownLeft < 0.0f)
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

void PlayerHeartSystem::DoBloodSplat(PlayerHeart& )//playerHeart)
{
	// int enemyEntityId = playerHeart.EnemyEntityIdsInRange.front();
}