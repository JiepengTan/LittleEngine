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


int main(int argc, char* args[])
{
    
    argc = 4;
    args[1] =(char*) "I:/Projects/OverloadEngine/Overload/Sources/Runtime/../../Bin/Tools/CodeAnalyzerOutput.txt";
    args[2] =(char*) "I:/Projects/OverloadEngine/Overload/Sources/Runtime/../../Sources/Runtime";
    args[3] =(char*) "I:/Projects/OverloadEngine/Overload/Sources/Runtime/../../Bin/Tools/tempBuild.json";
    std::cout << "Arguments Count"<< argc <<std::endl;
    if(argc <4)
    {
        std::cerr << "Arguments parse error!" << std::endl
        << "Please call the tool like this:" << std::endl
        << "MetaParser filesToGenerate sourceCodeDirectory tempBuildFile "
        << std::endl
        << std::endl;
        for ( int i =0;i< argc;i++  )
            std::cerr << "Param " <<i<<"=" + std::string(args[i]) << std::endl;
        return -1;
    }
    
  
    auto start_time = std::chrono::system_clock::now();
    if (true)
    {
        MetaParser::prepare();
        std::string project_input_file_name = args[1];
        std::string include_path = args[2];
        std::string show_errors = "0";
        std::string source_include_file_name =args[3];
        std::string sys_include = "*";
        std::string module_name = "LittleEngine";
        std::string template_dir = include_path + "/_CodeTemplate/";
        
        std::cout <<
            "MetaParser inputFiles=" +project_input_file_name +
            " sourceDir ="<<include_path +
            " tempBuildFile =" << source_include_file_name +
            " showErrors=" + show_errors
        << std::endl;
        
        auto result = parse(
            project_input_file_name, template_dir, source_include_file_name, include_path,
            sys_include, module_name, show_errors);

        auto duration_time = std::chrono::system_clock::now() - start_time;
        std::cout << "Completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(duration_time).count()
            << "ms" << std::endl;
        return result;
    }
    else
    {

        return -1;
    }

    return 0;
}
