#pragma once
#include <Component.h>
#include <unordered_map>

struct IndividualEnemyAttributes final
{
	float Health;
	float MovementSpeed;
	float AmountOfXPToDrop;
	int PointCost;
};

enum class EnemyType
{
	TestEnemy,
	ENEMYTYPE_END
};

struct EnemyAttributes final : public Bloodforge::Component<EnemyAttributes>
{
	std::unordered_map<EnemyType, IndividualEnemyAttributes> EnemyAttributes
	{
		{
			EnemyType::TestEnemy, 
			IndividualEnemyAttributes
			{
				20.0f,
				25.0f,
				5.0f,
				1
			}
		}
	};
};
