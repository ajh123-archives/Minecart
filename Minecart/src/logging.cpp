#include <iostream>
#include <sstream>
#include "logging.h"
#include "logging_types.h"

namespace minecart
{
    namespace logging
    {
        Logger::Logger(const std::string& level) : std::ostream(new LogBuf(level)) {}
        Logger::~Logger() { delete rdbuf(); }

        minecart::logging::Logger log_debug("Debug");
        minecart::logging::Logger log_info("Info");
        minecart::logging::Logger log_error("Error");
        minecart::logging::Logger log_fatal("Fatal");
    }
}