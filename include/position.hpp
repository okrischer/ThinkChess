#pragma once

#include "pieces.hpp"
#include <iostream>

using namespace std;

// convert columns to files
char colToFile(int col);

// convert rows to ranks
char rowToRank(int row);

// get board coordinates
pair<int, int> getField(int x, int y);

// convert board coordinates to moves notation
string convertFromBoard(bool cap, Piece* from, pair<int, int> to);

// test wether castling is possible
char castling(vector<vector<Piece*>>& bd, Piece* king, pair<int, int> to);

// test for check
bool check(const vector<vector<Piece*>>& bd, bool white);

// check wether a given check can be resolved
bool resolveCheck(vector<vector<Piece*>>& bd, bool player);

// print board for debug
void printBoard(const vector<vector<Piece*>>& bd);


class Position {
public:
  ~Position() {}
  Position() : gamestate{1}, player{true},
               board{8, vector<Piece*>(8)},
               mvCount{0}, castled{0},
               checkmate{-1, -1}, eval{0.f}
  {}

  // returns a material evaluation for both players
  pair<int, int> evaluate() {
    int white = 0;
    int black = 0;
    
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        auto pc = board[row][col];
        if (pc) {
          pc->isWhite() ? white += pc->getValue() : black += pc->getValue();
        }
      }
    }
    pair<int, int> eval = make_pair(white, black);
    return eval;
  }

  // make a move
  bool makeMove(pair<int, int>& td, pair<int, int> to) {
    auto pcf = board[td.first][td.second];
    auto pct = board[to.first][to.second];
    bool cap = false;
    bool prom = false;

    if (!pcf) {
      cout << "no piece under cursor\n";
      td = {-1, -1};
      return false;
    }
    if (pcf->isWhite() != player) {
      cout << "it's not your turn\n";
      td = {-1, -1};
      return false;
    }

    // castling
    short cast = player ? 1 : 2;
    if (castled < 3 && castled != cast && pcf->getType() == 'K' && !pct) {
      string move = "";
      char form = castling(board, pcf, to);
      if (form == 'K') {
        castled = castled > 0 ? 3 : cast;
        move = "0-0";
        // make king move
        board[td.first][td.second+2] = pcf;
        pcf->makeMove(td.first, td.second+2);
        board[td.first][td.second] = nullptr;
        // make rook move
        auto rook = board[td.first][7];
        board[td.first][td.second+1] = rook;
        rook->makeMove(td.first, td.second+1);
        board[td.first][7] = nullptr;
        if (check(board, !player)) { // gives opponent check
          if (resolveCheck(board, !player)) {
            move.append(1, '+');
          } else { // cannot get out of check
            move.append(1, '#');
            moves.push_back(move);
            cout << moves.back() << "\n";
            checkmate = to;
            return true;
          }
        }
        // complete move
        moves.push_back(move);
        cout << moves.back() << "\n";
        td = {-1, -1};
        player = !player;
        return true;
      } else if (form == 'Q') {
        castled = castled > 0 ? 3 : cast;
        move = "0-0-0";
        // make king move
        board[td.first][td.second-2] = pcf;
        pcf->makeMove(td.first, td.second-2);
        board[td.first][td.second] = nullptr;
        // make rook move
        auto rook = board[td.first][0];
        board[td.first][td.second-1] = rook;
        rook->makeMove(td.first, td.second-1);
        board[td.first][0] = nullptr;
        if (check(board, !player)) { // gives opponent check
          if (resolveCheck(board, !player)) {
            move.append(1, '+');
          } else { // cannot get out of check
            move.append(1, '#');
            moves.push_back(move);
            cout << moves.back() << "\n";
            checkmate = to;
            return true;
          }
        }
        // complete move
        moves.push_back(move);
        cout << moves.back() << "\n";
        td = {-1, -1};
        player = !player;
        return true;
      }
    } // end castling

    // valid move?
    if (pcf->isValid(board, to.first, to.second)) {
      // can capture?
      if (pct && pct->isWhite() != pcf->isWhite()) {
        cap = true;
        captured.push_back(pct);
      } else if (pct) { // same color
        cout << "illegal move!\n";
        td = {-1, -1};
        return false;
      }

      string move = convertFromBoard(cap, pcf, to);

      // promotion
      if (pcf && pcf->getType() == 'P') {
        if (pcf->isWhite() && pcf->getRow() == 1) { // white
          pcf = new Queen(1, pcf->getRow(), pcf->getCol());
          move.append("=Q");
          prom = true;
        } else if (!pcf->isWhite() && pcf->getRow() == 6) { // black
          pcf = new Queen(0, pcf->getRow(), pcf->getCol());
          move.append("=Q");
          prom = true;
        }
      } // end promotion

      // TODO: en passant

      // make move
      board[to.first][to.second] = pcf;
      pcf->makeMove(to.first, to.second);
      board[td.first][td.second] = nullptr;
      if (check(board, player)) { // gives itself check
        cout << "observe check!\n";
        // reset move
        board[to.first][to.second] = pct;
        if (prom) { // promotion
          pcf = new Pawn(pcf->isWhite(), pcf->getRow(), pcf->getCol());
        }
        board[td.first][td.second] = pcf;
        pcf->makeMove(td.first, td.second);
        if (cap) captured.pop_back();
        td = {-1, -1};
        return false;
      } else if (check(board, !player)) { // gives opponent check
        if (resolveCheck(board, !player)) {
          move.append(1, '+');
        } else { // cannot get out of check
          move.append(1, '#');
          moves.push_back(move);
          cout << moves.back() << "\n";
          checkmate = to;
          return true;
        }
      }
      // complete move
      moves.push_back(move);
      cout << moves.back() << "\n";
      td = {-1, -1};
      player = !player;
      return true;

    // illegal move
    } else {
      cout << "illegal move!\n";
      td = {-1, -1};
      return false;
    }
  }

  // gamestate: 1 = running, 0 = game over
  short gamestate;

  // player on turn, starting with white
  bool player;
  
  // matrix of pieces representing the board
  vector<vector<Piece*>> board;

  // vector of moves, used as a stack
  vector<string> moves;

  // list of captured pieces, used as a stack
  vector<Piece*> captured;

  // total count of moves
  int mvCount;
  
  // castled: 0 = no, 1 = white, 2 = black, 3 = both
  short castled;
  
  // coordinates of the piece, which gave checkmate
  std::pair<int, int> checkmate;
  
  // basic evaluation of positions
  float eval;
}; // end Position

