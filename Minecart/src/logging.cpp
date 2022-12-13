#include <iostream>
#include <sstream>
#include "logging.h"
#include "logging_types.h"

namespace minecart
{
    namespace logging
    {
        Logger::Logger(const std::string& level, const std::string& color) : std::ostream(new LogBuf(level, color)) {}
        Logger::~Logger() { delete rdbuf(); }

        minecart::logging::Logger log_debug("Debug", "\033[1;37m");
        minecart::logging::Logger log_info("Info", "\033[1;34m");
        minecart::logging::Logger log_error("Error", "\033[1;33m");
        minecart::logging::Logger log_fatal("Fatal", "\033[1;31m");
    }
}