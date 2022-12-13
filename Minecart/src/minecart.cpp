#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "things.h"
#include "minecart.h"

const char* minecart::engine::GLSL_VERSION = "#version 150";

static void glfw_error_callback(int error, const char* description)
{
    minecart::logging::log_error << "[GLFW ERROR] " << error << description << std::endl;
}

GLuint VertexArrayID;
// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

// This will identify our vertex buffer
GLuint vertexbuffer;

// Camera matrix
glm::mat4 View = glm::lookAt(
    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
);

// Model matrix : an identity matrix (model will be at the origin)
glm::mat4 Model = glm::mat4(1.0f);
// Our ModelViewProjection : multiplication of our 3 matrices
glm::mat4 mvp;

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

            glGenVertexArrays(1, &VertexArrayID);
            glBindVertexArray(VertexArrayID);
            // Generate 1 buffer, put the resulting identifier in vertexbuffer
            glGenBuffers(1, &vertexbuffer);
            // The following commands will talk about our 'vertexbuffer' buffer
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            // Give our vertices to OpenGL.
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

            minecart::engine::Shader* shader = new Shader(
                "./resources/minecart/shaders/default_vertex.glsl", 
                "./resources/minecart/shaders/default_fragment.glsl"
            );
            shader->compile();
            FrameBuffer* buffer = new FrameBuffer(frame_width, frame_height);

            // Our ModelViewProjection : multiplication of our 3 matrices
            mvp = buffer->Projection * View * Model; // Remember, matrix multiplication is the other way around
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
            glUniformMatrix4fv(properties->defaultShader->get("MVP"), 1, GL_FALSE, &mvp[0][0]);

            // Bind buffer
            glBindFramebuffer(GL_FRAMEBUFFER, properties->buffer->FramebufferName);
            glDrawBuffers(1, properties->buffer->DrawBuffers);
            glViewport(0,0,properties->buffer->width,properties->buffer->height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

            glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );
            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
            glDisableVertexAttribArray(0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glUseProgram(0);
        }

        void end(EngineProperties* properties) {
            minecart::logging::log_debug << "[Engine] End"  << std::endl;
            glfwDestroyWindow(properties->window);
            glfwTerminate();
        }
    }
}