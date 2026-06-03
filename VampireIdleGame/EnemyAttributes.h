#pragma once
#include <Component.h>
#include <unordered_map>

struct IndividualEnemyAttributes final
{
	float Health;
	float MovementSpeed;
	float AmountOfXPToDrop;
	float PointCost;
};

enum class EnemyType
{
	TestEnemy,
	ENEMYTYPE_END
};

struct EnemyAttributes final : public Bloodforge::Component<EnemyAttributes>
{
	std::unordered_map<EnemyType, IndividualEnemyAttributes> Attributes
	{
		{
			EnemyType::TestEnemy, 
			IndividualEnemyAttributes
			{
				.Health = 20.0f,
				.MovementSpeed = 25.0f,
				.AmountOfXPToDrop = 5.0f,
				.PointCost = 1,
			}
		}
	};
};
