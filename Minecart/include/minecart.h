#ifndef MINECART_H
#define MINECART_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "logging.h"
#include "shaders.h"

namespace minecart
{
    namespace testing
    {
        void printSomething();
    }

    namespace engine
    {
        class EngineProperties {
            public:
            GLFWwindow* window;
            Shader* defaultShader;
            EngineProperties(GLFWwindow* window, Shader* defaultShader);
        };

        EngineProperties* init(std::string title);
        void render(EngineProperties* properties);
        void end(EngineProperties* properties);

        extern const char* GLSL_VERSION;
    }
}

#endif // MINECART_H