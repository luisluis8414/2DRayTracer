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

  void processSFMLEvents();
};
