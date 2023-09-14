#include "common/precompiled.h"

#include "generator/generator.h"
#include "language_types/class.h"

namespace Generator
{
    void GeneratorInterface::prepareStatus(std::string path)
    {
        if (!fs::exists(path))
        {
            fs::create_directories(path);
        }
    }
    void GeneratorInterface::genClassRenderData(std::shared_ptr<Class> class_temp, Mustache::data& class_def)
    {
        Class classInfo = *class_temp;
        
        class_def.set("class_name", class_temp->getClassName());
        class_def.set("class_is_enum", class_temp->is_enum);
        class_def.set("class_enum_type_name", class_temp->enum_type_name);
        class_def.set("class_is_abstract", class_temp->isAbstract() ?"true":"false");
        class_def.set("class_meta_datas", class_temp->getMetaDataString() );
        class_def.set("class_namespace", class_temp->getCurrentNamespaceStr());
        class_def.set("class_full_name", class_temp->getFullName());
        class_def.set("class_full_code_name", class_temp->getFullCodeName());
        class_def.set("class_type_id", std::to_string(classInfo.type_id));
        class_def.set("class_base_class_size", std::to_string(class_temp->m_base_classes.size()));
        class_def.set("class_need_register", true);
        class_def.set("class_is_struct", class_temp->is_struct);
        
        //std::cout<<classInfo.type_id<<std::endl;
        if (class_temp->m_base_classes.size() > 0)
        {
            Mustache::data class_base_class_defines(Mustache::data::type::list);
            class_def.set("class_has_base", true);
            for (int index = 0; index < class_temp->m_base_classes.size(); ++index)
            {
                Mustache::data class_base_class_def;
                class_base_class_def.set("class_base_class_name", class_temp->m_base_classes[index]->name);
                class_base_class_def.set("class_base_class_index", std::to_string(index));
                class_base_class_defines.push_back(class_base_class_def);
            }
            class_def.set("class_base_class_defines", class_base_class_defines);
        }

        Mustache::data class_field_defines = Mustache::data::type::list;
        genClassFieldRenderData(class_temp, class_field_defines);
        class_def.set("class_field_defines", class_field_defines);

        
        Mustache::data class_method_defines = Mustache::data::type::list;
        genClassMethodRenderData(class_temp, class_method_defines);
        class_def.set("class_method_defines", class_method_defines);
    }
    void GeneratorInterface::genClassFieldRenderData(std::shared_ptr<Class> class_temp, Mustache::data& feild_defs)
    {
        static const std::string vector_prefix = "std::vector<";
        static const std::string array_tga1 = "[";
        static const std::string array_tga2 = "]";
        static const std::string white_space_string = " \t\r\n";

        for (auto& field : class_temp->m_fields)
        {
            if (!field->shouldCompile())
                continue;
            Mustache::data filed_define;

            filed_define.set("class_field_name", field->m_name);
            filed_define.set("class_field_type", field->m_type);
            filed_define.set("class_field_display_name", field->m_display_name);
            filed_define.set("class_field_is_enum", field->m_is_enum);
            filed_define.set("class_field_is_pointer", field->m_is_pointer);
            filed_define.set("class_field_is_normal", !field->m_is_pointer&&!field->m_is_enum);
            filed_define.set("class_field_meta_datas", field->getMetaDataString());
            filed_define.set("class_field_enum_value", field->m_enum_value);
            
            bool is_vector = field->m_type.find(vector_prefix) == 0;
            filed_define.set("class_field_is_vector", is_vector);
            bool is_array= field->m_type.find(array_tga1) != field->m_type.npos;
            int size = 0;
            if(is_array)
            {
                
                auto startIdx = field->m_type.find(array_tga1);
                auto endIdx = field->m_type.find(array_tga2);
                if(endIdx != field->m_type.npos && endIdx>startIdx)
                {
                    auto sizeStr = field->m_type.substr(startIdx+1,endIdx-startIdx-1);
                    sizeStr = Utils::trim(sizeStr,white_space_string);
                    //std::cout << "arysize field->m_type "+ field->m_type +  " result= " + sizeStr <<std::endl;
                    size = std::stoi(sizeStr);
                }else
                {
                    std::cerr << "parse array size error "+class_temp->getFullName() +"::" + field->m_name + " "+ field->m_type <<std::endl;
                }
            }
            filed_define.set("class_field_is_array", is_array);
            filed_define.set("class_field_array_size", std::to_string(size));
            filed_define.set("class_field_is_not_container", !is_array && !is_vector);
            feild_defs.push_back(filed_define);
        }
    }

    void GeneratorInterface::genClassMethodRenderData(std::shared_ptr<Class> class_temp, Mustache::data& method_defs)
    {
        auto methods = class_temp->m_methods;
       for (auto& method : class_temp->m_methods)
        {
            if (!method->shouldCompile())
                continue;
            Mustache::data method_define;

            method_define.set("class_method_name", method->m_name);  
            method_define.set("class_method_return_type", method->getReturnType());
            method_define.set("class_method_void_declare", method->getVoidDeclareStr());  
            method_define.set("class_method_void_call", method->getVoidCallStr());     
            method_define.set("class_method_normal_declare", method->getDeclareStr());  
            method_define.set("class_method_normal_call", method->getCallStr());
            method_define.set("class_method_param_types", method->getParamsType());
            method_define.set("class_mothod_meta_datas", method->getMetaDataString());
            method_define.set("class_mothod_return_str", method->getReturnStr());
            method_define.set("class_mothod_has_return", method->hasReturn());
            method_define.set("class_mothod_has_params", method->hasParams());
           
           
            method_defs.push_back(method_define);
        }
    }
} // namespace Generator
