#pragma once

enum class StatusEffectTypes 
{
	Stun,
	Fire,
	Poison,
	Slow
};

struct StatusEffectData
{
	StatusEffectTypes Type;
	float TimeTracker;
	float TimeBeforeRemoval;
};