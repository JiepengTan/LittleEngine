#include "common/precompiled.h"
#include "parser/parser.h"

int parse(std::string project_input_file_name,
          std::string template_dir,
          std::string source_include_file_name,
          std::string include_path,
          std::string sys_include,
          std::string module_name,
          std::string show_errors)
{
    std::cout << std::endl;
    std::cout << "Parsing meta data for target \"" << module_name << "\"" << std::endl;
    std::fstream input_file;

    bool is_show_errors = "0" != show_errors;
    MetaParser parser(
        project_input_file_name, template_dir, source_include_file_name,
        include_path, sys_include, module_name, is_show_errors);

    std::cout << "Parsing in " << include_path << std::endl;
    int result = parser.parse();
    if (0 != result)
    {
        return result;
    }

    parser.generateFiles();

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc < 7)
    {
        auto path =  argv[0];
        argv = new char*[7];
        int idx = 0;
        argv[idx++] = path;
        argv[idx++] = (char*)("I:/Projects/OverloadEngine/Overload/Bin/Tools/CodeAnalyzerOutput.txt");
        argv[idx++] = (char*)("I:/Projects/OverloadEngine/Overload/Bin/Tools/CodeParserResult.h");
        argv[idx++] = (char*)("I:/Projects/OverloadEngine/Overload/Sources/Runtime");
        argv[idx++] = (char*)("*");
        argv[idx++] = (char*)("LittleEngine");
        argv[idx++] = (char*)("0");
    }
    auto start_time = std::chrono::system_clock::now();
    int result = 0;
    if (argv[1] != nullptr && argv[2] != nullptr && argv[3] != nullptr && argv[4] != nullptr && argv[5] != nullptr &&
        argv[6] != nullptr)
    {
        std::cout << "!ParseCo11de!" << std::endl;
        for (int i = 0; i < 7; i++)
        {
            auto str = std::string(argv[i]);
            std::cout << str << std::endl;
        }

        MetaParser::prepare();

        std::string project_input_file_name = argv[1];
        std::string source_include_file_name = argv[2];
        std::string include_path =argv[3];
        std::string sys_include = argv[4];
        std::string module_name = argv[5];
        std::string show_errors = argv[6];
        std::string template_dir = include_path + "/_CodeTemplate/";
        result = parse(
            project_input_file_name, template_dir, argv[2], include_path,
            sys_include, module_name, show_errors);

        auto duration_time = std::chrono::system_clock::now() - start_time;
        std::cout << "Completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(duration_time).count()
            << "ms" << std::endl;
        return result;
    }
    else
    {
        std::cerr << "Arguments parse error!" << std::endl
                  << "Please call the tool like this:" << std::endl
                  << "meta_parser  project_file_name  include_file_name_to_generate  project_base_directory "
                     "sys_include_directory module_name showErrors(0 or 1)"
                  << std::endl
                  << std::endl;
        return -1;
    }

    return 0;
}
