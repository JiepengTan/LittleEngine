#include "common/precompiled.h"

#include "class.h"
#include "field.h"

Field::Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent,bool isEnum) :
    TypeInfo(cursor, current_namespace), m_is_const(cursor.getType().IsConst()), m_parent(parent),
    m_name(cursor.getSpelling()), m_display_name(Utils::getNameWithoutFirstM(m_name))
   
{
    m_type= Utils::getTypeNameWithNamespace(cursor.getType());
    Utils::replaceAll(m_type, " ", "");
    //Utils::replaceAll(m_type, "LittleEngine::", "");
    //std::cout<<m_name << " cursor.getType().GetKind() " << cursor.getType().GetKind() <<std::endl;
    m_is_enum = cursor.getType().GetKind() == CXType_Enum;
    m_is_pointer = cursor.getType().GetKind() == CXType_Pointer;
    if(isEnum)
    {
        m_enum_value = cursor.getEnumVal();
    }
    auto ret_string = Utils::getStringWithoutQuot(m_meta_data.getProperty("default"));
    m_default       = ret_string;
}

bool Field::shouldCompile(void) const { return isAccessible(); }

bool Field::isAccessible(void) const
{
    return ((m_parent->m_meta_data.getFlag(NativeProperty::Fields) ||
             m_parent->m_meta_data.getFlag(NativeProperty::All)) &&
            !m_meta_data.getFlag(NativeProperty::Disable)) ||
           (m_parent->m_meta_data.getFlag(NativeProperty::WhiteListFields) &&
            m_meta_data.getFlag(NativeProperty::Enable));
}
