#pragma once
#include <System.h>

struct EnemySpawnEventData
{
	int EnemyEntityId;
};

class EnemySpawnSystem final : public Bloodforge::System
{
public:
	virtual void OnUpdate() override;
private:
	void UpdateAvailablePoints();
};