#pragma once

#include "pieces.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

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
  void evaluate() {
    pair<int, int> matEval = evaluateBoard(board);
    eval = float(matEval.first) / 100
         - float(matEval.second) / 100;
  }

  // test wether castling is possible
  char castling(Piece* king, pair<int, int> to) {
    if (check(board, king->isWhite())) return 'N'; // king is in check
    string history;
    for (auto mv : moves) {
      history += mv;
    }
    if (king->isWhite()) { // white
      auto pos = history.find("Ke1");
      if (pos != std::string::npos) return 'N'; // king has moved
      if (king->getRow() == 7 && king->getCol() == 4) {
        if (to.first == 7 && to.second == 6) { // kingside
          auto rook = board[7][7];
          if (!rook || rook->getType() != 'R' || rook->isWhite() != king->isWhite()) {
            return 'N'; // no rook
          } else {
            auto pos = history.find("Rh1");
            if (pos != std::string::npos) return 'N'; // rook has moved
          }
          auto pc1 = board[7][5];
          auto pc2 = board[7][6];
          if (pc1 || pc2) return 'N'; // fields occupied
          else {
            // test for check
            board[7][5] = king;
            king->makeMove(7, 5);
            if (check(board, king->isWhite())) {
              board[7][4] = king;
              king->makeMove(7, 4);
              board[7][5] = nullptr;
              return 'N';
            } else {
              board[7][4] = king;
              king->makeMove(7, 4);
              board[7][5] = nullptr;
            }
            board[7][6] = king;
            king->makeMove(7, 6);
            if (check(board, king->isWhite())) {
              board[7][4] = king;
              king->makeMove(7, 4);
              board[7][6] = nullptr;
              return 'N';
            } else {
              board[7][4] = king;
              king->makeMove(7, 4);
              board[7][6] = nullptr;
            }
            return 'K';
          }
        } else if (to.first == 7 && to.second == 2) { // queenside
          auto rook = board[7][0];
          if (!rook || rook->getType() != 'R' || rook->isWhite() != king->isWhite()) {
            return 'N'; // no rook
          } else {
            auto pos = history.find("Ra1");
            if (pos != std::string::npos) return 'N'; // rook has moved
          }
          auto pc1 = board[7][1];
          auto pc2 = board[7][2];
          auto pc3 = board[7][3];
          if (pc1 || pc2 || pc3) return 'N'; // fields occupied
          else {
            // test for check
            board[7][3] = king;
            king->makeMove(7, 3);
            if (check(board, king->isWhite())) {
              board[7][4] = king;
              king->makeMove(7, 4);
              board[7][3] = nullptr;
              return 'N';
            } else {
              board[7][4] = king;
              king->makeMove(7, 4);
              board[7][3] = nullptr;
            }
            board[7][2] = king;
            king->makeMove(7, 2);
            if (check(board, king->isWhite())) {
              board[7][4] = king;
              king->makeMove(7, 4);
              board[7][2] = nullptr;
              return 'N';
            } else {
              board[7][4] = king;
              king->makeMove(7, 4);
              board[7][2] = nullptr;
            }
            return 'Q';
          }
        }
      }
    } else { // black
      auto pos = history.find("Ke8");
      if (pos != std::string::npos) return 'N'; // king has moved
      if (king->getRow() == 0 && king->getCol() == 4) {
        if (to.first == 0 && to.second == 6) { // kingside
          auto rook = board[0][7];
          if (!rook || rook->getType() != 'R' || rook->isWhite() != king->isWhite()) {
            return 'N'; // no rook
          } else {
            auto pos = history.find("Rh8");
            if (pos != std::string::npos) return 'N'; // rook has moved
          }
          auto pc1 = board[0][5];
          auto pc2 = board[0][6];
          if (pc1 || pc2) return 'N'; // fields occupied
          else {
            // test for check
            board[0][5] = king;
            king->makeMove(0, 5);
            if (check(board, king->isWhite())) {
              board[0][4] = king;
              king->makeMove(0, 4);
              board[0][5] = nullptr;
              return 'N';
            } else {
              board[0][4] = king;
              king->makeMove(0, 4);
              board[0][5] = nullptr;
            }
            board[0][6] = king;
            king->makeMove(0, 6);
            if (check(board, king->isWhite())) {
              board[0][4] = king;
              king->makeMove(0, 4);
              board[0][6] = nullptr;
              return 'N';
            } else {
              board[0][4] = king;
              king->makeMove(0, 4);
              board[0][6] = nullptr;
            }
            return 'K';
          }
        } else if (to.first == 0 && to.second == 2) { // queenside
          auto rook = board[0][0];
          if (!rook || rook->getType() != 'R' || rook->isWhite() != king->isWhite()) {
            return 'N'; // no rook
          } else {
            auto pos = history.find("Ra8");
            if (pos != std::string::npos) return 'N'; // rook has moved
          }
          auto pc1 = board[0][1];
          auto pc2 = board[0][2];
          auto pc3 = board[0][3];
          if (pc1 || pc2 || pc3) return 'N'; // fields occupied
          else {
            // test for check
            board[0][3] = king;
            king->makeMove(0, 3);
            if (check(board, king->isWhite())) {
              board[0][4] = king;
              king->makeMove(0, 4);
              board[0][3] = nullptr;
              return 'N';
            } else {
              board[0][4] = king;
              king->makeMove(0, 4);
              board[0][3] = nullptr;
            }
            board[0][2] = king;
            king->makeMove(0, 2);
            if (check(board, king->isWhite())) {
              board[0][4] = king;
              king->makeMove(0, 4);
              board[0][2] = nullptr;
              return 'N';
            } else {
              board[0][4] = king;
              king->makeMove(0, 4);
              board[0][2] = nullptr;
            }
            return 'Q';
          }
        }
      }
    }
    return 'N';
  }

  // take back last move
  bool takeBackMove() {
    string move;
    if (moves.size() > 0) {
      move = moves.back();
      moves.pop_back();
      mvCount--;
    } else return false;
    // castling
    if (move.back() == '0' || move[move.size()-2] == '0') {
      if (move.back() == '+') move.pop_back();
      char form;
      move == "0-0" ? form = 'K' : form = 'Q';
      bool white = (mvCount % 2 == 0);
      if (white && form == 'K') {
        board[7][4] = board[7][6];
        board[7][6] = nullptr;
        auto king = board[7][4];
        king->makeMove(7, 4);
        board[7][7] = board[7][5];
        board[7][5] = nullptr;
        auto rook = board[7][7];
        rook->makeMove(7, 7);
        castled == 1 ? castled = 0 : castled = 2;
      }
      if (white && form == 'Q') {
        board[7][4] = board[7][2];
        board[7][2] = nullptr;
        auto king = board[7][4];
        king->makeMove(7, 4);
        board[7][0] = board[7][3];
        board[7][3] = nullptr;
        auto rook = board[7][0];
        rook->makeMove(7, 0);
        castled == 1 ? castled = 0 : castled = 2;
      }
      if (!white && form == 'K') {
        board[0][4] = board[0][6];
        board[0][6] = nullptr;
        auto king = board[0][4];
        king->makeMove(0, 4);
        board[0][7] = board[0][5];
        board[0][5] = nullptr;
        auto rook = board[0][7];
        rook->makeMove(0, 7);
        castled == 2 ? castled = 0 : castled = 1;
      }
      if (!white && form == 'Q') {
        board[0][4] = board[0][2];
        board[0][2] = nullptr;
        auto king = board[0][4];
        king->makeMove(0, 4);
        board[0][0] = board[0][3];
        board[0][3] = nullptr;
        auto rook = board[0][0];
        rook->makeMove(0, 0);
        castled == 2 ? castled = 0 : castled = 1;
      }
      player = !player;
      return true;
    }
    // en passant
    if (move.back() == 'p' && move[move.size()-2] == 'e') {
      move.pop_back();
      move.pop_back();
      vector<int> coords = parseMove(move);
      auto pc = board[coords[4]][coords[3]];
      board[coords[4]][coords[3]] = nullptr;
      board[coords[1]][coords[0]] = pc;
      pc->makeMove(coords[1], coords[0]);
      auto cp = captured.back();
      captured.pop_back();
      bool white = (mvCount % 2 == 0);
      if (white) {
        board[coords[4]+1][coords[3]] = cp;
      } else {
        board[coords[4]-1][coords[3]] = cp;
      }
      player = !player;
      return true;
    }
    vector<int> coords = parseMove(move);
    auto pc = board[coords[4]][coords[3]];
    // promotion
    if (move.back() == 'Q' || move[move.size()-2] == 'Q') {
      bool white = coords[4] == 0;
      pc = new Pawn(white, coords[1], coords[0]);
    }
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
  bool makeMove(pair<int, int> from, pair<int, int> to) {
    auto pcf = board[from.first][from.second];
    auto pct = board[to.first][to.second];
    bool cap = false;
    bool prom = false;
    bool passant = false;

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
      char form = castling(pcf, to);
      if (form == 'K') {
        castled = castled > 0 ? 3 : cast;
        move = "0-0";
        // make king move
        board[from.first][from.second+2] = pcf;
        pcf->makeMove(from.first, from.second+2);
        board[from.first][from.second] = nullptr;
        // make rook move
        auto rook = board[from.first][7];
        board[from.first][from.second+1] = rook;
        rook->makeMove(from.first, from.second+1);
        board[from.first][7] = nullptr;
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
        board[from.first][from.second-2] = pcf;
        pcf->makeMove(from.first, from.second-2);
        board[from.first][from.second] = nullptr;
        // make rook move
        auto rook = board[from.first][0];
        board[from.first][from.second-1] = rook;
        rook->makeMove(from.first, from.second-1);
        board[from.first][0] = nullptr;
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

      // en passant
      if (pcf && pcf->getType() == 'P') {
        if (pcf->isWhite() && pcf->getRow() == 3) { // white
          if (!pct) {
            auto pawn = board[pcf->getRow()][to.second];
            if (pawn && pawn->getType() == 'P' && pawn->isWhite() != player) {
              string lastMove = moves.back();
              pair<int, int> epTo = make_pair(pawn->getRow(), pawn->getCol());
              auto epPc = new Pawn(!player, 1, to.second);
              string ep = convertFromBoard(false, epPc, epTo);
              delete epPc;
              if (ep != lastMove) {
                info = "illegal move";
                return false;
              } else {
                passant = true;
                cap = true;
                captured.push_back(pawn);
                board[pcf->getRow()][to.second] = nullptr;
              }
            }
          }
        } else if (!pcf->isWhite() && pcf->getRow() == 4) { // black
          if (!pct) {
            auto pawn = board[pcf->getRow()][to.second];
            if (pawn && pawn->getType() == 'P' && pawn->isWhite() != player) {
              string lastMove = moves.back();
              pair<int, int> epTo = make_pair(pawn->getRow(), pawn->getCol());
              auto epPc = new Pawn(!player, 6, to.second);
              string ep = convertFromBoard(false, epPc, epTo);
              delete epPc;
              if (ep != lastMove) {
                info = "illegal move";
                return false;
              } else {
                passant = true;
                cap = true;
                captured.push_back(pawn);
                board[pcf->getRow()][to.second] = nullptr;
              }
            }
          }
        }
      } // end en passant

      string move = convertFromBoard(cap, pcf, to);
      if (passant) move.append("ep");

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


      // make move
      board[to.first][to.second] = pcf;
      pcf->makeMove(to.first, to.second);
      board[from.first][from.second] = nullptr;
      if (check(board, player)) { // gives itself check
        info = "observe check";
        // reset move
        board[to.first][to.second] = pct;
        if (prom) { // promotion
          pcf = new Pawn(pcf->isWhite(), pcf->getRow(), pcf->getCol());
        }
        board[from.first][from.second] = pcf;
        pcf->makeMove(from.first, from.second);
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

