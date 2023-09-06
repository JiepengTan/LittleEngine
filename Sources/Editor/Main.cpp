/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <filesystem>

#include "Core/Tools/Utils/String.h"

#include "Modules/Rendering/Utils/Defines.h"

#include "../Editor/Core/ProjectHub.h"
#include "../Editor/Core/EditorApp.h"
#include "Resource/ResInclude.h"

#undef APIENTRY
#include <fstream>

#include "Windows.h"
#include "Core/Base/Application.h"
#include "Core/Reflection/ReflectionRegister.h"
#include "_Generated/Serializer/AllSerializer.ipp"
#include "_Generated/Reflection/AllReflection.h"

FORCE_DEDICATED_GPU

/**
* When Overload is launched from a project file, we should consider the executable path as
* the current working directory
* @param p_executablePath
*/
void UpdateWorkingDirectory(const std::string& p_executablePath)
{
	if (!IsDebuggerPresent())
	{
		std::filesystem::current_path(LittleEngine::Utils::PathParser::GetContainingFolder(p_executablePath));
	}
}
using namespace LittleEngine;
void ShowMetaExample()
    {
        Test1 test1_in;
        test1_in.m_int  = 12;
        test1_in.m_char = 'g';
        int i           = 1;
        test1_in.m_int_vector.emplace_back(&i);

        Test1 test1_out;
        // test on array
        Test2 test2_in;
        test2_in.m_test_base_array.emplace_back("Test1", &test1_in);
        Test1 Test2_temp;
        test2_in.m_test_base_array.emplace_back("Test1", &Test2_temp);

        // serializer & deserializer

        // Write Test1_in (object) to Test1_json_in (json)
        auto test1_json_in =LittleEngine:: JsonSerializer::Write(test1_in);

        std::string test1_context = test1_json_in.dump();

        // Read Test1_context (json) to Test1_out (object)
        std::string err;

        auto&& Test1_json = Json::parse(test1_context, err);
        LittleEngine::JsonSerializer::Read(Test1_json, test1_out);
        LOG_INFO(test1_context);

        auto        Test2_json_in = LittleEngine::JsonSerializer::Write(test2_in);
        std::string test2_context = Test2_json_in.dump();

        std::fstream out_put("test_reflection_example.json", std::ios::out);
        out_put << test2_context;
        out_put.flush();
        out_put.close();
        LittleEngine::Test2  test2_out;
        auto&& test2_json = Json::parse(test2_context, err);
        LittleEngine::JsonSerializer::Read(test2_json, test2_out);
        LOG_INFO(test2_context.c_str());

        // reflection
        auto                       Meta = TypeMetaDef(Test2, &test2_out);
        LittleEngine::Reflection::FieldAccessor* fields;
        int                        fields_count = Meta.m_meta.GetFieldsList(fields);
        for (int i = 0; i < fields_count; ++i)
        {
            auto filed_accesser = fields[i];
            std::cout << filed_accesser.GetFieldTypeName() << " " << filed_accesser.GetFieldName() << " "
                      << (char*)filed_accesser.Get(Meta.m_instance) << std::endl;
            if (filed_accesser.IsArrayType())
            {
                LittleEngine::Reflection::ArrayAccessor array_accesser;
                if (LittleEngine::Reflection::TypeMeta::NewArrayAccessorFromName(filed_accesser.GetFieldTypeName(), array_accesser))
                {
                    void* field_instance = filed_accesser.Get(Meta.m_instance);
                    int   count          = array_accesser.GetSize(field_instance);
                    auto  typeMetaItem   = LittleEngine::Reflection::TypeMeta::NewMetaFromName(array_accesser.GetElementTypeName());
                    for (int index = 0; index < count; ++index)
                    {
                        std::cout << ":L:" << index << ":R:" << (int*)array_accesser.Get(index, field_instance)
                                  << std::endl;
                        ;
                    }
                }
            }
        }

	
	/**/
    }

int main(int argc, char** argv);
static void TryRun(const std::string& projectPath, const std::string& projectName);

#ifndef _DEBUG
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	main(__argc, __argv);
}
#endif

int main(int argc, char** argv)
{
	UpdateWorkingDirectory(argv[0]);
	Reflection::TypeMetaRegister::MetaRegister();
	
	ShowMetaExample();
	bool ready = false;
	std::string projectPath;
	std::string projectName;

	{
		LittleEngine::Editor::Core::ProjectHub hub;

		if (argc < 2)
		{
			// No project file given as argument ==> Open the ProjectHub
			std::tie(ready, projectPath, projectName) = hub.Run();
		}
		else
		{
			// Project file given as argument ==> Open the project
			std::string projectFile = argv[1];

			if (LittleEngine::Utils::PathParser::GetExtension(projectFile) == "ovproject")
			{
				ready = true;
				projectPath = LittleEngine::Utils::PathParser::GetContainingFolder(projectFile);
				projectName = LittleEngine::Utils::PathParser::GetElementName(projectFile);
				LittleEngine::Utils::String::Replace(projectName, ".ovproject", "");
			}

			hub.RegisterProject(projectPath);
		}
	}

	LittleEngine::Application::isPlaying = false;
	LittleEngine::Application::isEditorMode = true;
	if (ready)
		TryRun(projectPath, projectName);

	return EXIT_SUCCESS;
}

static void TryRun(const std::string& projectPath, const std::string& projectName)
{
	auto errorEvent =
		[](LittleEngine::Windowing::Context::EDeviceError, std::string errMsg)
		{
			errMsg = "Overload requires OpenGL 4.3 or newer.\r\n" + errMsg;
			MessageBox(0, errMsg.c_str(), "Overload", MB_OK | MB_ICONSTOP);
		};

	std::unique_ptr<LittleEngine::Editor::Core::EditorApp> app;

	try
	{
		auto listenerId = LittleEngine::Windowing::Context::Device::ErrorEvent += errorEvent;
		app = std::make_unique<LittleEngine::Editor::Core::EditorApp>(projectPath, projectName);
		LittleEngine::Windowing::Context::Device::ErrorEvent -= listenerId;
	}
	catch (...) {}
	if (app)
		app->Run();
}
