#pragma once
#include <Component.h>
#include <IdCreator.h>
#include <unordered_map>
#include <Event.h>
#include <EntityManager.h>

using AttributeId = unsigned int;
using ScalingVariableId = unsigned int;
using ScalingVariablesMap = std::unordered_map<ScalingVariableId, float>;

struct PlayerAttributeData
{
	AttributeId SelfAttributeId = 0;

	float BaseValue = 0.0f;
	float CurrentValue = 0.0f;

	std::function<float(int, float, const ScalingVariablesMap&)> AttributeScaleFunction;
	std::function<int(int, int, const ScalingVariablesMap&)> CostScaleFunction;
	ScalingVariablesMap ScalingVariables;
	int BaseUpgradeCost = 10;
	int CurrentCost = 10;
	int CurrentLevel = 0;
	int MaxLevel = 10;

	Bloodforge::Event<AttributeId> OnUpgradeApplied;

	bool TryBuyAndApplyUpgrade(int& bloodDroplets)
	{
		if (bloodDroplets < CurrentCost || CurrentLevel >= MaxLevel) return false;
		bloodDroplets -= CurrentCost;
		++CurrentLevel;
		CurrentCost = CostScaleFunction(CurrentLevel, BaseUpgradeCost, ScalingVariables);
		CurrentValue = AttributeScaleFunction(CurrentLevel, BaseValue, ScalingVariables);
		OnUpgradeApplied.Invoke(SelfAttributeId);
		return true;
	}

	// Used primarily when loading from a save file, so the upgrades can be reapplied without a cost
	void ForceApplyUpgrade()
	{
		if (CurrentLevel >= MaxLevel) return;
		++CurrentLevel;
		CurrentCost = CostScaleFunction(CurrentLevel, BaseUpgradeCost, ScalingVariables);
		CurrentValue = AttributeScaleFunction(CurrentLevel, BaseValue, ScalingVariables);
	}
};


namespace ScalingFunctions
{
	inline float AttributeLinearScaleFunction(int level, float baseValue, const ScalingVariablesMap& scalingVariables)
	{
		return baseValue + (scalingVariables.at(CreateId("ScalingFactor")) * level);
	}

	inline float AttributeExponentialScaleFunction(int level, float baseValue, const ScalingVariablesMap& scalingVariables)
	{
		return baseValue * static_cast<float>(std::pow(scalingVariables.at(CreateId("ScalingFactor")), level));
	}

	inline int CostLinearScaleFunction(int level, int baseCost, const ScalingVariablesMap& scalingVariables)
	{
		return static_cast<int>(baseCost + (scalingVariables.at(CreateId("ScalingFactor")) * level));
	}

	inline int CostExponentialScaleFunction(int level, int baseCost, const ScalingVariablesMap& scalingVariables)
	{
		return static_cast<int>(baseCost * std::pow(scalingVariables.at(CreateId("ScalingFactor")), level));
	}
}

struct PlayerAttributes final : Bloodforge::Component<PlayerAttributes>
{
	std::unordered_map<AttributeId, PlayerAttributeData> Attributes
	{
		{
			CreateId("BloodSplatCooldown"), 
			{
				.SelfAttributeId = CreateId("BloodSplatCooldown"),
				.BaseValue = 0.5f,
				.CurrentValue = 0.5f,
				.AttributeScaleFunction = ScalingFunctions::AttributeLinearScaleFunction,
				.CostScaleFunction = ScalingFunctions::CostExponentialScaleFunction,
				.ScalingVariables = { { CreateId("ScalingFactor"), 0.5f } },
				.BaseUpgradeCost = 10,
				.CurrentCost = 10,
				.CurrentLevel = 0,
				.MaxLevel = 10
			},
		},
		{	
			CreateId("BloodSplatDamage"),
			{
				.SelfAttributeId = CreateId("BloodSplatDamage"),
				.BaseValue = 5.0f,
				.CurrentValue = 5.0f,
				.AttributeScaleFunction = ScalingFunctions::AttributeLinearScaleFunction,
				.CostScaleFunction = ScalingFunctions::CostExponentialScaleFunction,
				.ScalingVariables = { { CreateId("ScalingFactor"), 0.5f } },
				.BaseUpgradeCost = 10,
				.CurrentCost = 10,
				.CurrentLevel = 0,
				.MaxLevel = 10
			}
		},
		{
			CreateId("BloodSplatSpeed"),
			{
				.SelfAttributeId = CreateId("BloodSplatSpeed"),
				.BaseValue = 75.0f,
				.CurrentValue = 75.0f,
				.AttributeScaleFunction = ScalingFunctions::AttributeLinearScaleFunction,
				.CostScaleFunction = ScalingFunctions::CostExponentialScaleFunction,
				.ScalingVariables = { { CreateId("ScalingFactor"), 0.5f } },
				.BaseUpgradeCost = 10,
				.CurrentCost = 10,
				.CurrentLevel = 0,
				.MaxLevel = 10
			}
		}
	};

	// BLOOD STONES FOR HEART
	// Different blood stones that gives different stat boosts, dropped by enemies, can be fused together to upgrade quality
};