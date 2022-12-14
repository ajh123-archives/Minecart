#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "things.h"
#include "minecart.h"
#include "model.h"

const char* minecart::engine::GLSL_VERSION = "#version 150";

static void glfw_error_callback(int error, const char* description)
{
    minecart::logging::log_error << "[GLFW ERROR] " << error << description << std::endl;
}

// An array of 3 vectors which represents 3 vertices
// Model matrix : an identity matrix (model will be at the origin)
minecart::engine::Model* model = new minecart::engine::Model({
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
}, {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
}, glm::mat4(1.0f), 12*3);

// Camera matrix
glm::mat4 View = glm::lookAt(
    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
);

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
        EngineProperties::EngineProperties(GLFWwindow* window, Shader* defaultShader, FrameBuffer* buffer) {
            this->window = window;
            this->defaultShader = defaultShader;
            this->buffer = buffer;
        }

        EngineProperties* init(std::string title, int frame_width, int frame_height) {
            minecart::logging::log_debug << "[Engine] Init" << std::endl;
            glfwSetErrorCallback(glfw_error_callback);
            if (!glfwInit())
                return NULL;

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
                return NULL;
            }
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1); // Enable vsync

            // Initialize OpenGL loader
            bool err = gladLoadGL() == 0;
            if (err)
            {
                minecart::logging::log_fatal << "[Init Error] Failed to initialize OpenGL loader!" << std::endl;
                return NULL;
            }

            minecart::engine::Shader* shader = new Shader(
                "./resources/minecart/assets/shaders/default_vertex.glsl", 
                "./resources/minecart/assets/shaders/default_fragment.glsl"
            );
            shader->compile();
            FrameBuffer* buffer = new FrameBuffer(frame_width, frame_height);

            model->init(buffer, View);

            // Get a handle for our "MVP" uniform
            // Only during the initialisation
            shader->findLocation("MVP");
            
            return new EngineProperties(
                window,
                shader,
                buffer
            );
        }

        void render(EngineProperties* properties) {
            // Bind shader
            glUseProgram(properties->defaultShader->getId());
            // Send our transformation to the currently bound shader, in the "MVP" uniform
            // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
            glUniformMatrix4fv(properties->defaultShader->get("MVP"), 1, GL_FALSE, &model->mvp[0][0]);

            // Bind buffer
            glBindFramebuffer(GL_FRAMEBUFFER, properties->buffer->FramebufferName);
            glDrawBuffers(1, properties->buffer->DrawBuffers);
            glViewport(0,0,properties->buffer->width,properties->buffer->height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

            // Enable depth test
            glEnable(GL_DEPTH_TEST);
            // Accept fragment if it closer to the camera than the former one
            glDepthFunc(GL_LESS);

            glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            model->draw();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glUseProgram(0);
            glDisable(GL_DEPTH_TEST);
        }

        void end(EngineProperties* properties) {
            minecart::logging::log_debug << "[Engine] End"  << std::endl;
            glfwDestroyWindow(properties->window);
            glfwTerminate();
        }
    }
}