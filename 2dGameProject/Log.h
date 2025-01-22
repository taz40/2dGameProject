#pragma once
#include "spdlog/logger.h"
#include <memory>

#define TRACE(...) Log::GetLogger()->trace(__VA_ARGS__)
#define DEBUG(...) Log::GetLogger()->debug(__VA_ARGS__)
#define INFO(...) Log::GetLogger()->info(__VA_ARGS__)
#define WARN(...) Log::GetLogger()->warn(__VA_ARGS__)
#define ERR(...) Log::GetLogger()->error(__VA_ARGS__)
#define CRITICAL(...) Log::GetLogger()->critical(__VA_ARGS__)

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return logger; };
private:
	static std::shared_ptr<spdlog::logger> logger;
};

