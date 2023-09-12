#pragma once
#include "memory"
#include "vector"
#include "map"
#include "list"
#include "array"
#include "unordered_map"
#include "unordered_set"
#include "set"

#include "string"
#define StringName std::string
#define StringText std::string

#define LITTLE_ENGINE_WINDOWS true
#define LITTLE_ENGINE_EDITOR  true


// common containers for easy replacement
#define WeakPtr         std::weak_ptr
#define SharedPtr       std::shared_ptr
#define UniquePtr       std::unique_ptr

#define TVector         std::vector
#define TArray          std::array
#define TMap            std::map
#define TList           std::list
#define TSet            std::set
#define TUnorderedMap   std::unordered_map
#define TUnorderedSet   std::unordered_set
#define TMultiMap		std::multimap
#define TQueue          std::queue

#define MakeSharedPtr     std::make_shared
#define MakeUniquePtr     std::make_unique

#define GetGlobalService  LittleEngine::Global::ServiceLocator::Get


// string
#include "Core/Debug/Assertion.h"
#include "Core/Debug/Logger.h"

#define LE_ASSERT(condition, message) OvDebug::Assertion::Assert(condition, message)

#define LOG_INFO(message)		OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_INFO,		OvDebug::ELogMode::CONSOLE)
#define LOG_WARNING(message)	OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_WARNING,	OvDebug::ELogMode::CONSOLE)
#define LOG_ERROR(message)	    OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_ERROR,	OvDebug::ELogMode::CONSOLE)
#define LOG_FATAL(message)	    OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_FATAL,	OvDebug::ELogMode::CONSOLE)


#define FLOG_INFO(message)	    OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_INFO, 	OvDebug::ELogMode::CONSOLE); OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_INFO, 	OvDebug::ELogMode::FILE)
#define FLOG_WARNING(message)	OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_WARNING, 	OvDebug::ELogMode::CONSOLE); OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_WARNING, OvDebug::ELogMode::FILE)
#define FLOG_ERROR(message)	    OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_ERROR, 	OvDebug::ELogMode::CONSOLE); OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_ERROR, 	OvDebug::ELogMode::FILE)
#define FLOG_FATAL(message)	    OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_FATAL, 	OvDebug::ELogMode::CONSOLE); OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_ERROR, 	OvDebug::ELogMode::FILE)


namespace LittleEngine
{
    using TypeID   = uint32_t;
    using ObjectID = uint64_t;
    using ActorID = uint64_t;
	
	class Actor;
	class Component;
	
	using ActorMap = TUnorderedMap<ActorID, SharedPtr<Actor>>;
	using ActorVector = TVector<SharedPtr<Actor>>;
	using CompMap = TUnorderedMap<TypeID, SharedPtr<Component>>;
	using CompVector = TVector<SharedPtr<Component>>;
	using ActorPtr = SharedPtr<Actor>;
	using CompPtr = SharedPtr<Component>;

	
	constexpr ObjectID k_maxID = std::numeric_limits<std::size_t>::max();
	constexpr ObjectID k_invalidID = std::numeric_limits<std::size_t>::max();
}
