#include "RayTracer.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

RayTracer::RayTracer()
    : m_window(sf::VideoMode({800, 600}), "2D RayTracer"), m_textRenderer("resources/fonts/arial.ttf") {
}

RayTracer::~RayTracer() {
  if (m_window.isOpen()) {
    m_window.close();
  }
}

void RayTracer::run() {
  const float fpsTarget = 60.0f;
  const float targetFrameTime = 1.0f / fpsTarget;  // 60fps and ticks

  sf::Clock clock;
  sf::Clock secondsClock;

  while (m_window.isOpen()) {
    sf::Time deltaTime = clock.restart();

    // process SFML events
    processSFMLEvents();

    // render frame
    m_window.clear();

    m_window.display();

    // cap frame rate
    sf::Time frameEnd = sf::seconds(targetFrameTime) - clock.getElapsedTime();
    if (frameEnd > sf::Time::Zero) sf::sleep(frameEnd);
  }
}

void RayTracer::processSFMLEvents() {
  while (const std::optional event = m_window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      m_window.close();
    } else if (const sf::Event::MouseButtonPressed* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
      sf::Vector2f worldPosition = m_window.mapPixelToCoords({mouseButton->position.x, mouseButton->position.y});
    } else if (const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      sf::Vector2i mousePosition = sf::Mouse::getPosition();
      sf::Vector2f mouseWorldPosition = m_window.mapPixelToCoords(mousePosition);
    } else if (const sf::Event::Resized* resizedEvent = event->getIf<sf::Event::Resized>()) {
      sf::View newView(sf::FloatRect({0.f, 0.f}, {(float)resizedEvent->size.x, (float)resizedEvent->size.y}));
      m_window.setView(newView);
    }
  }
}
