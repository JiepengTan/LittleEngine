#pragma once

#include "type_info.h"

#include "field.h"
#include "method.h"

struct BaseClass
{
    BaseClass(const Cursor& cursor);

    std::string name;
};

class Class : public TypeInfo
{
    // to access m_qualifiedName
    friend class Field;
    friend class Method;
    friend class MetaParser;

public:
    Class(const Cursor& cursor, const Namespace& current_namespace);

    virtual bool shouldCompile(void) const;

    bool shouldCompileFields(void) const;
    bool shouldCompileMethods(void) const;

    template<typename T>
    using SharedPtrVector = std::vector<std::shared_ptr<T>>;

    std::string getClassName(void);
    std::string getFullName();
    SharedPtrVector<BaseClass> m_base_classes;

public:
    std::string m_name;

    std::string m_qualified_name;

    SharedPtrVector<Field> m_fields;
    SharedPtrVector<Method> m_methods;

    std::string m_display_name;
    int type_id;
    bool isAccessible(void) const;
public:
    static int s_type_id;
};
