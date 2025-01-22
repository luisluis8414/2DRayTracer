#pragma once

constexpr int NUM_RAYS = 360;
constexpr float RAY_LENGTH = 800.0f;
constexpr float M_PI = 3.14159265358979323846f;

#include "../rendering/TextRenderer.h"
#include "Event.h"

class RayTracer {
 public:
  RayTracer();
  ~RayTracer();

  void run();

 private:
  sf::RenderWindow m_window;
  TextRenderer m_textRenderer;

  sf::Shader m_rayShader;

  sf::CircleShape m_sun;
  sf::CircleShape m_obstacle;
  sf::CircleShape m_obstacle2;

  sf::VertexArray m_rays;

  sf::RenderTexture m_renderTexture;

  void processSFMLEvents();

  void initializeRays();

  void updateRays();

  bool isPointInCircle(sf::Vector2f point, sf::Vector2f circlePosition, float radius);
};
