#include "UpgradeMenuSystem.h"
#include <EntityManager.h>
#include "PlayerAttributes.h"
#include "UpgradeMenu.h"
#include <ButtonComponent.h>
#include <ResourceManager.h>
#include <SpriteComponent.h>
#include <ResourceManager.h>
#include <BloodRenderer.h>
#include <TextComponent.h>

using namespace Bloodforge;

void UpgradeMenuSystem::OnStart()
{
	EntityManager& entityManager = EntityManager::GetInstance();

	Entity& upgradeMenuEntity = entityManager.CreateEntity();
	int upgradeMenuEntityId = upgradeMenuEntity.Id;
	{
		SpriteComponent* upgradeMenuBackground = entityManager.AddComponent<SpriteComponent>(upgradeMenuEntityId);
		upgradeMenuBackground->SetTexture(ResourceManager::GetInstance().LoadTexture("UI/FrameDigitalLarge2.png"));
		/*UpgradeMenu* upgradeMenu = */entityManager.AddComponent<UpgradeMenu>(upgradeMenuEntityId);
		TransformComponent* upgradeMenuTransform = entityManager.GetComponent<TransformComponent>(upgradeMenuEntityId);
		upgradeMenuTransform->SetLocalPosition(BloodRenderer::GetInstance().GetWindowWidth() / 2.0f, BloodRenderer::GetInstance().GetWindowHeight() / 2.0f);
	}

	EntityView<PlayerAttributes> playerAttributesView = entityManager.GetOrCreateFirstEntityWithComponents<PlayerAttributes>();
	PlayerAttributes& playerAttributes = playerAttributesView.GetComponent<PlayerAttributes>();

	Font* font = ResourceManager::GetInstance().LoadFont("ARCADE_N.TTF", 6.0f);

	float startY = -100.0f;
	float offset = 30.0f;
	float current = startY;

	for (const auto& [id, attribute] : playerAttributes.Attributes)
	{
		Entity& buttonEntity = entityManager.CreateEntity();
		int buttonEntityId = buttonEntity.Id;
		
		SpriteComponent* sprite = entityManager.AddComponent<SpriteComponent>(buttonEntityId);
		sprite->SetTexture(ResourceManager::GetInstance().LoadTexture("UI/ListItem.png"));
		ButtonComponent* buttonComp = entityManager.AddComponent<ButtonComponent>(buttonEntityId);
		buttonComp->NormalScale = { 1.0f, 1.0f };
		buttonComp->HoverScale = { 1.1f, 1.1f };
		buttonComp->TimeToReachHoverScale = 0.2f;
		buttonComp->TimeToReachNormalScale = 0.2f;
		buttonComp->NormalTexture = ResourceManager::GetInstance().LoadTexture("UI/ListItem.png");
		buttonComp->HoverTexture = ResourceManager::GetInstance().LoadTexture("UI/ListItemSelected.png");
		buttonComp->PressedTexture = ResourceManager::GetInstance().LoadTexture("UI/ListItemSelected.png");
		buttonComp->OnClick.AddListener([id](ButtonComponent&)
			{
				int temp = 200;
				EntityManager::GetInstance().GetOrCreateFirstEntityWithComponents<PlayerAttributes>().GetComponent<PlayerAttributes>().Attributes[id].TryBuyAndApplyUpgrade(temp);
			});
		TransformComponent* transform = entityManager.GetComponent<TransformComponent>(buttonEntityId);
		transform->SetParent(upgradeMenuEntityId, false);
		transform->SetLocalPosition({ 0.0f, current});
		
		{
			Entity& textEntity = entityManager.CreateEntity();
			int textEntityId = textEntity.Id;
			TextComponent* text = entityManager.AddComponent<TextComponent>(textEntityId);
			text->SetText(attribute.Name);
			text->SetFont(font);
			TransformComponent* textTransform = entityManager.GetComponent<TransformComponent>(textEntityId);
			textTransform->SetParent(buttonEntityId, false);
			textTransform->SetLocalPosition({ -20.0f, 0.0f });
		}

		{
			Entity& textEntity = entityManager.CreateEntity();
			int textEntityId = textEntity.Id;
			TextComponent* text = entityManager.AddComponent<TextComponent>(textEntityId);
			text->SetText(std::to_string(attribute.CurrentValue));
			text->SetFont(font);
			TransformComponent* textTransform = entityManager.GetComponent<TransformComponent>(textEntityId);
			textTransform->SetParent(buttonEntityId, false);
			textTransform->SetLocalPosition({ 60.0f, 0.0f });
		}

		{
			Entity& textEntity = entityManager.CreateEntity();
			int textEntityId = textEntity.Id;
			TextComponent* text = entityManager.AddComponent<TextComponent>(textEntityId);
			text->SetText(std::to_string(attribute.CurrentCost));
			text->SetFont(font);
			TransformComponent* textTransform = entityManager.GetComponent<TransformComponent>(textEntityId);
			textTransform->SetParent(buttonEntityId, false);
			textTransform->SetLocalPosition({ 80.0f, 0.0f });
		}
		
		current += offset;
	}

	Entity& buttonCompEntity = entityManager.CreateEntity();
	int buttonCompId = buttonCompEntity.Id;
	
	SpriteComponent* spriteComp = entityManager.AddComponent<SpriteComponent>(buttonCompId);
	spriteComp->SetTexture(ResourceManager::GetInstance().LoadTexture("UI/Button.png"));

	ButtonComponent* buttonComp = entityManager.AddComponent<ButtonComponent>(buttonCompId);
	buttonComp->NormalTexture = ResourceManager::GetInstance().LoadTexture("UI/Button.png");
	buttonComp->HoverTexture = ResourceManager::GetInstance().LoadTexture("UI/ButtonHovered.png");
	buttonComp->PressedTexture = ResourceManager::GetInstance().LoadTexture("UI/ButtonHovered.png");
	buttonComp->NormalScale = { 1.0f, 1.0f };
	buttonComp->HoverScale = { 1.0f, 1.0f };
	buttonComp->TimeToReachHoverScale = 0.1f;
	buttonComp->TimeToReachNormalScale = 0.1f;
	buttonComp->OnClick.AddListener([upgradeMenuEntityId](ButtonComponent&)
		{
			EntityManager::GetInstance().GetEntity(upgradeMenuEntityId).SetActive(!EntityManager::GetInstance().GetEntity(upgradeMenuEntityId).IsActive);
		});

	TransformComponent* buttonTransform = entityManager.GetComponent<TransformComponent>(buttonCompId);
	buttonTransform->SetLocalPosition({ BloodRenderer::GetInstance().GetWindowWidth() / 2.0f , 20.0f });

	EntityManager::GetInstance().GetEntity(upgradeMenuEntityId).SetActive(false);
}

void UpgradeMenuSystem::OnRender()
{
	// EntityManager& entityManager = EntityManager::GetInstance();
	// EntityView<PlayerAttributes> attributesView = entityManager.GetOrCreateFirstEntityWithComponents<PlayerAttributes>();
	// PlayerAttributes& playerAttributes = attributesView.GetComponent<PlayerAttributes>();
	// 
	// EntityView<UpgradeMenu> upgradeMenuView = entityManager.GetFirstEntityWithComponents<UpgradeMenu>().value();
	// UpgradeMenu& upgradeMenu = upgradeMenuView.GetComponent<UpgradeMenu>();

}