//
// Created by mrinaal on 1/25/20.
//

#include "RendererGL3.h"

namespace ndk_opengl_app {

namespace {

#define STR(s) #s
#define STRV(s) STR(s)

#define POS_ATTRIB 0
#define COLOR_ATTRIB 1
#define TEXTURE_ATTRIB 2

constexpr const char* VERTEX_SHADER =
        "#version 300 es\n"
        "layout (location = " STRV(POS_ATTRIB) ") in vec3 aPos;\n"
        "layout (location = " STRV(COLOR_ATTRIB) ") in vec3 aColor;\n"
        "layout (location = " STRV(TEXTURE_ATTRIB) ") in vec2 aTexCoord;\n"
        "out vec3 ourColor;\n"
        "out vec2 TexCoord;\n"
        "void main() {\n"
        "  gl_Position = vec4(aPos, 1.0);\n"
        "  ourColor = aColor;\n"
        "  TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
        "}\n";


constexpr const char* FRAGMENT_SHADER =
        R"glsl(#version 300 es
        precision mediump float;
        out vec4 FragColor;

        in vec3 ourColor;
        in vec2 TexCoord;

        // texture sampler
        uniform sampler2D texture0;
        uniform sampler2D texture1;

        void main() {
          // FragColor = vec4(ourColor, 1.0f);
          // FragColor = texture(texture1, TexCoord);
          // linearly interpolate between both textures (80% container, 20% awesomeface)
          FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);
        })glsl";


float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
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
  glDeleteBuffers(1, &element_buffer_obj);
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
  glGenBuffers(1, &element_buffer_obj);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(vertex_array_obj);

  // Allocate and store Vertex Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Allocate and store Element Array Buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(POS_ATTRIB);

  // color attribute
  glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(COLOR_ATTRIB);

  // texture coord attribute
  glVertexAttribPointer(TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(TEXTURE_ATTRIB);

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
  // Bind Textures
  for (auto & texture_obj : texture_objs) {
    texture_obj.Bind();
  }

  UseProgram();
  glBindVertexArray(vertex_array_obj);  // seeing as we only have a single VAO there's no need to
                                        // bind it every time, but we'll do so to keep things a
                                        // bit more organized
  // glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // when using EBO
  // glBindVertexArray(0); // no need to unbind it every time
}


/**
 * Should be called after OnSurfaceCreated()
 * @param env
 * @param bitmap
 */
void Renderer::LoadTextureFromBitmap(JNIEnv* env, jobject bitmap, int new_active_texture_id) {
  Texture texture;

  bool status = texture.LoadTextureFromBitmap(env, bitmap, new_active_texture_id);
  if ( !status ) {
    LOGE("Failed Loading texture");
    abort();
  }

  // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
  // -------------------------------------------------------------------------------------------
  UseProgram();   // don't forget to activate/use the shader before setting uniforms!
  std::string sampler_name = "texture" + std::to_string(texture_objs.size());
  glUniform1i(glGetUniformLocation(shader_program, sampler_name.c_str()), texture_objs.size());

  texture_objs.push_back(texture);
}

}   // namespace ndk_opengl_app
