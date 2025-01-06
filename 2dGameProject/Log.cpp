#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> Log::logger;

void Log::Init() {
	auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true);
	logger = std::make_shared<spdlog::logger>(spdlog::logger("Logger", { console, file }));
	INFO("spdlog Initialized");
}