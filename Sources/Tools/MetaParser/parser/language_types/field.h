#pragma once

#include "type_info.h"

class Class;

class Field : public TypeInfo
{

public:
    Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent = nullptr,bool isEnum = false);

    virtual ~Field(void) {}

    bool shouldCompile(void) const;

public:
    bool m_is_const;

    Class* m_parent;

    std::string m_name;
    std::string m_display_name;
    std::string m_type;

    std::string m_default;

    int64_t  m_enum_value = 0;
    bool m_is_enum;
    bool m_is_pointer;
    bool isAccessible(void) const;
};