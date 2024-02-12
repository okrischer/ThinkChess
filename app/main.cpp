#include <SFML/Graphics.hpp>
#include "pieces.hpp"
#include <iostream>
#include <vector>
using namespace std;

vector<vector<Piece*>> board(8, vector<Piece*>(8));

void reset_board(vector<vector<Piece*>>& bd) {
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
  auto window = sf::RenderWindow{ {1000u, 640u}, "Think Chess", sf::Style::Default, settings };
  window.setFramerateLimit(1);

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

  // begin animation
  while (window.isOpen()) {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // draw board
    window.draw(bs);
    // draw pieces
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (board[row][col]) {
          auto piece = board[row][col];
          sf::Sprite pc;
          switch (piece->get_type()) {
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
          pc.setPosition(col*80.f + 10.f, row*80.f + 10.f);
          window.draw(pc);
        }
      }
    }

    // display frame
    window.display();
  }
}
