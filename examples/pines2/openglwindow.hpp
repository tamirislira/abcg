#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"
#include "ground.hpp"
#include "model.hpp"
#include <random>

/*
struct Vertex {
  glm::vec3 position;

  bool operator==(const Vertex& other) const {
    return position == other.position;
  }
};*/

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:

  Model m_model;
  int m_trianglesToDraw{};
  int m_mappingMode{};


  GLuint m_program{};

  // Light and material properties

  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka;
  glm::vec4 m_Kd;
  glm::vec4 m_Ks;
  float m_shininess{100};
  

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};

  Ground m_ground;
  float topView{0.5f};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  std::default_random_engine m_randomEngine;

  void loadModel(std::string_view path);
  void update();
};

#endif