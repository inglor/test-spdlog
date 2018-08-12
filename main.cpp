#include <iostream>

void stdout_example();

#include "spdlog/spdlog.h"

int main(int, char *[]) {
    try {
        // console logging example
        stdout_example();
        // flush all *registered* loggers using a worker thread every 3 seconds.
        // note: registered loggers *must* be thread safe for this to work correctly!
        spdlog::flush_every(std::chrono::seconds(3));
        // apply some function on all registered loggers
        spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info("End of example."); });
        // release any threads created by spdlog, and drop all loggers in the registry.
        spdlog::shutdown();
    }
        // Exceptions will only be thrown upon failed logger or sink construction (not during logging)
    catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
        return 1;
    }
}

// or #include "spdlog/sinks/stdout_sinks.h" if no colors needed
void stdout_example() {
    // create color multi threaded logger
    auto console = spdlog::stdout_color_mt("console");
    console->info("Welcome to spdlog version {}.{}.{} !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    console->error("Some error message with arg: {}", 1);

    auto err_logger = spdlog::stderr_color_mt("stderr");
    err_logger->error("Some error message");

    // Formatting examples
    console->warn("Easy padding in numbers like {:08d}", 12);
    console->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    console->info("Support for floats {:03.2f}", 1.23456);
    console->info("Positional args are {1} {0}..", "too", "supported");
    console->info("{:<30}", "left aligned");

    spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");

    // Runtime log levels
    spdlog::set_level(spdlog::level::info); // Set global log level to info
    console->debug("This message should not be displayed!");
    console->set_level(spdlog::level::trace); // Set specific logger's log level
    console->debug("This message should be displayed..");

    // Customize msg format for all loggers
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    console->info("This an info message with custom format");

    // Compile time log levels
    // define SPDLOG_DEBUG_ON or SPDLOG_TRACE_ON
    SPDLOG_TRACE(console, "Enabled only #ifdef SPDLOG_TRACE_ON..{} ,{}", 1, 3.23);
    SPDLOG_DEBUG(console, "Enabled only #ifdef SPDLOG_DEBUG_ON.. {} ,{}", 1, 3.23);
}
