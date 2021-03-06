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

// constexpr const char* VERTEX_SHADER =
//         "#version 300 es\n"
//         "layout (location = " STRV(POS_ATTRIB) ") in vec3 aPos;\n"
//         "layout (location = " STRV(TEXTURE_ATTRIB) ") in vec2 aTexCoord;\n"
//         "// layout (location = " STRV(COLOR_ATTRIB) ") in vec3 aColor;\n"
//         "\n"
//         "// out vec3 ourColor;\n"
//         "out vec2 TexCoord;\n"
//         "uniform mat4 model;\n"
//         "uniform mat4 view;\n"
//         "uniform mat4 projection;\n"
//         "\n"
//         "void main() {\n"
//         "  gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
//         "  TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
//         "}\n";


// constexpr const char* FRAGMENT_SHADER =
//         R"glsl(#version 300 es
//         precision mediump float;
//         out vec4 FragColor;
//
//         // in vec3 ourColor;
//         in vec2 TexCoord;
//
//         // texture sampler
//         uniform sampler2D texture0;
//         uniform sampler2D texture1;
//
//         void main() {
//           // FragColor = vec4(ourColor, 1.0f);
//           // FragColor = texture(texture1, TexCoord);
//           // linearly interpolate between both textures (80% container, 20% awesomeface)
//           FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);
//         })glsl";

constexpr const char* VERTEX_SHADER =
        "#version 300 es\n"
        "layout (location = " STRV(POS_ATTRIB) ") in vec3 aPos;\n"
        "\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main() {\n"
        "  gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\n";

constexpr const char* OBJ_FRAGMENT_SHADER =
        R"glsl(#version 300 es
        precision mediump float;
        out vec4 FragColor;

        uniform vec3 objectColor;
        uniform vec3 lightColor;

        void main() {
          FragColor = vec4(lightColor * objectColor, 1.0);
        })glsl";

constexpr const char* LIGHT_FRAGMENT_SHADER =
        R"glsl(#version 300 es
        precision mediump float;
        out vec4 FragColor;

        void main() {
          FragColor = vec4(1.0); // set all 4 vector values to 1.0
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
        : cubeVAO(0),
          lightVAO(0),
          vertex_buffer_obj(0),
          element_buffer_obj(0),
          screen_width(0),
          screen_height(0) {
  camera = new Camera();
}


Renderer::~Renderer() {
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &vertex_buffer_obj);
  // glDeleteBuffers(1, &element_buffer_obj);

  delete camera;
  delete object_shader;
  delete light_shader;
}


void Renderer::Initialize() {
  object_shader = new Shader(VERTEX_SHADER, OBJ_FRAGMENT_SHADER);
  light_shader = new Shader(VERTEX_SHADER, LIGHT_FRAGMENT_SHADER);

  // Creating Vertex Buffer Objects and Vertex Buffer Arrays
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &vertex_buffer_obj);
  // glGenBuffers(1, &element_buffer_obj);

  // Allocate and store Vertex Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // // Allocate and store Element Array Buffer
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // bind the Vertex Array Object first, and configure vertex attributes(s).
  glBindVertexArray(cubeVAO);

  // position attribute
  glVertexAttribPointer(POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(POS_ATTRIB);

  // // color attribute
  // glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(COLOR_ATTRIB);

  // // texture coord attribute
  // glVertexAttribPointer(TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(TEXTURE_ATTRIB);

  // // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
  // // attribute's bound vertex buffer object so afterwards we can safely unbind
  // glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Configure the light's VAO (VBO stays the same; the vertices are the same for the light object
  // which is also a 3D cube)
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it;
  // the VBO's data already contains all we need (it's already bound, but we do it again for
  // educational purposes)
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(POS_ATTRIB);
}


void Renderer::RenderFrame() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // // Bind Textures
  // for (auto & texture_obj : texture_objs) {
  //   texture_obj.Bind();
  // }

  object_shader->UseProgram();
  object_shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  object_shader->setVec3("lightColor",  1.0f, 1.0f, 1.0f);

  glmath::Matrix4x4 view = camera->GetViewMatrix();
  object_shader->setMat4("view", view);

  glmath::Matrix4x4 model;
  object_shader->setMat4("model", model);

  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);


  // Rendering Light source object
  light_shader->UseProgram();
  light_shader->setMat4("view", view);

  glmath::Vec3 lightPos(1.2f, 0.5f, 2.0f);
  model.Translate(lightPos);
  model.Scale(glmath::Vec3(0.2f, 0.2f, 0.2f));
  light_shader->setMat4("model", model);

  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // when using EBO
  // glBindVertexArray(0); // no need to unbind it every time
}


void Renderer::SetScreenParams(const int width, const int height) {
  screen_width = width;
  screen_height = height;

  LOGD("Screen Params (WxH): %d x %d", width, height);

  // pass projection matrix to shader (as projection matrix rarely changes there's no
  // need to do this per frame)
  object_shader->UseProgram();
  glmath::Matrix4x4 projection;
  projection.SetToPerspective(glmath::DegToRad(45.0f),
                              ((float)screen_width) / screen_height, 0.1f, 100.0f);

  object_shader->setMat4("projection", projection);

  light_shader->UseProgram();
  light_shader->setMat4("projection", projection);
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
  // object_shader->UseProgram();   // don't forget to activate/use the shader before setting uniforms!
  // std::string sampler_name = "texture" + std::to_string(texture_objs.size());
  // object_shader->setInt(sampler_name, texture_objs.size());

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
