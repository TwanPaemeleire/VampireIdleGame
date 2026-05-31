#pragma once
#include <Component.h>

struct RegularProjecile final : Bloodforge::Component<RegularProjecile>
{
	float Damage;
	float Speed;
};