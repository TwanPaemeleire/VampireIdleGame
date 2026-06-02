#pragma once
#include <System.h>
class EnemySpawnSystem final : public Bloodforge::System
{
public:
	virtual void OnUpdate() override;
private:
	void UpdateAvailablePoints();
};