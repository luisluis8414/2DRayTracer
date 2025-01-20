#include "RayTracer.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <cmath>
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
  const float fpsTarget = 120.0f;
  const float targetFrameTime = 1.0f / fpsTarget;  // 120 FPS
  const float ticksPerSecond = 500.0f;
  const float targetTickTime = 1.0f / ticksPerSecond;  // 500 ticks per second

  {
    m_circle.setRadius(40.f);
    m_circle.setOrigin({m_circle.getRadius(), m_circle.getRadius()});
    m_circle.setPointCount(200);
    m_circle.setFillColor(sf::Color(245, 176, 66));
  }

  auto frameStart = std::chrono::high_resolution_clock::now();
  auto tickStart = std::chrono::high_resolution_clock::now();
  auto secondStart = std::chrono::high_resolution_clock::now();

  int fps = 0;
  int ticks = 0;

  while (m_window.isOpen()) {
    processSFMLEvents();

    auto tickEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> tickDuration = tickEnd - tickStart;

    if (tickDuration.count() >= targetTickTime) {
      tickStart = tickEnd;
      ticks++;

      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2i pixelPosition = sf::Mouse::getPosition(m_window);
        sf::Vector2f worldPosition = m_window.mapPixelToCoords(pixelPosition);

        float dx = worldPosition.x - m_circle.getPosition().x;
        float dy = worldPosition.y - m_circle.getPosition().y;

        if (pow((worldPosition.x - m_circle.getPosition().x), 2) +
                pow((worldPosition.y - m_circle.getPosition().y), 2) <
            pow(m_circle.getRadius(), 2)) {
          m_circle.setPosition(worldPosition);
        }
      }
    }

    // Get current time and calculate elapsed time for frames
    auto frameEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> frameDuration = frameEnd - frameStart;

    if (frameDuration.count() >= targetFrameTime) {
      // Render frame
      m_window.clear();
      m_window.draw(m_circle);
      m_window.display();
      fps++;
      frameStart = frameEnd;
    }

    auto secondEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> secondDuration = secondEnd - secondStart;

    if (secondDuration.count() >= 1.0f) {
      std::cout << "Fps: " << fps << std::endl;
      std::cout << "Ticks: " << ticks << std::endl;
      fps = 0;
      ticks = 0;
      secondStart = secondEnd;
    }

    while (std::chrono::high_resolution_clock::now() - tickStart < std::chrono::duration<float>(targetTickTime)) {
      // No-op to busy-wait
    }
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
