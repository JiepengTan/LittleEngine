#include "common/precompiled.h"

#include "type_info.h"

TypeInfo::TypeInfo(const Cursor& cursor, const Namespace& current_namespace) :
    m_meta_data(cursor), m_enabled(m_meta_data.getFlag(NativeProperty::Enable)), m_root_cursor(cursor),
    m_namespace(current_namespace)
{
}

const MetaInfo& TypeInfo::getMetaData(void) const { return m_meta_data; }

const std::string TypeInfo::getMetaDataString() const
{
    std::string retStr= "";
    auto infos = m_meta_data.getAllProperties();
    for (auto element : infos)
    {
        retStr += "{\"" +element.first + "\",\""+element.second + "\"},";
    }
    return retStr;
}


std::string TypeInfo::getSourceFile(void) const { return m_root_cursor.getSourceFile(); }

Namespace TypeInfo::getCurrentNamespace() const { return m_namespace; }

std::string TypeInfo::getCurrentNamespaceStr() const
{
    std::string ret;
    for (size_t i = 0; i < m_namespace.size(); i++)
    {
        ret += m_namespace[i];
        if (i != m_namespace.size() - 1)
            ret += "::";
    }
    return ret;
}

Cursor& TypeInfo::getCurosr() { return m_root_cursor; }
