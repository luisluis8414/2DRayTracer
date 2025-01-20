#pragma once

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

  sf::CircleShape m_circle;

  void processSFMLEvents();

  void updateLogic(float deltaTime);

  void render();
};
