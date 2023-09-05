/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Tools/Utils/Random.h"

std::default_random_engine LittleEngine::Utils::Random::__GENERATOR;

int LittleEngine::Utils::Random::Generate(int p_min, int p_max)
{
	std::uniform_int_distribution<int> distribution(p_min, p_max);
	return distribution(__GENERATOR);
}

float LittleEngine::Utils::Random::Generate(float p_min, float p_max)
{
	std::uniform_real_distribution<float> distribution(p_min, p_max);
	return distribution(__GENERATOR);
}

bool LittleEngine::Utils::Random::CheckPercentage(float p_percentage)
{
	std::uniform_real_distribution<float> distribution(0.0f, 100.0f);
	return distribution(__GENERATOR) <= p_percentage;
}
