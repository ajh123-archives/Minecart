#include <iostream>
#include <string>
#include "things.h"
#include "minecart.h"

const char* minecart::engine::GLSL_VERSION = "#version 150";

static void glfw_error_callback(int error, const char* description)
{
    minecart::logging::log_error << "[GLFW ERROR] " << error << description << std::endl;
}


namespace minecart
{
    namespace testing
    {
        void printSomething() {
            minecart::logging::log_info << "[Testing] "<< someString  << std::endl;
        }
    }

    namespace engine
    {
        EngineProperties init(std::string title) {
            minecart::logging::log_debug << "[Engine] Init" << std::endl;
            glfwSetErrorCallback(glfw_error_callback);
            if (!glfwInit())
                return EngineProperties();

            // Decide GL+GLSL versions
            // GL 3.2 + GLSL 150
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac


            // Create window with graphics context
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            std::string main = "Minecart ";
            std::string glfwTitlte = main+title;
            GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, glfwTitlte.c_str(), NULL, NULL);
            if (window == NULL) {
                minecart::logging::log_fatal << "[Init Error] Window is null" << std::endl;
                return EngineProperties() ;
            }
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1); // Enable vsync

            // Initialize OpenGL loader
            bool err = gladLoadGL() == 0;
            if (err)
            {
                minecart::logging::log_fatal << "[Init Error] Failed to initialize OpenGL loader!" << std::endl;
                return EngineProperties();
            }
            EngineProperties data = {
                window,
            };
            return data;
        }

        void render(EngineProperties properties) {
            minecart::logging::log_debug << "[Engine] Render"  << std::endl;
        }

        void end(EngineProperties properties) {
            minecart::logging::log_debug << "[Engine] End"  << std::endl;
            glfwDestroyWindow(properties.window);
            glfwTerminate();
        }
    }
}