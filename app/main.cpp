#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  auto window = sf::RenderWindow{ {1000u, 640u}, "Think Chess", sf::Style::Default, settings };
  window.setFramerateLimit(1);

  sf::Texture board_img;
  if (!board_img.loadFromFile("../img/chessboard.jpg")) std::cout << "cannot load board\n";
  sf::Sprite board;
  board.setTexture(board_img);

  sf::Texture queen_img;
  if (!queen_img.loadFromFile("../img/queen.png")) std::cout << "cannot load queen\n";
  sf::Sprite queen;
  queen.setTexture(queen_img);

  // begin animation
  while (window.isOpen()) {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // start of new frame
    window.draw(board);

    // set back queens positions
    // for (int q = 0; q < n; q++) queens[q].setPosition(10.f, 10.f + q*80.f);

    window.display();
  } // end of animation
}
