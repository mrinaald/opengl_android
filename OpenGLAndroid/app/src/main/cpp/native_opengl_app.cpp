//
// Created by mrinaal on 1/19/20.
//

#include "native_opengl_app.h"

#include <android/log.h>

namespace ndk_opengl_app {

namespace {

#define STR(s) #s
#define STRV(s) STR(s)

#define POS_ATTRIB 0

constexpr const char* VERTEX_SHADER =
        "#version 300 es\n"
        "layout (location = " STRV(POS_ATTRIB) ") in vec3 aPos;\n"
        "void main() {\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n";


constexpr const char* FRAGMENT_SHADER =
        R"glsl(#version 300 es
        precision mediump float;
        out vec4 FragColor;

        void main() {
          FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        })glsl";


float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
};
unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
};

}   // anonymous namespace


NativeOpenGLApp::NativeOpenGLApp(JavaVM *vm)
        : screen_width(0),
          screen_height(0),
          shader_program(0),
          vertex_array_obj(0),
          vertex_buffer_obj(0),
          element_buffer_obj(0) {
  JNIEnv* env;
  vm->GetEnv((void**)&env, JNI_VERSION_1_6);
}


NativeOpenGLApp::~NativeOpenGLApp() {
  glDeleteVertexArrays(1, &vertex_array_obj);
  glDeleteBuffers(1, &vertex_buffer_obj);
  glDeleteBuffers(1, &element_buffer_obj);
}


void NativeOpenGLApp::OnPause() {}


void NativeOpenGLApp::OnResume() {}


void NativeOpenGLApp::OnSurfaceCreated(JNIEnv *env) {
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
  glGenBuffers(1, &element_buffer_obj);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(vertex_array_obj);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
  glEnableVertexAttribArray(POS_ATTRIB);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
  // attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // REMEMBER: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS
  // stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO,
  // but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways
  // so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}


void NativeOpenGLApp::OnDrawFrame() {
  glUseProgram(shader_program);
  glBindVertexArray(vertex_array_obj);  // seeing as we only have a single VAO there's no need to
                                        // bind it every time, but we'll do so to keep things a
                                        // bit more organized
  //glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // when using EBO
  // glBindVertexArray(0); // no need to unbind it every time
}


void NativeOpenGLApp::OnSurfaceChanged(int width, int height) {
  screen_width = width;
  screen_height = height;
}

}   // namespace ndk_opengl_app
