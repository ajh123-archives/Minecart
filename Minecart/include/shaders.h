#ifndef MINECART_SHADER_H
#define MINECART_SHADER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>

namespace minecart
{
    namespace engine
    {
        class Shader {
        private:
            const char * m_vertex_file_path;
            const char * m_fragment_file_path;
            int m_program_id;
            std::map<std::string, int> uniforms;
        public:
            Shader(const char * vertex_file_path, const char * fragment_file_path);
            ~Shader();
            void compile();
            int getId();
            void findLocation(std::string uniform_name);
            int get(std::string uniform_name);
        };
    }
}

#endif // MINECART_SHADER_H