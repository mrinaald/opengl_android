//
// Created by mrinaal on 1/25/20.
//

#include "RendererGL3.h"

#include <android/log.h>

namespace ndk_opengl_app {

namespace {

#define STR(s) #s
#define STRV(s) STR(s)

#define POS_ATTRIB 0
#define COLOR_ATTRIB 1

constexpr const char* VERTEX_SHADER =
        "#version 300 es\n"
        "layout (location = " STRV(POS_ATTRIB) ") in vec3 aPos;\n"
        "layout (location = " STRV(COLOR_ATTRIB) ") in vec3 aColor;\n"
        "out vec3 ourColor;\n"
        "void main() {\n"
        "  gl_Position = vec4(aPos, 1.0);\n"
        "  ourColor = aColor;\n"
        "}\n";


constexpr const char* FRAGMENT_SHADER =
        R"glsl(#version 300 es
        precision mediump float;
        out vec4 FragColor;
        in vec3 ourColor;

        void main() {
          FragColor = vec4(ourColor, 1.0f);
        })glsl";


float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
};

}   // anonymous namespace


Renderer::Renderer()
        : shader_program(0),
          vertex_array_obj(0),
          vertex_buffer_obj(0),
          element_buffer_obj(0) {}


Renderer::~Renderer() {
  glDeleteVertexArrays(1, &vertex_array_obj);
  glDeleteBuffers(1, &vertex_buffer_obj);
}


void Renderer::Initialize() {
  const GLuint vertex_shader =
          LoadGLShader(GL_VERTEX_SHADER, VERTEX_SHADER);
  const GLuint fragment_shader =
          LoadGLShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  CHECKGLERROR("Compile Shader program");

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // According to "https://learnopengl.com/Getting-started/Hello-Triangle"
  // it is okay to delete shader objects after linking
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);


  // Creating Vertex Buffer Objects and Vertex Buffer Arrays
  glGenVertexArrays(1, &vertex_array_obj);
  glGenBuffers(1, &vertex_buffer_obj);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(vertex_array_obj);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0);
  glEnableVertexAttribArray(POS_ATTRIB);

  // color attribute
  glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(COLOR_ATTRIB);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
  // attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO,
  // but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways
  // so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}


void Renderer::UseProgram() {
  glUseProgram(shader_program);
}


void Renderer::RenderFrame() {
  UseProgram();
  glBindVertexArray(vertex_array_obj);  // seeing as we only have a single VAO there's no need to
                                        // bind it every time, but we'll do so to keep things a
                                        // bit more organized
  glDrawArrays(GL_TRIANGLES, 0, 3);
  // glBindVertexArray(0); // no need to unbind it every time
}

}   // namespace ndk_opengl_app
