#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <glm/gtc/matrix_inverse.hpp>

// Explicit specialization of std::hash for Vertex

namespace std {
template <>

struct hash<Vertex> {
  size_t operator()(Vertex const& vertex) const noexcept {
    const std::size_t h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};
}  // namespace std 

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}

void OpenGLWindow::loadModel(std::string_view path) {
  m_model.terminateGL();

  m_model.loadDiffuseTexture(getAssetsPath() + "maps/pattern.png");
  m_model.loadObj(path, false);
  m_model.setupVAO(m_program);
  m_trianglesToDraw = m_model.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0.06, 0.14, 0.26, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "texture.vert",
                                    getAssetsPath() + "texture.frag");

  // Load model

  loadModel(getAssetsPath() + "pine.obj");
  m_mappingMode = 3;  // "From mesh" option
  
  m_ground.initializeGL(m_program);
}


void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  const GLint mappingModeLoc{abcg::glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);

  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);


  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Draw pines
  glm::mat4 model{1.0f};


  for(float i = -5.0; i<5.0; i++){
    for(float j = -5.0; j<5.0; j++){

      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(i, 0.0f, j));
      model = glm::scale(model, glm::vec3(0.002f));

      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform1f(shininessLoc, m_shininess);
      abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
      abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
      abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
      m_model.render(m_trianglesToDraw);

      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(i+0.5, 0.0f, j+0.5));
      model = glm::scale(model, glm::vec3(0.0018f));

      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform1f(shininessLoc, m_shininess);
      abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
      abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
      abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
      m_model.render(m_trianglesToDraw);

    }
    
  }

  abcg::glBindVertexArray(0);

  // Draw ground
  m_ground.paintGL();

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::paintUI();

  ImGui::Begin("Properties");

  // Slider from 0.5f to 1.5f to control  top view
  ImGui::PushItemWidth(220);
  ImGui::SliderFloat("Top View", &topView, 0.5f, 1.5f);
  ImGui::PopItemWidth();

  ImGui::Spacing();

  ImGui::Text("Light properties");

  ImGui::PushItemWidth(260);
  ImGui::ColorEdit3("Ia", &m_Ia.x, ImGuiColorEditFlags_Float);
  ImGui::ColorEdit3("Id", &m_Id.x, ImGuiColorEditFlags_Float);
  ImGui::ColorEdit3("Is", &m_Is.x, ImGuiColorEditFlags_Float);
  ImGui::PopItemWidth();

  ImGui::Spacing();

  ImGui::Text("Material properties");

  // Slider to control material properties
  ImGui::PushItemWidth(260);
  ImGui::ColorEdit3("Ka", &m_Ka.x, ImGuiColorEditFlags_Float);
  ImGui::ColorEdit3("Kd", &m_Kd.x, ImGuiColorEditFlags_Float);
  ImGui::ColorEdit3("Ks", &m_Ks.x, ImGuiColorEditFlags_Float);
  ImGui::PopItemWidth();

  // Slider to control the specular shininess
  ImGui::PushItemWidth(260);
  ImGui::SliderFloat("", &m_shininess, 0.0f, 200.0f, "shininess: %.1f");
  ImGui::PopItemWidth();

  ImGui::End();

}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_ground.terminateGL();

  m_model.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera

  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.view(topView);
  
}