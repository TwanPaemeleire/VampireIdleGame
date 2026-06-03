#pragma once
#include <Component.h>
#include <unordered_map>
#include <list>
#include <unordered_map>

enum class HeartWeaponType
{
	BloodSplat,
	BloodArea
};

struct PlayerHeart final : public Bloodforge::Component<PlayerHeart>
{
	std::unordered_map<HeartWeaponType, float> WeaponsNextAvailableTime
	{
		{
			HeartWeaponType::BloodSplat, 0.0f
		},
	};

	std::list<int> EnemyEntityIdsInRange;
	std::unordered_map<int, std::list<int>::iterator> EnemyEntityIdIteratorsInRange; // Enemy id to iterator, so we don't have to search through entire list
};