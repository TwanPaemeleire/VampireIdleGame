#pragma once
#include <System.h>

struct PlayerHeart;

class PlayerHeartSystem final : public Bloodforge::System
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate() override;
private:
	void DoBloodSplat(PlayerHeart& playerHeart);
};