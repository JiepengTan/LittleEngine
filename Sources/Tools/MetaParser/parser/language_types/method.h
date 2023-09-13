#pragma once

#include "type_info.h"

class Class;

class Method : public TypeInfo
{

public:
    Method(const Cursor& cursor, const Namespace& current_namespace, Class* parent = nullptr);

    virtual ~Method(void) {}

    bool shouldCompile(void) const;
    void parseFunctionParams(const Cursor& cursor);

    std::string getVoidDeclareStr();
    std::string getVoidCallStr();
    std::string getDeclareStr();
    std::string getCallStr();

    bool hasReturn();
    bool hasParams(){ return m_param_count >0;}
    std::string getReturnStr();
    std::string getParamsType();

    std::string getReturnType();
public:

    Class* m_parent;

    std::string m_name;

    bool isAccessible(void) const;

    std::string m_func_name;
    std::string m_return_type;
    std::vector<std::string> m_param_types;
    std::vector<std::string> m_param_names;
    std::vector<std::string> m_param_split;
    int m_param_count;
};