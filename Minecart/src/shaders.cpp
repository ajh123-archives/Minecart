#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "shaders.h"
#include "logging.h"

namespace minecart
{
    namespace engine
    {
        Shader::Shader(const char * vertex_file_path, const char * fragment_file_path) {
            this->m_vertex_file_path = vertex_file_path;
            this->m_fragment_file_path = fragment_file_path;
        }
        Shader::~Shader() {}
        int Shader::getId() {
            return this->m_program_id;
        }
        void Shader::compile() {
            // Create the shaders
            GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

            // Read the Vertex Shader code from the file
            std::string VertexShaderCode;
            std::ifstream VertexShaderStream(this->m_vertex_file_path, std::ios::in);
            if (VertexShaderStream.is_open()) {
                std::stringstream sstr;
                sstr << VertexShaderStream.rdbuf();
                VertexShaderCode = sstr.str();
                VertexShaderStream.close();
            } else {
                minecart::logging::log_fatal << "[Shaders] Unable to open " << this->m_vertex_file_path << std::endl;
                return;
            }

            // Read the Fragment Shader code from the file
            std::string FragmentShaderCode;
            std::ifstream FragmentShaderStream(this->m_fragment_file_path, std::ios::in);
            if (FragmentShaderStream.is_open()) {
                std::stringstream sstr;
                sstr << FragmentShaderStream.rdbuf();
                FragmentShaderCode = sstr.str();
                FragmentShaderStream.close();
            } else {
                minecart::logging::log_fatal << "[Shaders] Unable to open " << this->m_fragment_file_path << std::endl;
                return;
            }

            GLint Result = GL_FALSE;
            int InfoLogLength;

            // Compile Vertex Shader
            minecart::logging::log_debug << "[Shaders] Compiling " << this->m_vertex_file_path << std::endl;
            char const * VertexSourcePointer = VertexShaderCode.c_str();
            glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
            glCompileShader(VertexShaderID);

            // Check Vertex Shader
            glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if (InfoLogLength > 0){
                std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
                minecart::logging::log_error << "[Shaders]\n" << &VertexShaderErrorMessage[0] << std::endl;
            }

            // Compile Fragment Shader
            minecart::logging::log_debug << "[Shaders] Compiling " << this->m_fragment_file_path << std::endl;
            char const * FragmentSourcePointer = FragmentShaderCode.c_str();
            glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
            glCompileShader(FragmentShaderID);

            // Check Fragment Shader
            glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if (InfoLogLength > 0){
                std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
                minecart::logging::log_error << "[Shaders]\n" << &FragmentShaderErrorMessage[0] << std::endl;
            }

            // Link the program
            minecart::logging::log_debug << "[Shaders] Linking program" << std::endl;
            int ProgramID = glCreateProgram();
            glAttachShader(ProgramID, VertexShaderID);
            glAttachShader(ProgramID, FragmentShaderID);
            glLinkProgram(ProgramID);

            // Check the program
            glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
            glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if (InfoLogLength > 0){
                std::vector<char> ProgramErrorMessage(InfoLogLength+1);
                glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
                minecart::logging::log_error << "[Shaders]\n" << &ProgramErrorMessage[0] << std::endl;
            }
            
            glDetachShader(ProgramID, VertexShaderID);
            glDetachShader(ProgramID, FragmentShaderID);
            
            glDeleteShader(VertexShaderID);
            glDeleteShader(FragmentShaderID);
            this->m_program_id = ProgramID;
            minecart::logging::log_debug << "[Shaders] Program " << ProgramID << " linked" <<  std::endl;
        }

        void Shader::findLocation(std::string uniform_name) {
            int id = glGetUniformLocation(this->getId(), uniform_name.c_str());
            minecart::logging::log_debug << "[Shaders] \"" << uniform_name << "\" is located at " << id << " in program " << this->m_program_id << std::endl;
            this->uniforms.emplace(uniform_name, id);
        }
        int Shader::get(std::string uniform_name) {
            return this->uniforms.at(uniform_name.c_str());
        }
    }
}