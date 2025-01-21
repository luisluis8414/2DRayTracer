#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "../core/Event.h"

class TextRenderer {
 public:
  TextRenderer(const std::string& fontPath);

  void draw(sf::RenderWindow& window);

  void setFpsText(int text);

 private:
  sf::Font m_font;

  int m_fps = 0;
  sf::Text m_fpsText;
};
