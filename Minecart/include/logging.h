#ifndef MINECART_LOG_H
#define MINECART_LOG_H
#include <iostream>
#include <sstream>

namespace minecart
{
    namespace logging
    {
        class Logger : public std::ostream {
        public:
            // level is a string which represents the log level e.g. "error"
            Logger(const std::string& level, const std::string& color);
            ~Logger();
        };

        extern minecart::logging::Logger log_debug; //Default Debug logger
        extern minecart::logging::Logger log_info;  //Default Info logger
        extern minecart::logging::Logger log_error; //Default Error logger
        extern minecart::logging::Logger log_fatal; //Default Fatal logger
    }
}

#endif // MINECART_LOG_H