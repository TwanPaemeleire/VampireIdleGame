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

#include <SceneDataComponent.h>
#include <SceneManagingDataComponent.h>
#include <SpriteAnimatorComponent.h>
#include <SpriteComponent.h>

void LoadFunction()
{
	Bloodforge::EntityManager& entityManager = Bloodforge::EntityManager::GetInstance();
	Bloodforge::ResourceManager& resourceManager = Bloodforge::ResourceManager::GetInstance();
	Bloodforge::BloodRenderer& renderer = Bloodforge::BloodRenderer::GetInstance();

	{
		Bloodforge::Entity& heartEntity = entityManager.CreateEntity();
		int heartEntityId = heartEntity.Id;
		entityManager.AddComponent<Bloodforge::SpriteComponent>(heartEntityId);
		Bloodforge::SpriteAnimatorComponent* animator = entityManager.AddComponent<Bloodforge::SpriteAnimatorComponent>(heartEntityId);

		Bloodforge::AnimationData animData;
		animData.Texture = resourceManager.LoadTexture("Heart/Idle.png");
		animData.FrameTime = 0.06f;
		animData.ShouldLoop = true;
		animData.NumberOfFrames = 15;
		animator->AddAnimation(CreateId("IdleAnim"), animData);

		animator->PlayAnimation(CreateId("IdleAnim"));

		Bloodforge::TransformComponent* transformComp = entityManager.GetComponent<Bloodforge::TransformComponent>(heartEntityId);
		transformComp->SetLocalPosition(renderer.GetWindowWidth() / 2.0f, renderer.GetWindowHeight() / 2.0f);
		transformComp->SetLocalScale({ 0.75f, 0.75f });
	}
}

int main(int, char* [])
{
	Bloodforge::Bloodforge& engine = Bloodforge::Bloodforge::GetInstance();
	engine.SetResourcesDirectory("Resources");

	Bloodforge::WindowUtils::SetWindowAlwaysOnTop(true);
	Bloodforge::WindowUtils::SetWindowBordered(false);
	Bloodforge::WindowUtils::SetWindowFullScreen(false);
	Bloodforge::WindowUtils::SetWindowSize(350, 350);
	Bloodforge::WindowUtils::SetWindowTitle("VampireIdleGame");

	Bloodforge::EntityManager& entityManager = Bloodforge::EntityManager::GetInstance();

	{
		Bloodforge::Entity& sceneDataEntity = entityManager.CreateEntity();
		int entityId = sceneDataEntity.Id;
		Bloodforge::SceneDataComponent* sceneData = entityManager.AddComponent<Bloodforge::SceneDataComponent>(entityId);
		sceneData->LoadFunction = LoadFunction;

		Bloodforge::EntityView<Bloodforge::SceneManagingDataComponent> view = entityManager.GetOrCreateFirstEntityWithComponents<Bloodforge::SceneManagingDataComponent>();
		Bloodforge::SceneManagingDataComponent& sceneManagingData = view.GetComponent<Bloodforge::SceneManagingDataComponent>();
		sceneManagingData.SceneToLoadDataEntityId = entityId;
		sceneManagingData.ShouldLoadScene = true;
	}

	engine.Run();
	return 0;
}