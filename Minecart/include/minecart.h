#ifndef MINECART_H
#define MINECART_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "logging.h"
#include "shaders.h"
#include "framebuffer.h"

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
            FrameBuffer* buffer;
            EngineProperties(GLFWwindow* window, Shader* defaultShader, FrameBuffer* buffer);
        };

        EngineProperties* init(std::string title, int frame_width, int frame_height);
        void render(EngineProperties* properties);
        void end(EngineProperties* properties);

        extern const char* GLSL_VERSION;
    }
}

#endif // MINECART_H