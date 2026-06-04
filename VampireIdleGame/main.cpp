#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <Bloodforge.h>
#include <WindowUtils.h>
#include <EntityManager.h>
#include <IdCreator.h>
#include <ResourceManager.h>
#include <BloodRenderer.h>
#include <SceneSystemManager.h>

#include <SceneDataComponent.h>
#include <SceneManagingDataComponent.h>
#include <SpriteAnimatorComponent.h>
#include <SpriteComponent.h>
#include "PlayerHeart.h"
#include <RectColliderComponent.h>

#include "EnemyMovementSystem.h"
#include "EnemySpawnSystem.h"
#include "PlayerHeartSystem.h"
#include "EnemyStatusEffectSystem.h"
#include "BloodSplatSystem.h"

void LoadFunction()
{
	Bloodforge::EntityManager& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::ResourceManager& resourceManager = Bloodforge::ResourceManager::GetInstance();
	Bloodforge::BloodRenderer& renderer = Bloodforge::BloodRenderer::GetInstance();

	{
		Bloodforge::Entity& heartEntity = entityManager.CreateEntity();
		int heartEntityId = heartEntity.Id;

		Bloodforge::Entity& heartVisualsEntity = entityManager.CreateEntity();
		int heartVisualsEntityId = heartVisualsEntity.Id;
		Bloodforge::TransformComponent* heartVisualsTransform = entityManager.GetComponent<Bloodforge::TransformComponent>(heartVisualsEntityId);
		heartVisualsTransform->SetParent(heartEntityId);
		heartVisualsTransform->SetLocalScale({ 0.75f, 0.75f });

		entityManager.AddComponent<Bloodforge::SpriteComponent>(heartVisualsEntityId);
		Bloodforge::SpriteAnimatorComponent* animator = entityManager.AddComponent<Bloodforge::SpriteAnimatorComponent>(heartVisualsEntityId);

		Bloodforge::AnimationData animDataAppear;
		animDataAppear.Texture = resourceManager.LoadTexture("Heart/Appear.png");
		animDataAppear.FrameTime = 0.06f;
		animDataAppear.ShouldLoop = false;
		animDataAppear.NumberOfFrames = 8;
		animator->AddAnimation(CreateId("AppearAnim"), animDataAppear);
		animator->AddAnimationEvent(CreateId("AppearAnim"), [](Bloodforge::SpriteAnimatorComponent& animator)
			{
				animator.PlayAnimation(CreateId("IdleAnim"));
			}, 7, 0.06f);

		Bloodforge::AnimationData animDataIdle;
		animDataIdle.Texture = resourceManager.LoadTexture("Heart/Idle.png");
		animDataIdle.FrameTime = 0.06f;
		animDataIdle.ShouldLoop = true;
		animDataIdle.NumberOfFrames = 15;
		animator->AddAnimation(CreateId("IdleAnim"), animDataIdle);

		animator->PlayAnimation(CreateId("AppearAnim"));

		Bloodforge::TransformComponent* transformComp = entityManager.GetComponent<Bloodforge::TransformComponent>(heartEntityId);
		transformComp->SetLocalPosition(renderer.GetWindowWidth() / 2.0f, renderer.GetWindowHeight() / 2.0f);

		entityManager.AddComponent<PlayerHeart>(heartEntityId);

		Bloodforge::RectColliderComponent* rectCollider = entityManager.AddComponent<Bloodforge::RectColliderComponent>(heartEntityId);
		rectCollider->SetSize({ static_cast<float>(renderer.GetWindowWidth() - 2), static_cast<float>(renderer.GetWindowHeight() - 2) });
	}
}

int main(int, char* [])
{
	Bloodforge::Bloodforge& engine = Bloodforge::Bloodforge::GetInstance();
	engine.SetResourcesDirectory("Resources");

	Bloodforge::WindowUtils::SetWindowAlwaysOnTop(false);
	Bloodforge::WindowUtils::SetWindowBordered(true);
	Bloodforge::WindowUtils::SetWindowFullScreen(false);
	Bloodforge::WindowUtils::SetWindowSize(350, 350);
	Bloodforge::WindowUtils::SetWindowTitle("VampireIdleGame");

	Bloodforge::EntityManager& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::SceneSystemManager& sceneSystemManager = Bloodforge::SceneSystemManager::GetInstance();
	sceneSystemManager.TryRegisterSystem<EnemyMovementSystem>();
	sceneSystemManager.TryRegisterSystem<EnemySpawnSystem>();
	sceneSystemManager.TryRegisterSystem<EnemyStatusEffectSystem>();
	sceneSystemManager.TryRegisterSystem<PlayerHeartSystem>();
	sceneSystemManager.TryRegisterSystem<BloodSplatSystem>();

	{
		Bloodforge::Entity& sceneDataEntity = entityManager.CreateEntity();
		sceneDataEntity.DontDestroyOnSceneSwitch = true;
		int entityId = sceneDataEntity.Id;
		Bloodforge::SceneDataComponent* sceneData = entityManager.AddComponent<Bloodforge::SceneDataComponent>(entityId);
		sceneData->LoadFunction = LoadFunction;

		Bloodforge::EntityView<Bloodforge::SceneManagingDataComponent> view = entityManager.GetOrCreateFirstEntityWithComponents<Bloodforge::SceneManagingDataComponent>();
		Bloodforge::SceneManagingDataComponent& sceneManagingData = view.GetComponent<Bloodforge::SceneManagingDataComponent>();
		entityManager.GetEntity(view.EntityId).DontDestroyOnSceneSwitch = true;
		sceneManagingData.SceneToLoadDataEntityId = entityId;
		sceneManagingData.ShouldLoadScene = true;
	}

	engine.Run();
	return 0;
}