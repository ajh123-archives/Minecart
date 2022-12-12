#ifndef MINECART_LOG_TYPES_H
#define MINECART_LOG_TYPES_H
#include <iostream>
#include <sstream>

namespace minecart
{
    namespace logging
    {
        class LogBuf : public std::stringbuf {
        private:
            std::string m_level;
        public:
            LogBuf(const std::string& level) : m_level(level) { }
            ~LogBuf() {  pubsync(); }
            int sync() {
                std::cout << "[" << m_level << "] " << str();
                str("");
                return std::cout?0:-1;
            }
        };
    }
}

#endif // MINECART_LOG_TYPES_H