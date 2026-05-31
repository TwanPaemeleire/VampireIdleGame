#include "EnemyStatusEffectSystem.h"
#include <EntityManager.h>
#include "EnemyData.h"
#include "Health.h"
#include "StatusEffectsAttributes.h"
#include <BloodTime.h>

using namespace Bloodforge;

void EnemyStatusEffectSystem::OnUpdate()
{
	EntityManager& entityManager = EntityManager::GetInstance();
	StatusEffectsAttributes& statusEffectAttributes = entityManager.GetOrCreateFirstEntityWithComponents<StatusEffectsAttributes>().GetComponent<StatusEffectsAttributes>();
	EntityQueryResult<Health, EnemyData> result = entityManager.QueryEntities<Health, EnemyData>();
	for (EntityView<Health, EnemyData>& view : result.EntityViews)
	{
		EnemyData& enemyData = view.GetComponent<EnemyData>();
		if (enemyData.AppliedStatusEffects.size() == 0) continue;
		Health& health = view.GetComponent<Health>();
		for (StatusEffectData& statusEffectData : enemyData.AppliedStatusEffects)
		{
			statusEffectData.TimeTracker -= BloodTime::GetInstance().DeltaTime;
			statusEffectData.TimeBeforeRemoval -= BloodTime::GetInstance().DeltaTime;
			switch (statusEffectData.Type)
			{
			case StatusEffectTypes::Fire:
				if (statusEffectData.TimeTracker > 0.0f) continue;
				health.Damage(statusEffectAttributes.BurnDamagePerTick);
				statusEffectData.TimeTracker += statusEffectAttributes.BurnTickDelay;
				break;
			}
		}

		// Remove expired status effects
		enemyData.AppliedStatusEffects.erase(std::remove_if(enemyData.AppliedStatusEffects.begin(), enemyData.AppliedStatusEffects.end(),
			[](const StatusEffectData& effect)
			{
				return effect.TimeBeforeRemoval <= 0.0f;
			}), enemyData.AppliedStatusEffects.end());
	}
}