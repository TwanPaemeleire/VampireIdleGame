#pragma once
#include <System.h>

class EnemyMovementSystem final : public Bloodforge::System
{
public:
	virtual void OnUpdate();
};