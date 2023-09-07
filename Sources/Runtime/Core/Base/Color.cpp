/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Base/Color.h"

const LittleEngine::Color LittleEngine::Color::Red		= { 1.f, 0.f, 0.f };
const LittleEngine::Color LittleEngine::Color::Green		= { 0.f, 1.f, 0.f };
const LittleEngine::Color LittleEngine::Color::Blue		= { 0.f, 0.f, 1.f };
const LittleEngine::Color LittleEngine::Color::White		= { 1.f, 1.f, 1.f };
const LittleEngine::Color LittleEngine::Color::Black		= { 0.f, 0.f, 0.f };
const LittleEngine::Color LittleEngine::Color::Grey		= { 0.5f, 0.5f, 0.5f };
const LittleEngine::Color LittleEngine::Color::Yellow		= { 1.f, 1.f, 0.f };
const LittleEngine::Color LittleEngine::Color::Cyan		= { 0.f, 1.f, 1.f };
const LittleEngine::Color LittleEngine::Color::Magenta	= { 1.f, 0.f, 1.f };

LittleEngine::Color::Color(float p_r, float p_g, float p_b, float p_a) : r(p_r), g(p_g), b(p_b), a(p_a)
{
}

bool LittleEngine::Color::operator==(const Color & p_other)
{
	return this->r == p_other.r && this->g == p_other.g && this->b == p_other.b && this->a == p_other.a;
}

bool LittleEngine::Color::operator!=(const Color & p_other)
{
	return !operator==(p_other);
}
