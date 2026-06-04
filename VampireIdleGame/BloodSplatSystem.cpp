#include "BloodSplatSystem.h"
#include <EntityManager.h>
#include "BloodSplat.h"
#include <SpriteComponent.h>
#include <TransformComponent.h>
#include <BloodTime.h>
#include "PlayerAttributes.h"

using namespace Bloodforge;

void BloodSplatSystem::OnUpdate()
{
	EntityManager& entityManager = EntityManager::GetInstance();
	EntityQueryResult<BloodSplat, SpriteComponent, TransformComponent> result = entityManager.QueryEntities<BloodSplat, SpriteComponent, TransformComponent>();

	EntityView<PlayerAttributes> playerAttributesView = entityManager.GetOrCreateFirstEntityWithComponents<PlayerAttributes>();
	PlayerAttributes& playerAttributes = playerAttributesView.GetComponent<PlayerAttributes>();

	for (EntityView<BloodSplat, SpriteComponent, TransformComponent> view : result.EntityViews)
	{
		BloodSplat& bloodSplat = view.GetComponent<BloodSplat>();
		TransformComponent& transform = view.GetComponent<TransformComponent>();

		float radians = std::atan2(bloodSplat.Direction.Y, bloodSplat.Direction.X);
		float degrees = radians * Vector2::RadiansToDegrees;
		transform.SetLocalRotation(degrees);

		Vector2 forwardVec = transform.GetForwardVector();
		float movementSpeed = playerAttributes.Attributes[CreateId("BloodSplatSpeed")].CurrentValue;
		Vector2 newPos = transform.GetWorldPosition() + (forwardVec * BloodTime::GetInstance().DeltaTime * movementSpeed);
		transform.SetLocalPosition(newPos);
	}
}