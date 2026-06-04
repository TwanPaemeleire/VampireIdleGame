#pragma once
#include <Component.h>
#include <Vector2.h>

struct BloodSplat final : public Bloodforge::Component<BloodSplat>
{
	Bloodforge::Vector2 Direction;
};