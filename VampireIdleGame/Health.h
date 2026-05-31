#pragma once
#include <Component.h>
#include <Event.h>

struct Health final : public Bloodforge::Component<Health>
{
	void Heal(float amount)
	{
		CurrentHealth += amount;
		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
		OnHealed.Invoke(OwnerEntityId, amount, CurrentHealth);
	}

	void Damage(float amount)
	{
		CurrentHealth -= amount;
		OnDamaged.Invoke(OwnerEntityId, amount, CurrentHealth);
		if (CurrentHealth <= 0)
		{
			CurrentHealth = 0;
			OnDeath.Invoke(OwnerEntityId);
		}
	}

	float MaxHealth;
	float CurrentHealth;

	Bloodforge::Event<int, float, float> OnDamaged; // EntityId, amount of damage, current health
	Bloodforge::Event<int, float, float> OnHealed; // EntityId, amount of healing, current health
	Bloodforge::Event<int> OnDeath;
};