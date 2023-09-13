#include "common/precompiled.h"

#include "class.h"
#include "method.h"

void Method::parseFunctionParams(const Cursor& cursor)
{
    auto return_type = cursor.getType().getReturnType();
    auto return_type_name = return_type.GetDisplayName();

    m_return_type = return_type_name;
    auto func_name = cursor.getSpelling();
    auto full_name = cursor.getDisplayName();
    auto temp_full_name = full_name;

    m_func_name = func_name;
    temp_full_name = Utils::replace(temp_full_name, func_name, "");
    temp_full_name = Utils::replace(temp_full_name, "(", "");
    temp_full_name = Utils::replace(temp_full_name, ")", "");

    auto param_types = Utils::split(temp_full_name, ",");
    int count = param_types.size();
    for (int i = 0; i < count; ++i)
    {
        auto type_name = param_types[i];
        m_param_names.push_back("_" + std::to_string(i+1));
        m_param_types.push_back(type_name);
        m_param_split.push_back(i == count - 1 ? "" : ",");
    }
    m_param_count = m_param_types.size();

    /*
    if(func_name.find("AAA") != -1)
    {
        auto str1= getVoidDeclareStr();
        auto str2= getVoidCallStr();
        auto str3= getDeclareStr();
        auto str4= getCallStr();
        int ss =0;
    }
     */
}

std::string Method::getVoidDeclareStr()
{
    std::string ret_str =  "";
    for (int i = 0; i < m_param_count; ++i)
    {
        ret_str += "void* " + m_param_names[i] + m_param_split[i];
    }
    return ret_str;
}

std::string Method::getVoidCallStr()
{
    std::string ret_str =  "";
    for (int i = 0; i < m_param_count; ++i)
    {
        ret_str += "*(" + m_param_types[i] +"*)" +m_param_names[i] + m_param_split[i];
    }
    return ret_str;
}

std::string Method::getDeclareStr()
{
    std::string ret_str =  "";
    for (int i = 0; i < m_param_count; ++i)
    {
        ret_str += m_param_types[i] + " " + m_param_names[i] + m_param_split[i];
    }
    return ret_str;
}

std::string Method::getCallStr()
{
    std::string ret_str =  "";
    for (int i = 0; i < m_param_count; ++i)
    {
        ret_str += m_param_names[i] + m_param_split[i];
    }
    return ret_str;
}

bool Method::hasReturn()
{
    return getReturnType() != "void";
}

std::string Method::getReturnStr()
{ return getReturnType() != "void" ?"":"return" ;}

std::string Method::getParamsType()
{
    std::string call_str ="";
    for (int i = 0; i < m_param_count; ++i)
    {
        call_str += "\"" + m_param_types[i] + "\"" + m_param_split[i];
    }
    return call_str;
}

std::string Method::getReturnType()
{
    return m_return_type;
}

Method::Method(const Cursor& cursor, const Namespace& current_namespace, Class* parent) :
    TypeInfo(cursor, current_namespace), m_parent(parent), m_name(cursor.getSpelling())
{
    
    if (shouldCompile())
    {
        parseFunctionParams(cursor);
    }
}

bool Method::shouldCompile(void) const { return isAccessible(); }

bool Method::isAccessible(void) const
{
    return ((m_parent->m_meta_data.getFlag(NativeProperty::Methods) ||
                m_parent->m_meta_data.getFlag(NativeProperty::All)) &&
            !m_meta_data.getFlag(NativeProperty::Disable)) ||
        (m_parent->m_meta_data.getFlag(NativeProperty::WhiteListMethods) &&
            !m_meta_data.isEmpty());
}
