#pragma once
#include <json.hpp>

struct SaveFile
{
	SaveFileEnemySpawningData EnemySpawningData;
	GameStats Stats;
	std::unordered_map<unsigned int, int> PlayerAttributeLevels; // AttributeId and level
};

struct SaveFileEnemySpawningData
{
    int EnemyPointsPerSecond = 1;
};

struct GameStats
{
    int TotalEnemiesKilled = 0;
    int TotalDamageDealt = 0;
};


inline void to_json(nlohmann::json& j, const SaveFile& p)
{
    j = nlohmann::json{
        {"EnemyPointsPerSecond", p.EnemySpawningData.EnemyPointsPerSecond},
        {"TotalEnemiesKilled", p.Stats.TotalEnemiesKilled},
        {"TotalDamageDealt", p.Stats.TotalDamageDealt},
        {"PlayerAttributeLevels", p.PlayerAttributeLevels }
    };
}

inline void from_json(const nlohmann::json& j, SaveFile& p)
{
    j.at("EnemyPointsPerSecond").get_to(p.EnemySpawningData.EnemyPointsPerSecond);
	j.at("TotalEnemiesKilled").get_to(p.Stats.TotalEnemiesKilled);
	j.at("TotalDamageDealt").get_to(p.Stats.TotalDamageDealt);
	j.at("PlayerAttributeLevels").get_to(p.PlayerAttributeLevels);
}