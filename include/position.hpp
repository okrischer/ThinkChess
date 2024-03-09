#pragma once

#include "pieces.hpp"
#include <iostream>

using namespace std;

// convert columns to files
char colToFile(int col);

// convert rows to ranks
char rowToRank(int row);

// convert files to colums
int fileToCol(char file);

// convert ranks to rows
int rankToRow(char rank);

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

// evaluate board
pair<int, int> evaluateBoard(const vector<vector<Piece*>>& bd);

// parse move string and return coordinates
vector<int> parseMove(string move);

// print board for debug
void printBoard(const vector<vector<Piece*>>& bd);


class Position {
public:
  ~Position() {}
  Position(short gs) : gamestate{gs}, player{true},
               board{8, vector<Piece*>(8)},
               mvCount{0}, castled{0}, checkmate{-1, -1},
               checked{false}, eval{0.f}
  {}

  // returns a material evaluation for both players
  pair<int, int> evaluate() {
    return evaluateBoard(board);
  }

  // take back last move
  bool takeBackMove() {
    string move;
    if (moves.size() > 0) {
      move = moves.back();
      moves.pop_back();
      mvCount--;
    } else return false;
    vector<int> coords = parseMove(move);
    auto pc = board[coords[4]][coords[3]];
    board[coords[1]][coords[0]] = pc;
    pc->makeMove(coords[1], coords[0]);
    if (coords[2] == 0) { // normal move
      board[coords[4]][coords[3]] = nullptr;
    } else { // captured
      auto cp = captured.back();
      board[coords[4]][coords[3]] = cp;
      captured.pop_back();
    }
    player = !player;
    return true;
  }

  // make a move
  bool makeMove(pair<int, int> td, pair<int, int> to) {
    auto pcf = board[td.first][td.second];
    auto pct = board[to.first][to.second];
    bool cap = false;
    bool prom = false;

    if (!pcf) {
      info = "no piece selected";
      return false;
    }
    if (pcf->isWhite() != player) {
      info = "it's not your turn";
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
            checked = true;
          } else { // cannot get out of check
            move.append(1, '#');
            moves.push_back(move);
            checkmate = to;
            mvCount++;
            return true;
          }
        }
        // complete move
        moves.push_back(move);
        player = !player;
        mvCount++;
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
            checked = true;
          } else { // cannot get out of check
            move.append(1, '#');
            moves.push_back(move);
            checkmate = to;
            mvCount++;
            return true;
          }
        }
        // complete move
        moves.push_back(move);
        player = !player;
        mvCount++;
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
        info = "illegal move";
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
        info = "observe check";
        // reset move
        board[to.first][to.second] = pct;
        if (prom) { // promotion
          pcf = new Pawn(pcf->isWhite(), pcf->getRow(), pcf->getCol());
        }
        board[td.first][td.second] = pcf;
        pcf->makeMove(td.first, td.second);
        if (cap) captured.pop_back();
        return false;
      } else if (check(board, !player)) { // gives opponent check
        if (resolveCheck(board, !player)) {
          move.append(1, '+');
          checked = true;
        } else { // cannot get out of check
          move.append(1, '#');
          moves.push_back(move);
          checkmate = to;
          mvCount++;
          return true;
        }
      }
      // complete move
      moves.push_back(move);
      player = !player;
      mvCount++;
      return true;

    // illegal move
    } else {
      info = "illegal move";
      return false;
    }
  }

  // gamestate: 0 = game over, 1 = 2-player, 2 = analyze 
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
  
  // last move gave check
  bool checked;

  // basic evaluation of positions
  float eval;

  // infotext
  string info;
}; // end Position

