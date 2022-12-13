#ifndef MINECART_MODEL_H
#define MINECART_MODEL_H
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "framebuffer.h"

namespace minecart
{
    namespace engine
    {
        class Model {
            public:
            GLuint VertexArrayID;
            GLuint vertexbuffer;
            int vertexCount;
            GLuint colorbuffer;
            // Model matrix : an identity matrix
            glm::mat4 ModelMatrix;
            // Our ModelViewProjection : multiplication of our 3 matrices
            glm::mat4 mvp;
            std::vector<GLfloat> g_vertex_buffer_data;
            std::vector<GLfloat> g_color_buffer_data;
            Model(std::vector<GLfloat> g_vertex_buffer_data, std::vector<GLfloat> g_color_buffer_data, glm::mat4 ModelMatrix, int vertexCount) {
                this->g_vertex_buffer_data = g_vertex_buffer_data;
                this->g_color_buffer_data = g_color_buffer_data;
                this->ModelMatrix = ModelMatrix;
                this->vertexCount = vertexCount;
            }

            void init(minecart::engine::FrameBuffer* frame, glm::mat4 View) {
                glGenVertexArrays(1, &this->VertexArrayID);
                glBindVertexArray(this->VertexArrayID);
                // Generate 1 buffer, put the resulting identifier in vertexbuffer
                glGenBuffers(1, &this->vertexbuffer);
                // The following commands will talk about our 'vertexbuffer' buffer
                glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
                // Give our vertices to OpenGL.
                GLfloat arr[this->g_vertex_buffer_data.size()];
                std::copy(this->g_vertex_buffer_data.begin(), this->g_vertex_buffer_data.end(), arr);
                glBufferData(
                    GL_ARRAY_BUFFER,
                    sizeof(arr),
                    arr,
                    GL_STATIC_DRAW
                );

                
                GLfloat carr[this->g_color_buffer_data.size()];
                std::copy(this->g_color_buffer_data.begin(), this->g_color_buffer_data.end(), carr);
                glGenBuffers(1, &this->colorbuffer);
                glBindBuffer(GL_ARRAY_BUFFER, this->colorbuffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(carr), carr, GL_STATIC_DRAW);

                // Our ModelViewProjection : multiplication of our 3 matrices
                this->mvp = frame->Projection * View * this->ModelMatrix; // Remember, matrix multiplication is the other way around
            }

            void draw() {
                // 1st attribute buffer : vertices
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
                glVertexAttribPointer(
                    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                );
                // 2nd attribute buffer : colors
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, this->colorbuffer);
                glVertexAttribPointer(
                    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                    3,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    0,                                // stride
                    (void*)0                          // array buffer offset
                );
                // Draw the triangle !
                glDrawArrays(GL_TRIANGLES, 0, this->vertexCount); // Starting from vertex 0; 3 vertices total -> 1 triangle
                glDisableVertexAttribArray(0);
            }
        };
    }
}

#endif // MINECART_MODEL_H