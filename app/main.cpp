#include <SFML/Graphics.hpp>
#include "pieces.hpp"
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

vector<vector<Piece*>> board(8, vector<Piece*>(8));
vector<vector<short>> validMoves(8, vector<short>(8, 0));
bool player = true; // white to move
pair<int, int> touched{-1, -1};

void makeMove(vector<vector<Piece*>>& bd, pair<int, int> from, pair<int, int> to) {
  auto pcf = bd[from.first][from.second];
  if (!pcf) {
    cout << "no piece under cursor\n";
    touched = {-1, -1};
    return;
  }
  if (pcf->isWhite() != player) {
    cout << "it's not your turn\n";
    touched = {-1, -1};
    return;
  }
  if (pcf->isValid(bd, to.first, to.second)) {
    auto pct = bd[to.first][to.second];
    if (pct) {
      pct->capture();
      delete pct;
    }
    bd[to.first][to.second] = pcf;
    bd[from.first][from.second] = nullptr;
    pcf->makeMove(to.first, to.second);
    touched = {-1, -1};
    player = !player;
    cout << "ok\n";
  } else {
    cout << "illegal move!\n";
    touched = {-1, -1};
  }
}

void setValidMoves(vector<vector<Piece*>>& bd, Piece* pc) {
  if (!pc) return;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (pc->isValid(bd, row, col)) {
        auto current = bd[row][col];
        if (!current) validMoves[row][col] = 1; 
        else if (pc->isWhite() != current->isWhite()) validMoves[row][col] = 2;
      }
    }
  }
}

pair<int, int> getField(int x, int y) {
  int fx = x / 80;
  int fy = y / 80;
  auto field = make_pair(fy, fx);
  return field;
}

void reset_board(vector<vector<Piece*>>& bd) {
  for (auto rank : bd) {
    for (auto piece : rank) {
      delete piece;
    }
  }
  // rank 8 (black)
  bd[0][0] = new Rook(0,0,0);
  bd[0][1] = new Knight(0,0,1);
  bd[0][2] = new Bishop(0,0,2);
  bd[0][3] = new Queen(0,0,3);
  bd[0][4] = new King(0,0,4);
  bd[0][5] = new Bishop(0,0,5);
  bd[0][6] = new Knight(0,0,6);
  bd[0][7] = new Rook(0,0,7);
  // rank 7 (black)
  bd[1][0] = new Pawn(0,1,0);
  bd[1][1] = new Pawn(0,1,1);
  bd[1][2] = new Pawn(0,1,2);
  bd[1][3] = new Pawn(0,1,3);
  bd[1][4] = new Pawn(0,1,4);
  bd[1][5] = new Pawn(0,1,5);
  bd[1][6] = new Pawn(0,1,6);
  bd[1][7] = new Pawn(0,1,7);
  // rank 2 (white)
  bd[6][0] = new Pawn(1,6,0);
  bd[6][1] = new Pawn(1,6,1);
  bd[6][2] = new Pawn(1,6,2);
  bd[6][3] = new Pawn(1,6,3);
  bd[6][4] = new Pawn(1,6,4);
  bd[6][5] = new Pawn(1,6,5);
  bd[6][6] = new Pawn(1,6,6);
  bd[6][7] = new Pawn(1,6,7);
  // rank 1 (white)
  bd[7][0] = new Rook(1,7,0);
  bd[7][1] = new Knight(1,7,1);
  bd[7][2] = new Bishop(1,7,2);
  bd[7][3] = new Queen(1,7,3);
  bd[7][4] = new King(1,7,4);
  bd[7][5] = new Bishop(1,7,5);
  bd[7][6] = new Knight(1,7,6);
  bd[7][7] = new Rook(1,7,7);
}

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  auto window = sf::RenderWindow{ {640u, 640u}, "Think Chess++", sf::Style::Default, settings };
  window.setFramerateLimit(10);

  reset_board(board);

  sf::Texture bi;
  bi.loadFromFile("../img/chessboard.jpg");
  sf::Sprite bs;
  bs.setTexture(bi);

  sf::Texture figures;
  figures.loadFromFile("../img/figures.png");

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

  sf::CircleShape valid(20.f);

  // game loop
  while (window.isOpen()) {
    // event loop
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      // mouse button pressed
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
          pair<int, int> f = getField(event.mouseButton.x, event.mouseButton.y);
          setValidMoves(board, board[f.first][f.second]);
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
          pair<int, int> f = getField(event.mouseButton.x, event.mouseButton.y);
          if (touched.first == -1) touched = f;
          else if (f == touched) touched = {-1, -1};
        }
      }
      // mouse button released
      if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Right) {
          validMoves = vector<vector<short>>(8, vector<short>(8, 0));
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
          pair<int, int> f = getField(event.mouseButton.x, event.mouseButton.y);
          if (touched.first != -1 && touched != f)
            makeMove(board, touched, f);
        }
      }
    } // end event loop

    // draw board
    window.draw(bs);
    // draw pieces
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (board[row][col]) {
          auto piece = board[row][col];
          sf::Sprite pc;
          switch (piece->getType()) {
          case 'k':
            piece->isWhite() ? pc = wk : pc = bk;
            break;
          case 'q':
            piece->isWhite() ? pc = wq : pc = bq;
            break;
          case 'r':
            piece->isWhite() ? pc = wr : pc = br;
            break;
          case 'b':
            piece->isWhite() ? pc = wb : pc = bb;
            break;
          case 'n':
            piece->isWhite() ? pc = wn : pc = bn;
            break;
          case 'p':
            piece->isWhite() ? pc = wp : pc = bp;
            break;
          }
          if (row == touched.first && col == touched.second) {
            pc.setPosition(col*80.f, row*80.f);
          } else {
            pc.setPosition(col*80.f + 10.f, row*80.f + 10.f);
          }
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

  } // end game loop
} // end main
