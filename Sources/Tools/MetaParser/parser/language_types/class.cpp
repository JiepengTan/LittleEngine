#include "common/precompiled.h"

#include "class.h"

BaseClass::BaseClass(const Cursor& cursor) : name(Utils::getTypeNameWithoutNamespace(cursor.getType())) {}
int Class::s_type_id = 1;
Class::Class(const Cursor& cursor, const Namespace& current_namespace) :
    TypeInfo(cursor, current_namespace), m_name(cursor.getDisplayName()),
    m_qualified_name(Utils::getTypeNameWithoutNamespace(cursor.getType())),
    m_display_name(Utils::getNameWithoutFirstM(m_qualified_name))
{
    is_struct = cursor.getKind() == CXCursor_StructDecl;

    auto rawName = cursor.getDisplayName();
    std::string curNamespace = "";
    //for (auto nameItem : current_namespace)  curNamespace += nameItem + "::";
    
    Utils::replaceAll(m_name, " ", "");
    //Utils::replaceAll(m_name, "LittleEngine::", "");
    for (auto& child : cursor.getChildren())
    {
        switch (child.getKind())
        {
            case CXCursor_CXXBaseSpecifier: {
                auto base_class = new BaseClass(child);

                m_base_classes.emplace_back(base_class);
            }
            break;
            // field
            case CXCursor_FieldDecl:
                m_fields.emplace_back(new Field(child, current_namespace, this));
                break;
            // method
            case CXCursor_CXXMethod:
                m_methods.emplace_back(new Method(child, current_namespace, this));
            default:
                break;
        }
    }
    if(shouldCompile()){
        type_id = s_type_id++;
    }
}

bool Class::shouldCompile(void) const { return shouldCompileFields()|| shouldCompileMethods(); }

bool Class::shouldCompileFields(void) const
{
    return m_meta_data.getFlag(NativeProperty::All) || m_meta_data.getFlag(NativeProperty::Fields) ||
           m_meta_data.getFlag(NativeProperty::WhiteListFields);
}

bool Class::shouldCompileMethods(void) const{
    
    return m_meta_data.getFlag(NativeProperty::All) || m_meta_data.getFlag(NativeProperty::Methods) ||
           m_meta_data.getFlag(NativeProperty::WhiteListMethods);
}

std::string Class::getClassName(void) { return m_name; }

std::string Class::getFullName()
{
    return getCurrentNamespaceStr() + "::" + getClassName();
}

bool Class::isAccessible(void) const { return m_enabled; }
