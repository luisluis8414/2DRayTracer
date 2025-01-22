#include "TextRenderer.h"

TextRenderer::TextRenderer(const std::string& fontPath) : m_fpsText(m_font) {
  if (!m_font.openFromFile(fontPath)) {
    std::cerr << "Couldn't load font from: " << fontPath << std::endl;
    return;
  }
  m_fpsText.setFont(m_font);
  m_fpsText.setFillColor(sf::Color::Green);
}

void TextRenderer::setFpsText(int text) {
  m_fpsText.setString(std::to_string(text));
}

void TextRenderer::draw(sf::RenderWindow& window) {
  sf::View originalView = window.getView();

  sf::View hudView(
      sf::FloatRect({0, 0}, {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)}));
  window.setView(hudView);

  sf::Vector2u windowSize = window.getSize();
  sf::FloatRect textBounds = m_fpsText.getLocalBounds();

  float xPos = static_cast<float>(windowSize.x) - textBounds.size.x - 15.0f;
  float yPos = 10.0f;

  m_fpsText.setPosition({xPos, yPos});

  window.draw(m_fpsText);

  window.setView(originalView);
}
