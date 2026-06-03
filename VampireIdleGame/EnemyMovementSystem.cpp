#include "EnemyMovementSystem.h"
#include "EnemyAttributes.h"
#include "EnemyData.h"
#include <TransformComponent.h>
#include <EntityManager.h>
#include <BloodTime.h>

using namespace Bloodforge;

void EnemyMovementSystem::OnUpdate()
{
	EntityManager& entityManager = EntityManager::GetInstance();

	EntityView<EnemyAttributes> attributesView = entityManager.GetOrCreateFirstEntityWithComponents<EnemyAttributes>();
	EnemyAttributes& enemyAttributes = attributesView.GetComponent<EnemyAttributes>();

	EntityQueryResult<EnemyData, TransformComponent> result = entityManager.QueryEntities<EnemyData, TransformComponent>();
	for (EntityView<EnemyData, TransformComponent>& view : result.EntityViews)
	{
		EnemyData& enemyData = view.GetComponent<EnemyData>();
		TransformComponent& transform = view.GetComponent<TransformComponent>();

		Vector2 pos = transform.GetWorldPosition();
		Vector2 direction = enemyData.TargetPosition - pos;
		direction.Normalize();
		pos += direction * enemyAttributes.Attributes[enemyData.EnemyType].MovementSpeed * BloodTime::GetInstance().DeltaTime;
		transform.SetLocalPosition(pos);
	}
}