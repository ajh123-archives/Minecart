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
            std::string m_color;
        public:
            LogBuf(const std::string& level, const std::string& color) : m_level(level), m_color(color) { }
            ~LogBuf() {  pubsync(); }
            int sync() {
                std::cout << "[" << m_color << m_level << "\033[0m] " << str();
                str("");
                return std::cout?0:-1;
            }
        };
    }
}

#endif // MINECART_LOG_TYPES_H