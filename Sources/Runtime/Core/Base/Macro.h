#pragma once
#include "Core/Debug/Logger.h"

#define OVLOG(message)			OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_DEFAULT,	OvDebug::ELogMode::CONSOLE)
#define OVLOG_INFO(message)		OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_INFO,		OvDebug::ELogMode::CONSOLE)
#define OVLOG_WARNING(message)	OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_WARNING,	OvDebug::ELogMode::CONSOLE)
#define OVLOG_ERROR(message)	OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_ERROR,	OvDebug::ELogMode::CONSOLE)

#define OVFLOG(message)			OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_DEFAULT, 	OvDebug::ELogMode::CONSOLE); OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_DEFAULT, OvDebug::ELogMode::FILE)
#define OVFLOG_INFO(message)	OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_INFO, 	OvDebug::ELogMode::CONSOLE); OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_INFO, 	OvDebug::ELogMode::FILE)
#define OVFLOG_WARNING(message)	OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_WARNING, 	OvDebug::ELogMode::CONSOLE); OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_WARNING, OvDebug::ELogMode::FILE)
#define OVFLOG_ERROR(message)	OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_ERROR, 	OvDebug::ELogMode::CONSOLE); OvDebug::Logger::Log(message, OvDebug::ELogLevel::LOG_ERROR, 	OvDebug::ELogMode::FILE)
