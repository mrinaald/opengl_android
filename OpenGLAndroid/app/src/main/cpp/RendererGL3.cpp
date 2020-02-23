//
// Created by mrinaal on 1/25/20.
//

#include "RendererGL3.h"

namespace ndk_opengl_app {

namespace {

#define STR(s) #s
#define STRV(s) STR(s)

#define POS_ATTRIB 0
#define TEXTURE_ATTRIB 1
#define COLOR_ATTRIB 2

constexpr const char* VERTEX_SHADER =
        "#version 300 es\n"
        "layout (location = " STRV(POS_ATTRIB) ") in vec3 aPos;\n"
        "layout (location = " STRV(TEXTURE_ATTRIB) ") in vec2 aTexCoord;\n"
        "// layout (location = " STRV(COLOR_ATTRIB) ") in vec3 aColor;\n"
        "\n"
        "// out vec3 ourColor;\n"
        "out vec2 TexCoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main() {\n"
        "  gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "  TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
        "}\n";


constexpr const char* FRAGMENT_SHADER =
        R"glsl(#version 300 es
        precision mediump float;
        out vec4 FragColor;

        // in vec3 ourColor;
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


// Vertices for a Cube
float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// world space positions of our cubes
glmath::Vec3 cubePositions[] = {
        glmath::Vec3( 0.0f,  0.0f,  0.0f ),
        glmath::Vec3( 2.0f,  5.0f, -15.0f),
        glmath::Vec3(-1.5f, -2.2f, -2.5f ),
        glmath::Vec3(-3.8f, -2.0f, -12.3f),
        glmath::Vec3( 2.4f, -0.4f, -3.5f ),
        glmath::Vec3(-1.7f,  3.0f, -7.5f ),
        glmath::Vec3( 1.3f, -2.0f, -2.5f ),
        glmath::Vec3( 1.5f,  2.0f, -2.5f ),
        glmath::Vec3( 1.5f,  0.2f, -1.5f ),
        glmath::Vec3(-1.3f,  1.0f, -1.5f )
};

}   // anonymous namespace


Renderer::Renderer()
        : shader_program(0),
          vertex_array_obj(0),
          vertex_buffer_obj(0),
          element_buffer_obj(0),
          screen_width(0),
          screen_height(0) {
  camera = new Camera();
}


Renderer::~Renderer() {
  glDeleteVertexArrays(1, &vertex_array_obj);
  glDeleteBuffers(1, &vertex_buffer_obj);
  // glDeleteBuffers(1, &element_buffer_obj);

  delete camera;
}


void Renderer::Initialize() {
  const GLuint vertex_shader =
          LoadGLShader(GL_VERTEX_SHADER, VERTEX_SHADER);
  const GLuint fragment_shader =
          LoadGLShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);

  glEnable(GL_DEPTH_TEST);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  CHECKGLERROR("Compile Shader program");

  // According to "https://learnopengl.com/Getting-started/Hello-Triangle"
  // it is okay to delete shader objects after linking
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);


  // Creating Vertex Buffer Objects and Vertex Buffer Arrays
  glGenVertexArrays(1, &vertex_array_obj);
  glGenBuffers(1, &vertex_buffer_obj);
  // glGenBuffers(1, &element_buffer_obj);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(vertex_array_obj);

  // Allocate and store Vertex Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // // Allocate and store Element Array Buffer
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(POS_ATTRIB);

  // // color attribute
  // glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(COLOR_ATTRIB);

  // texture coord attribute
  glVertexAttribPointer(TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
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
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind Textures
  for (auto & texture_obj : texture_objs) {
    texture_obj.Bind();
  }

  // create transformations
  glmath::Matrix4x4 view = camera->GetViewMatrix();

  UseProgram();

  GLint view_loc = glGetUniformLocation(shader_program, "view");
  glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.ToGlArray().data());

  glBindVertexArray(vertex_array_obj);  // seeing as we only have a single VAO there's no need to
                                        // bind it every time, but we'll do so to keep things a
                                        // bit more organized

  GLint model_loc = glGetUniformLocation(shader_program, "model");
  for (int i=0; i<10; ++i) {
    glmath::Matrix4x4 model;
    model.Translate(cubePositions[i]);
    float angle = 20.0f * i;
    model.Rotate(glmath::DegToRad(angle), glmath::Vec3(1.0f, 0.3f, 0.5f));
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.ToGlArray().data());

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // when using EBO
  // glBindVertexArray(0); // no need to unbind it every time
}


void Renderer::SetScreenParams(const int width, const int height) {
  screen_width = width;
  screen_height = height;

  LOGD("Screen Params (WxH): %d x %d", width, height);
  // pass projection matrix to shader (as projection matrix rarely changes there's no
  // need to do this per frame)
  UseProgram();
  glmath::Matrix4x4 projection;
  projection.SetToPerspective(glmath::DegToRad(45.0f),
                              ((float)screen_width) / screen_height, 0.1f, 100.0f);

  GLint projection_loc = glGetUniformLocation(shader_program, "projection");
  glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection.ToGlArray().data());
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


void Renderer::OnViewReset() {
  camera->ResetCamera();
}


void Renderer::OnRotate(const float diff_x, const float diff_y) {
  camera->Rotate(diff_x, diff_y);
}


void Renderer::OnTranslate(const float diff_x, const float diff_y) {
  camera->Translate(diff_x, diff_y);
}


void Renderer::OnScale(const float scale_x, const float scale_y) {
  camera->Zoom(scale_x, scale_y);
}

}   // namespace ndk_opengl_app
