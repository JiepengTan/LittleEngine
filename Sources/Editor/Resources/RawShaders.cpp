/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "../Editor/Resources/RawShaders.h"

std::pair<std::string, std::string> LittleEngine::Editor::Resources::RawShaders::GetGrid()
{
	std::pair<std::string, std::string> source;

	source.first = R"(

)";

	source.second = R"(

)";

	return source;
}

std::pair<std::string, std::string> LittleEngine::Editor::Resources::RawShaders::GetGizmo()
{
	std::pair<std::string, std::string> source;

	source.first = R"(

)";

	source.second = R"(
)";

	return source;
}

std::pair<std::string, std::string> LittleEngine::Editor::Resources::RawShaders::GetBillboard()
{
	std::pair<std::string, std::string> source;

	source.first = R"(
)";

	source.second = R"(
)";

	return source;
}
