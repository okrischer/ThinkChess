#include <SFML/Graphics.hpp>
#include "pieces.hpp"
#include "display.hpp"
#include "position.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  auto window = sf::RenderWindow{ {870, 640u},
                                  "ThinkChess++",
                                  sf::Style::Close,
                                  settings };

  window.setFramerateLimit(10);

  // main state object
  Position position(0);

  // last played game
  filesystem::path actGame = "../games/lastGame";
  std::fstream game;

  // games folder
  const filesystem::path games = "../games";

  // board evaluation for evaluation meter
  float eval = 0.f;

  // content of moves history
  string history;

  // indicates whether a move was successful
  bool moved = false;

  // wether a draw was offered
  bool draw = false;

  // wether a move is taken back
  bool takeback = false;

  // wether a player wants to give up
  bool giveUp = false;

  // load game file
  bool load = false;
  bool loaded = false;

  // start timer
  unsigned wTime = 0;
  unsigned bTime = 0;
  steady_clock::time_point last;
  last = steady_clock::now();
  steady_clock::time_point now;

  // matrix of valid moves for display
  vector<vector<short>> validMoves(8, vector<short>(8, 0));

  // touched field for making moves
  pair<int, int> touched{-1, -1};

  // set font for text display
  sf::Font noto;
  if (!noto.loadFromFile("../img/NotoMono-Regular.ttf")) {
    cout << "failed to load the font\n";
    return 1;
  }

  // splash screen
  sf::RectangleShape bsplash(sf::Vector2f(280.f, 90.f));
  bsplash.setFillColor(sf::Color(30, 30, 30, 200));
  bsplash.setPosition(155.f, 235.f);
  sf::Text welcome;
  welcome.setFont(noto);
  welcome.setCharacterSize(16);
  welcome.setFillColor(sf::Color(220, 0, 0));
  welcome.setStyle(sf::Text::Bold);
  welcome.setPosition(170.f, 240.f);
  welcome.setString("Welcome to ThinkChess++");
  sf::Text spt;
  spt.setFont(noto);
  spt.setCharacterSize(16);
  spt.setFillColor(sf::Color(0, 220, 0));
  spt.setPosition(230.f, 270.f);
  spt.setString("Start game <S>\nLoad  game <L>");

  // file loader
  sf::RectangleShape bloader(sf::Vector2f(240.f, 320.f));
  bloader.setFillColor(sf::Color(30, 30, 30, 200));
  bloader.setPosition(160.f, 160.f);
  sf::Text tfiles;
  tfiles.setFont(noto);
  tfiles.setCharacterSize(14);
  tfiles.setFillColor(sf::Color(0, 220, 0));
  tfiles.setPosition(170.f, 165.f);

  // set timer
  sf::Text wTimer;
  wTimer.setFont(noto);
  wTimer.setCharacterSize(24);
  wTimer.setStyle(sf::Text::Bold);
  wTimer.setFillColor(sf::Color::White);
  wTimer.setString("00:00:00");
  wTimer.setPosition(690.f, 10.f);

  sf::Text bTimer = wTimer;
  bTimer.setFillColor(sf::Color::Black);
  bTimer.setPosition(690.f, 40.f);

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

  // evaluation meter
  sf::VertexArray mb(sf::Triangles, 6);
  mb[0].position = sf::Vector2f(690.f, 80.f);
  mb[1].position = sf::Vector2f(690.f, 100.f);
  mb[2].position = sf::Vector2f(810.f, 100.f);
  mb[3].position = sf::Vector2f(690.f, 80.f);
  mb[4].position = sf::Vector2f(810.f, 80.f);
  mb[5].position = sf::Vector2f(810.f, 100.f);
  mb[0].color = sf::Color::White;
  mb[1].color = sf::Color::White;
  mb[2].color = sf::Color::Black;
  mb[3].color = sf::Color::White;
  mb[4].color = sf::Color::Black;
  mb[5].color = sf::Color::Black;
  sf::VertexArray mi(sf::Lines, 4);
  mi[0].position = sf::Vector2f(750.f, 80.f);
  mi[1].position = sf::Vector2f(750.f, 100.f);
  mi[2].position = sf::Vector2f(750.f, 80.f);
  mi[3].position = sf::Vector2f(750.f, 100.f);
  mi[0].color = sf::Color::Green;
  mi[1].color = sf::Color::Green;
  mi[2].color = sf::Color::Black;
  mi[3].color = sf::Color::Black;

  // background for captured pieces
  sf::RectangleShape bcw(sf::Vector2f(210.f, 20.f));
  bcw.setFillColor(sf::Color(30, 30, 30, 200));
  bcw.setPosition(650.f, 115.f);
  sf::RectangleShape bcb = bcw;
  bcb.setFillColor(sf::Color(220, 220, 220));
  bcb.move(0.f, 30.f);

  // infotext
  sf::RectangleShape bit(sf::Vector2f(210.f, 25.f));
  bit.setFillColor(sf::Color(30, 30, 30, 200));
  bit.setPosition(650.f, 610.f);
  sf::Text itt;
  itt.setFont(noto);
  itt.setCharacterSize(14);
  itt.setFillColor(sf::Color(220, 0, 0));
  itt.setPosition(660.f, 613.f);

  // marker for aktive player
  sf::CircleShape wActive(10.f);
  wActive.setPosition(650.f, 15.f);
  wActive.setFillColor(sf::Color::White);
  sf::CircleShape bActive(10.f);
  bActive.setPosition(650.f, 45.f);
  bActive.setFillColor(sf::Color::Black);
  
  // current move background
  sf::RectangleShape mvb(sf::Vector2f(120.f, 20.f));
  mvb.setFillColor(sf::Color(200, 200, 0, 200));
  mvb.setPosition(690.f, 190.f);

  // current move indicator
  sf::Text mvi;
  mvi.setFont(noto);
  mvi.setCharacterSize(16);
  mvi.setFillColor(sf::Color::Black);
  mvi.setPosition(720.f, 190.f);

  // moves history
  sf::RectangleShape hisb(sf::Vector2f(180.f, 380.f));
  hisb.setFillColor(sf::Color::White);
  hisb.setPosition(660.f, 220.f);
  sf::Text hist;
  hist.setFont(noto);
  hist.setCharacterSize(12);
  hist.setFillColor(sf::Color::Black);
  hist.setPosition(670.f, 230.f);


  // game loop
  while (window.isOpen()) {
    // event loop
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
        if (game.is_open()) {
          game.flush();
          game.close();
        }
      }
      // splash screen
      if (position.gamestate == 0) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
          load = true;
          position = Position(2);
          resetBoard(position);
          string files;
          int cnt = 0;
          for (auto const& game : filesystem::directory_iterator{games}) {
            files.append(1, '<');
            files.append(to_string(cnt));
            cnt++;
            files.append("> ");
            string file = game.path();
            files += file.substr(9);
            files.append(1, '\n');
          }
          tfiles.setString(files);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
          position = Position(1);
          resetBoard(position);
          game.open(actGame, std::ios::trunc);
          if (!game.is_open()) {
            game.clear();
            game.open(actGame, std::ios::out); // create file
            game.close();
            game.open(actGame);
          }
          bActive.setFillColor(sf::Color::Black);
          wActive.setFillColor(sf::Color::White);
          mi[2].position = sf::Vector2f(750.f, 80.f);
          mi[3].position = sf::Vector2f(750.f, 100.f);
          mvb.setFillColor(sf::Color(200, 200, 0, 200));
          mvi.setString("");
          wTime = 0;
          bTime = 0;
          last = chrono::steady_clock::now();
          bTimer.setString("00:00:00");
          history.clear();
          hist.setString(history);
        }
      }
      // play mode
      if (position.gamestate == 1) {
        // draw offer
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !draw) {
          draw = true;
          itt.setString("Accept a draw? (Y/N)");
        }
        // respond to draw offer
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && draw) {
          position.gamestate = 0;
          draw = false;
          if (!position.player) game << "\n";
          game << "1/2-1/2\n";
          game.flush();
          game.close();
          welcome.setString("     It's a draw!");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && draw) {
          itt.setString("Draw offer declined.");
          draw = false;
        }
        // give up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) && !giveUp) {
          giveUp = true;
          itt.setString("Sure to give up? (Y/N)");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && giveUp) {
          position.gamestate = 0;
          giveUp = false;
          if (position.player) {
            game << "0-1\n";
            welcome.setString("    White gives up!\n");
          } else {
            game << "\n";
            game << "1-0\n";
            welcome.setString("    Black gives up!\n");
          }
          game.flush();
          game.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && giveUp) {
          giveUp = false;
          itt.setString("Okay, move on.");
        }
        // take back move
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) && !takeback) {
          takeback = true;
          string lastMove = position.moves.back();
          int sz = lastMove.size();
          moved = position.takeBackMove();
          if (moved) {
            if (position.mvCount > 0) {
              string newLast = position.moves.back();
              mvi.setString(newLast);
              newLast.back() == '+' ? mvb.setFillColor(sf::Color(200, 100, 0, 200))
                                    : mvb.setFillColor(sf::Color(200, 200, 0, 200));
            } else {
              mvi.setString("");
            }
            if (position.mvCount % 2 == 0) { // last move was white
              if (position.mvCount >= 199) sz += 6;
              else if (position.mvCount >= 19) sz += 5;
              else sz += 4;
            } else sz += 1; // last move was black
            game.seekp(-sz, std::ios_base::end);
            history = history.substr(0, history.size() - sz);
            hist.setString(history);
            moved = false;
          }
        }

        // mouse button pressed
        if (event.type == sf::Event::MouseButtonPressed) {
          if (event.mouseButton.button == sf::Mouse::Right) {
            if (event.mouseButton.x < 640) {
              pair<int, int> f = getField(event.mouseButton.x, event.mouseButton.y);
              setValidMoves(position, validMoves, f);
            }
          }
          if (event.mouseButton.button == sf::Mouse::Left) {
            if (event.mouseButton.x < 640) {
              pair<int, int> from = getField(event.mouseButton.x, event.mouseButton.y);
              if (touched.first == -1) touched = from;
              else if (from == touched) touched = {-1, -1};
            }
          }
        }
        // mouse button released
        if (event.type == sf::Event::MouseButtonReleased) {
          if (event.mouseButton.button == sf::Mouse::Right) {
            validMoves = vector<vector<short>>(8, vector<short>(8, 0));
          }
          if (event.mouseButton.button == sf::Mouse::Left) {
            if (event.mouseButton.x < 640) { // on the board
              pair<int, int> to = getField(event.mouseButton.x, event.mouseButton.y);
              if (touched.first != -1 && touched != to) {
                moved = position.makeMove(touched, to);
                touched = {-1, -1};
              }
            }
          }
        }
      }
      // analyze mode
      if (position.gamestate == 2) {
        // load game file
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) && load) {
          actGame = getGame(0, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && load) {
          actGame = getGame(1, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && load) {
          actGame = getGame(2, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && load) {
          actGame = getGame(3, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && load) {
          actGame = getGame(4, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) && load) {
          actGame = getGame(5, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) && load) {
          actGame = getGame(6, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) && load) {
          actGame = getGame(7, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) && load) {
          actGame = getGame(8, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9) && load) {
          actGame = getGame(9, games);
          if (exists(actGame) && is_regular_file(actGame)) {
            game.open(actGame);
            load = false;
          } else {
            itt.setString("file not found");
          }
        }
      } // end analyze mode
    } // end event loop

    // set timer
    if (position.gamestate == 1) {
      now = steady_clock::now();
      if (now - last > 1s) {
        last = steady_clock::now();
        if (position.player) wTime++;
        else bTime++;
      }
    }

    window.clear(sf::Color(100, 100, 100));

    // draw display
    // timer
    if (position.gamestate == 1) {
      string timer = "";
      if (position.player) {
        window.draw(wActive);
        timer = getTime(wTime);
        wTimer.setString(timer);
      } else {
        window.draw(bActive);
        timer = getTime(bTime);
        bTimer.setString(timer);
      }
      if (position.checkmate.first != -1) {
        if (position.player) {
          bActive.setFillColor(sf::Color(200, 0, 0));
          window.draw(bActive);
        } else {
          wActive.setFillColor(sf::Color(200, 0, 0));
          window.draw(wActive);
        }
      }
      window.draw(wTimer);
      window.draw(bTimer);
    }
    window.draw(hisb);
    // fill history for analyze mode
    if (position.gamestate == 2 && !loaded && !load) {
      mvb.setFillColor(sf::Color(200, 200, 0, 200));
      mvi.setString("");
      mb[0].color = sf::Color::White;
      mb[1].color = sf::Color::White;
      mb[2].color = sf::Color::Black;
      mb[3].color = sf::Color::White;
      mb[4].color = sf::Color::Black;
      mb[5].color = sf::Color::Black;
      mi[2].position = sf::Vector2f(750.f, 80.f);
      mi[3].position = sf::Vector2f(750.f, 100.f);
      mi[2].color = sf::Color::Black;
      mi[3].color = sf::Color::Black;
      history.clear();
      string line;
      while (getline(game, line)) {
        history += line;
        history.append(1, '\n');
      }
      hist.setString(history);
      string info = "loaded ";
      info += actGame.filename();
      itt.setString(info);
      loaded = true;
    }
    // made move
    if (moved) {
      draw = false;
      takeback = false;
      if (position.mvCount % 2 == 0) { // update after move completed
        position.evaluate();
        eval = position.eval;
        if (eval >= 10.0) {
          eval = 10.f;
          mb[2].color = sf::Color(200, 0, 0, 200);
          mb[4].color = sf::Color(200, 0, 0, 200);
          mb[5].color = sf::Color(200, 0, 0, 200);
        }
        if (eval <= -10.0) {
          eval = -10.f;
          mb[0].color = sf::Color(200, 0, 0, 200);
          mb[1].color = sf::Color(200, 0, 0, 200);
          mb[3].color = sf::Color(200, 0, 0, 200);
        }
        mi[2].position = sf::Vector2f(750.f + eval*6.f, 80.f);
        mi[3].position = sf::Vector2f(750.f + eval*6.f, 100.f);
        if (eval > 0) {
          mi[2].color = sf::Color::White;
          mi[3].color = sf::Color::White;
        } else {
          mi[2].color = sf::Color::Black;
          mi[3].color = sf::Color::Black;
        }
      }
      // current move
      position.mvCount > 0 ? mvi.setString(position.moves.back())
                           : mvi.setString("");
      position.checked ? mvb.setFillColor(sf::Color(200, 100, 0, 200))
                       : mvb.setFillColor(sf::Color(200, 200, 0, 200));
      position.checked = false;
      position.info.clear();
      itt.setString("");
      // write to game file and to moves history
      if (position.mvCount % 2 == 0) {
        game << position.moves.back() << "\n";
        history += position.moves.back();
        history += "\n";
      } else {
        game << (position.mvCount / 2) + 1 << ". "
             << position.moves.back() << " ";
        history += to_string((position.mvCount / 2) + 1);
        history += ". ";
        history += position.moves.back();
        history += " ";
      }
      if (position.mvCount > 52 && position.mvCount % 2 == 1) { // trim history string
        auto pos = history.find("\n") + 1;
        history = history.substr(pos, history.size());
      }
      hist.setString(history);
      moved = false;
    }
    window.draw(mb);
    window.draw(mi);
    window.draw(hist);
    // captured pieces
    window.draw(bcw);
    window.draw(bcb);
    int wc = -1;
    int bc = -1;
    for (auto piece : position.captured) {
      sf::Sprite cp;
      switch (piece->getType()) {
      case 'K':
        piece->isWhite() ? cp = wk : cp = bk;
        break;
      case 'Q':
        piece->isWhite() ? cp = wq : cp = bq;
        break;
      case 'R':
        piece->isWhite() ? cp = wr : cp = br;
        break;
      case 'B':
        piece->isWhite() ? cp = wb : cp = bb;
        break;
      case 'N':
        piece->isWhite() ? cp = wn : cp = bn;
        break;
      case 'P':
        piece->isWhite() ? cp = wp : cp = bp;
        break;
      }
      cp.scale(0.3f, 0.3f);
      if (piece->isWhite()) {
        wc++;
        cp.setPosition(650.f + wc*15.f, 115.f);
      } else {
        bc++;
        cp.setPosition(650.f + bc*15.f, 145.f);
      }
      window.draw(cp);
    }
    // current move
    if (position.checkmate.first != -1) {
      mvb.setFillColor(sf::Color(200, 0, 0));
    }
    window.draw(mvb);
    window.draw(mvi);
    // infotext
    window.draw(bit);
    if (!position.info.empty()) itt.setString(position.info);
    window.draw(itt);

    // draw board
    window.draw(bs);

    // draw pieces
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (position.board[row][col]) {
          auto piece = position.board[row][col];
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
          if (row == position.checkmate.first && col == position.checkmate.second) {
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
    // splash screen
    if (position.gamestate == 0) {
      window.draw(bsplash);
      window.draw(spt);
      if (position.checkmate.first != -1) {
        string restart;
        if (position.player) restart = "      White ";
        else restart = "      Black ";
        restart.append("wins!");
        welcome.setString(restart);
      }
      window.draw(welcome);
    }

    // file loader
    if (position.gamestate == 2 && load) {
      window.draw(bloader);
      window.draw(tfiles);
    }

    // display frame
    window.display();

    // stop game when checkmate
    if (position.checkmate.first != -1) {
      position.gamestate = 0;
      if (game.is_open()) {
        if (position.player) {
          game << "\n";
          game << "1-0\n";
        }
        else game << "0-1\n";
        game.flush();
        game.close();
      }
    }

  } // end game loop
} // end main
