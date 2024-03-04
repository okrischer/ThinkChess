#include <SFML/Graphics.hpp>
#include "pieces.hpp"
#include "moves.hpp"
#include "list.hpp"
#include "display.hpp"
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  auto window = sf::RenderWindow{ {860, 640u},
                                  "ThinkChess++",
                                  sf::Style::Default,
                                  settings };

  window.setFramerateLimit(10);

  // gamestate, set to play
  short state = 1;

  // start timer
  unsigned wTime = 0;
  unsigned bTime = 0;
  chrono::steady_clock::time_point last;
  last = chrono::steady_clock::now();
  chrono::steady_clock::time_point now;

  // matrix of pieces representing the board
  vector<vector<Piece*>> board(8, vector<Piece*>(8));

  // matrix of valid moves for display
  vector<vector<short>> validMoves(8, vector<short>(8, 0));

  // list of moves, used as a stack
  auto* moves = new list::List<string>;

  // list of captured pieces, used as a stack
  auto* captured = new list::List<Piece*>;

  // player to turn, starting with white
  bool player = true;

  // basic evaluation
  short eval = 0;

  // touched field for making moves
  pair<int, int> touched{-1, -1};

  // checkmate
  pair<int, int> checkmate{-1, -1};

  // already castled, 0 = no, 1 = white, 2 = black, 3 = both
  short castled = 0;

  // initialize board TODO: reset timers and eval when starting a new game
  resetBoard(board, moves, captured);

  // set font for text display
  sf::Font noto;
  if (!noto.loadFromFile("../img/NotoMono-Regular.ttf")) {
    cout << "failed to load the font\n";
    return 1;
  }

  // set timer
  sf::Text wTimer;
  wTimer.setFont(noto);
  wTimer.setCharacterSize(24);
  wTimer.setStyle(sf::Text::Bold);
  wTimer.setFillColor(sf::Color::White);
  wTimer.setPosition(690.f, 10.f);

  sf::Text bTimer = wTimer;
  bTimer.setFillColor(sf::Color::Black);
  bTimer.setPosition(690.f, 40.f);

  // set evaluation
  sf::Text evalText;
  evalText.setFont(noto);
  evalText.setCharacterSize(20);
  evalText.setFillColor(sf::Color::White);
  evalText.setString("+/-");
  evalText.setPosition(740.f, 70.f);

  // board sprite
  sf::Texture bi;
  if (!bi.loadFromFile("../img/board.jpg")) {
    cout << "failed to load the board\n";
    return 1;
  }
  sf::Sprite bs;
  bs.setTexture(bi);

  // sprites for pieces
  sf::Texture figures;
  if (!figures.loadFromFile("../img/figures.png")) {
    cout << "failed to load the figures\n";
    return 1;
  }

  sf::Sprite wk;
  wk.setTexture(figures);
  wk.setTextureRect(sf::IntRect(0,0,60,60));

  sf::Sprite bk;
  bk.setTexture(figures);
  bk.setTextureRect(sf::IntRect(0,60,60,60));

  sf::Sprite wq;
  wq.setTexture(figures);
  wq.setTextureRect(sf::IntRect(60,0,60,60));

  sf::Sprite bq;
  bq.setTexture(figures);
  bq.setTextureRect(sf::IntRect(60,60,60,60));

  sf::Sprite wb;
  wb.setTexture(figures);
  wb.setTextureRect(sf::IntRect(120,0,60,60));

  sf::Sprite bb;
  bb.setTexture(figures);
  bb.setTextureRect(sf::IntRect(120,60,60,60));

  sf::Sprite wn;
  wn.setTexture(figures);
  wn.setTextureRect(sf::IntRect(180,0,60,60));

  sf::Sprite bn;
  bn.setTexture(figures);
  bn.setTextureRect(sf::IntRect(180,60,60,60));

  sf::Sprite wr;
  wr.setTexture(figures);
  wr.setTextureRect(sf::IntRect(240,0,60,60));

  sf::Sprite br;
  br.setTexture(figures);
  br.setTextureRect(sf::IntRect(240,60,60,60));

  sf::Sprite wp;
  wp.setTexture(figures);
  wp.setTextureRect(sf::IntRect(300,0,60,60));

  sf::Sprite bp;
  bp.setTexture(figures);
  bp.setTextureRect(sf::IntRect(300,60,60,60));

  // marker for valid moves
  sf::CircleShape valid(20.f);

  //marker for active piece
  sf::RectangleShape frame(sf::Vector2f(63.f, 60.f));
  frame.setFillColor(sf::Color(200, 200, 200, 50));
  frame.setOutlineThickness(12.f);
  frame.setOutlineColor(sf::Color(100, 100, 0));

  //marker for mate
  sf::RectangleShape cm(sf::Vector2f(63.f, 60.f));
  cm.setFillColor(sf::Color(200, 200, 200, 50));
  cm.setOutlineThickness(12.f);
  cm.setOutlineColor(sf::Color(200, 0, 0));

  // background for evaluation
  sf::RectangleShape eb(sf::Vector2f(115.f, 25.f));
  eb.setFillColor(sf::Color::Black);
  eb.setPosition(695.f, 70.f);

  // marker for aktive player
  sf::CircleShape wActive(10.f);
  wActive.setPosition(650.f, 15.f);
  wActive.setFillColor(sf::Color::White);
  sf::CircleShape bActive(10.f);
  bActive.setPosition(650.f, 45.f);
  bActive.setFillColor(sf::Color::Black);
  

  // game loop
  while (window.isOpen()) {
    // event loop
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      // game is running in play mode
      if (state == 1) {
        // mouse button pressed
        if (event.type == sf::Event::MouseButtonPressed) {
          if (event.mouseButton.button == sf::Mouse::Right) {
            if (event.mouseButton.x < 640) {
              pair<int, int> f = getField(event.mouseButton.x, event.mouseButton.y);
              setValidMoves(board, validMoves, board[f.first][f.second]);
            }
          }
          if (event.mouseButton.button == sf::Mouse::Left) {
            if (event.mouseButton.x < 640) {
              pair<int, int> f = getField(event.mouseButton.x, event.mouseButton.y);
              if (touched.first == -1) touched = f;
              else if (f == touched) touched = {-1, -1};
            }
          }
        }
        // mouse button released
        if (event.type == sf::Event::MouseButtonReleased) {
          if (event.mouseButton.button == sf::Mouse::Right) {
            validMoves = vector<vector<short>>(8, vector<short>(8, 0));
          }
          if (event.mouseButton.button == sf::Mouse::Left) {
            if (event.mouseButton.x < 640) {
              pair<int, int> f = getField(event.mouseButton.x, event.mouseButton.y);
              if (touched.first != -1 && touched != f) {
                makeMove(board, moves, captured, touched,
                         f, player, checkmate, castled);

                // evaluation
                pair<short, short> matEval = evaluate(board);
                eval = matEval.first - matEval.second;
                string evalString = "";
                if (eval > 0) {
                  evalString.append(1, '+');
                  evalString.append(to_string(eval));
                } else if (eval < 0) {
                  evalString.append(1, '-');
                  evalString.append(to_string(eval));
                } else {
                  evalString = "+/-";
                }
                if (checkmate.first != -1) evalString = "#";
                evalText.setString(evalString);
              }
            }
          }
        }
      } // end play mode
    } // end event loop

    // set timer
    if (state == 1) {
      now = chrono::steady_clock::now();
      if (now - last > 1s) {
        last = chrono::steady_clock::now();
        if (player) wTime++;
        else bTime++;
      }
    }

    window.clear(sf::Color(100, 100, 100));

    // draw display
    // timer
    string timer = "";
    if (player) {
      window.draw(wActive);
      timer = getTime(wTime);
      wTimer.setString(timer);
    } else {
      window.draw(bActive);
      timer = getTime(bTime);
      bTimer.setString(timer);
    }
    window.draw(wTimer);
    window.draw(bTimer);
    window.draw(eb);
    window.draw(evalText);

    // draw board
    window.draw(bs);

    // draw pieces
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (board[row][col]) {
          auto piece = board[row][col];
          sf::Sprite pc;
          switch (piece->getType()) {
          case 'K':
            piece->isWhite() ? pc = wk : pc = bk;
            break;
          case 'Q':
            piece->isWhite() ? pc = wq : pc = bq;
            break;
          case 'R':
            piece->isWhite() ? pc = wr : pc = br;
            break;
          case 'B':
            piece->isWhite() ? pc = wb : pc = bb;
            break;
          case 'N':
            piece->isWhite() ? pc = wn : pc = bn;
            break;
          case 'P':
            piece->isWhite() ? pc = wp : pc = bp;
            break;
          }
          if (row == touched.first && col == touched.second) {
            frame.setPosition(col*80.f + 10.f, row*80.f + 10.f);
            window.draw(frame);
          }
          if (row == checkmate.first && col == checkmate.second) {
            cm.setPosition(col*80.f + 10.f, row*80.f + 10.f);
            window.draw(cm);
          }
          pc.setPosition(col*80.f + 10.f, row*80.f + 10.f);
          window.draw(pc);
        }
      }
    }
    // draw valid moves
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (validMoves[row][col] > 0) {
          valid.setPosition(col*80.f + 20.f, row*80.f + 20.f);
          if (validMoves[row][col] > 1) valid.setFillColor(sf::Color(0, 200, 0, 200));
          else valid.setFillColor(sf::Color(100, 200, 0, 100));
          window.draw(valid);
        }
      }
    }
    // display frame
    window.display();

    // stop game when checkmate
    if (checkmate.first != -1) state = 0;

  } // end game loop
} // end main
