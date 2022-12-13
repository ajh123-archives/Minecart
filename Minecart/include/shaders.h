#ifndef MINECART_SHADER_H
#define MINECART_SHADER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace minecart
{
    namespace engine
    {
        class Shader {
        private:
            const char * m_vertex_file_path;
            const char * m_fragment_file_path;
            long long m_program_id;
        public:
            Shader(const char * vertex_file_path, const char * fragment_file_path);
            ~Shader();
            void compile();
            long long getId();
        };
    }
}

#endif // MINECART_SHADER_H