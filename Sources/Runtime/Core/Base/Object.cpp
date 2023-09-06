#include "Object.h"
#include <atomic>


std::atomic<LittleEngine::ObjectID> LittleEngine::ObjectIDAllocator::m_next_id = 0;
std::atomic<LittleEngine::ActorID> LittleEngine::ActorIDAllocator::m_next_id = 0;

LittleEngine::ObjectID LittleEngine::ObjectIDAllocator::Alloc()
{    
    ++m_next_id;
    if (m_next_id >= k_maxID)
    {
        LOG_FATAL("object id overflow");
    }
    return m_next_id;
}
void LittleEngine::ObjectIDAllocator::Init(ObjectID p_startId)
{
    m_next_id = p_startId;
}
LittleEngine::ActorID LittleEngine::ActorIDAllocator::Alloc()
{
    ++m_next_id;
    if (m_next_id >= k_maxID)
    {
        LOG_FATAL("actor id overflow");
    }
    return m_next_id;
}
void LittleEngine::ActorIDAllocator::Init(ActorID p_startId)
{
    m_next_id = p_startId;
}


void LittleEngine::Object::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot)
{}

void LittleEngine::Object::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot)
{}
