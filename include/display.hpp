#pragma once
#include <SFML/Graphics.hpp>

class Display : public sf::Drawable, public sf::Transformable {
public:
  Display() : player{true} {
    setTimerFrame();
    setIndicator();
  }

  void setTimerFrame() {
    timerFrame = sf::VertexArray(sf::LineStrip, 5);
    timerFrame[0].position = sf::Vector2f(10.f, 10.f);
    timerFrame[1].position = sf::Vector2f(300.f, 10.f);
    timerFrame[2].position = sf::Vector2f(300.f, 50.f);
    timerFrame[3].position = sf::Vector2f(10.f, 50.f);
    timerFrame[4].position = sf::Vector2f(10.f, 10.f);
  }
  
  void setPlayer(bool p) {
    player = p;
    changePlayer();
  }

  void setIndicator() {
    indicator = sf::CircleShape(10.f);
    indicator.setPosition(20.f, 20.f);
    indicator.setFillColor(sf::Color::White);
  }

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(timerFrame, states);
    target.draw(indicator, states);
  }

  void changePlayer() {
    if (player) { // white
      indicator.setFillColor(sf::Color::White);
    } else { // black
      indicator.setFillColor(sf::Color::Black);
    }
  }

  bool player;
  sf::VertexArray timerFrame;
  sf::CircleShape indicator;
};

