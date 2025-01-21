#include "RayTracer.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <cmath>
#include <iostream>

RayTracer::RayTracer()
    : m_window(sf::VideoMode({800, 600}), "2D RayTracer"), m_textRenderer("resources/fonts/arial.ttf") {
  initializeRays();

  m_sun.setRadius(40.f);
  m_sun.setOrigin({m_sun.getRadius(), m_sun.getRadius()});
  m_sun.setPointCount(100);
  m_sun.setFillColor(sf::Color(245, 176, 66));

  m_obstacle.setRadius(80.f);
  m_obstacle.setOrigin({m_obstacle.getRadius(), m_obstacle.getRadius()});
  m_obstacle.setPointCount(100);
  m_obstacle.setFillColor(sf::Color::Blue);

  m_obstacle2.setRadius(20.f);
  m_obstacle2.setOrigin({m_obstacle2.getRadius(), m_obstacle2.getRadius()});
  m_obstacle2.setPointCount(100);
  m_obstacle2.setFillColor(sf::Color::Green);
}

RayTracer::~RayTracer() {
  if (m_window.isOpen()) {
    m_window.close();
  }
}

void RayTracer::run() {
  sf::Clock secondsClock;

  int fps = 0;

  while (m_window.isOpen()) {
    processSFMLEvents();

    updateRays();

    // Render frame
    m_window.clear();
    m_window.draw(m_obstacle2);
    m_window.draw(m_obstacle);
    m_window.draw(m_rays);
    m_window.draw(m_sun);
    m_textRenderer.draw(m_window);
    m_window.display();

    fps++;

    if (secondsClock.getElapsedTime().asSeconds() >= 1.0f) {
      m_textRenderer.setFpsText(fps);
      fps = 0;
      secondsClock.restart();
    }
  }
}

void RayTracer::initializeRays() {
  m_rays.setPrimitiveType(sf::PrimitiveType::Lines);
  m_rays.resize(NUM_RAYS * 2);
}

void RayTracer::updateRays() {
  sf::Vector2f circleCenter = m_sun.getPosition();

  for (int i = 0; i < NUM_RAYS; ++i) {
    float angle = i * (360.0f / NUM_RAYS);
    // radian berechnen im Einheitskreis, da wir Richtungsvektor berechnen
    float radian = angle * (M_PI / 180.0f);

    sf::Vector2f direction(std::cos(radian), std::sin(radian));
    sf::Vector2f rayEnd = circleCenter + direction * RAY_LENGTH;

    float xStep = (rayEnd.x - circleCenter.x) / RAY_LENGTH;
    float yStep = (rayEnd.y - circleCenter.y) / RAY_LENGTH;

    for (int j = 0; j < RAY_LENGTH; j++) {
      sf::Vector2f step = {j * xStep, j * yStep};

      sf::Vector2f stepPosition = {circleCenter.x + step.x, circleCenter.y + step.y};

      if (isPointInCircle(stepPosition, m_obstacle.getPosition(), m_obstacle.getRadius()) ||
          isPointInCircle(stepPosition, m_obstacle2.getPosition(), m_obstacle2.getRadius())) {
        rayEnd = stepPosition;
        break;
      }
    }

    m_rays[i * 2].position = circleCenter;
    m_rays[i * 2].color = sf::Color(245, 176, 66);

    m_rays[i * 2 + 1].position = rayEnd;
    m_rays[i * 2 + 1].color = sf::Color::Yellow;
  }
}

void RayTracer::processSFMLEvents() {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    sf::Vector2i pixelPosition = sf::Mouse::getPosition(m_window);
    sf::Vector2f worldPosition = m_window.mapPixelToCoords(pixelPosition);

    if (isPointInCircle(worldPosition, m_sun.getPosition(), m_sun.getRadius())) {
      m_sun.setPosition(worldPosition);
      return;
    }

    if (isPointInCircle(worldPosition, m_obstacle.getPosition(), m_obstacle.getRadius())) {
      m_obstacle.setPosition(worldPosition);
      return;
    }

    if (isPointInCircle(worldPosition, m_obstacle2.getPosition(), m_obstacle2.getRadius())) {
      m_obstacle2.setPosition(worldPosition);
      return;
    }
  }

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

bool RayTracer::isPointInCircle(sf::Vector2f point, sf::Vector2f circlePosition, float radius) {
  return pow((point.x - circlePosition.x), 2) + pow((point.y - circlePosition.y), 2) < pow(radius, 2);
}