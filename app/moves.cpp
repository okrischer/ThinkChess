#include "moves.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

// convert columns to files
char colToFile(int col) {
  char file = 97 + col;
  return file;
}

// convert rows to ranks
char rowToRank(int row) {
  char rank = 56 - row;
  return rank;
}

// get board coordinates
pair<int, int> getField(int x, int y) {
  int fx = x / 80;
  int fy = y / 80;
  auto field = make_pair(fy, fx);
  return field;
}

// convert board coordinates to moves notation
string convertFromBoard(bool cap, Piece* from, pair<int, int> to) {
  string move;
  char type = from->getType();
  if (type != 'P') {
    move.append(1, type);
  }
  move.append(1, colToFile(from->getCol()));
  move.append(1, rowToRank(from->getRow()));
  if (cap) {
    move.append(1, 'x');
  } else {
    move.append(1, '-');
  }
  move.append(1, colToFile(to.second));
  move.append(1, rowToRank(to.first));
  return move;
}

// set valid moves for display
void setValidMoves(vector<vector<Piece*>>& bd,
                   vector<vector<short>>& vm,
                   Piece* pc)
{
  if (!pc) return;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (pc->isValid(bd, row, col)) {
        auto current = bd[row][col];
        if (!current) vm[row][col] = 1;
        else if (pc->isWhite() != current->isWhite()) vm[row][col] = 2;
      }
    }
  }
}

// test for check
bool check(vector<vector<Piece*>>& bd, bool white) {
  Piece* king;
  bool check = false;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      auto current = bd[row][col];
      if (current && current->getType() == 'K' &&
          current->isWhite() == white)
      {
        king = current;
      }
    }
  }
  if (!king) {
    throw domain_error{"found no king in check()"};
  } else {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        auto current = bd[row][col];
        if (current && current->isWhite() != white) {
          if (current->isValid(bd, king->getRow(), king->getCol())) {
            check = true;
          }
        }
      }
    }
  }
  return check;
}

// test wether castling is possible
char castling(vector<vector<Piece*>>& bd, Piece* king, pair<int, int> to) {
  if (check(bd, king->isWhite())) return 'N'; // king is in check
  if (king->isWhite()) { // white
    if (king->getRow() == 7 && king->getCol() == 4) { // todo: king has not yet moved
      if (to.first == 7 && to.second == 6) { // kingside
        auto rook = bd[7][7];
        if (!rook || rook->getType() != 'R' || rook->isWhite() != king->isWhite()) {
          // todo: rook has moved
          return 'N';
        }
        auto pc1 = bd[7][5];
        auto pc2 = bd[7][6];
        if (pc1 || pc2) return 'N'; // fields occupied
        else {
          // test for check
          bd[7][5] = king;
          king->makeMove(7, 5);
          if (check(bd, king->isWhite())) {
            bd[7][4] = king;
            king->makeMove(7, 4);
            bd[7][5] = nullptr;
            return 'N';
          } else {
            bd[7][4] = king;
            king->makeMove(7, 4);
            bd[7][5] = nullptr;
          }
          bd[7][6] = king;
          king->makeMove(7, 6);
          if (check(bd, king->isWhite())) {
            bd[7][4] = king;
            king->makeMove(7, 4);
            bd[7][6] = nullptr;
            return 'N';
          } else {
            bd[7][4] = king;
            king->makeMove(7, 4);
            bd[7][6] = nullptr;
          }
          return 'K';
        }
      } else if (to.first == 7 && to.second == 2) { // queenside
        auto rook = bd[7][0];
        if (!rook || rook->getType() != 'R' || rook->isWhite() != king->isWhite()) {
          // todo: rook has moved
          return 'N';
        }
        auto pc1 = bd[7][1];
        auto pc2 = bd[7][2];
        auto pc3 = bd[7][3];
        if (pc1 || pc2 || pc3) return 'N'; // fields occupied
        else {
          // test for check
          bd[7][3] = king;
          king->makeMove(7, 3);
          if (check(bd, king->isWhite())) {
            bd[7][4] = king;
            king->makeMove(7, 4);
            bd[7][3] = nullptr;
            return 'N';
          } else {
            bd[7][4] = king;
            king->makeMove(7, 4);
            bd[7][3] = nullptr;
          }
          bd[7][2] = king;
          king->makeMove(7, 2);
          if (check(bd, king->isWhite())) {
            bd[7][4] = king;
            king->makeMove(7, 4);
            bd[7][2] = nullptr;
            return 'N';
          } else {
            bd[7][4] = king;
            king->makeMove(7, 4);
            bd[7][2] = nullptr;
          }
          return 'Q';
        }
      }
    }
  } else { // black
    if (king->getRow() == 0 && king->getCol() == 4) { // todo: king has not yet moved
      if (to.first == 0 && to.second == 6) { // kingside
        auto rook = bd[0][7];
        if (!rook || rook->getType() != 'R' || rook->isWhite() != king->isWhite()) {
          // todo: rook has moved
          return 'N';
        }
        auto pc1 = bd[0][5];
        auto pc2 = bd[0][6];
        if (pc1 || pc2) return 'N'; // fields occupied
        else {
          // test for check
          bd[0][5] = king;
          king->makeMove(0, 5);
          if (check(bd, king->isWhite())) {
            bd[0][4] = king;
            king->makeMove(0, 4);
            bd[0][5] = nullptr;
            return 'N';
          } else {
            bd[0][4] = king;
            king->makeMove(0, 4);
            bd[0][5] = nullptr;
          }
          bd[0][6] = king;
          king->makeMove(0, 6);
          if (check(bd, king->isWhite())) {
            bd[0][4] = king;
            king->makeMove(0, 4);
            bd[0][6] = nullptr;
            return 'N';
          } else {
            bd[0][4] = king;
            king->makeMove(0, 4);
            bd[0][6] = nullptr;
          }
          return 'K';
        }
      } else if (to.first == 0 && to.second == 2) { // queenside
        auto rook = bd[0][0];
        if (!rook || rook->getType() != 'R' || rook->isWhite() != king->isWhite()) {
          // todo: rook has moved
          return 'N';
        }
        auto pc1 = bd[0][1];
        auto pc2 = bd[0][2];
        auto pc3 = bd[0][3];
        if (pc1 || pc2 || pc3) return 'N'; // fields occupied
        else {
          // test for check
          bd[0][3] = king;
          king->makeMove(0, 3);
          if (check(bd, king->isWhite())) {
            bd[0][4] = king;
            king->makeMove(0, 4);
            bd[0][3] = nullptr;
            return 'N';
          } else {
            bd[0][4] = king;
            king->makeMove(0, 4);
            bd[0][3] = nullptr;
          }
          bd[0][2] = king;
          king->makeMove(0, 2);
          if (check(bd, king->isWhite())) {
            bd[0][4] = king;
            king->makeMove(0, 4);
            bd[0][2] = nullptr;
            return 'N';
          } else {
            bd[0][4] = king;
            king->makeMove(0, 4);
            bd[0][2] = nullptr;
          }
          return 'Q';
        }
      }
    }
  }
  return 'N';
}

// make a move
void makeMove(vector<vector<Piece*>>& bd,
              list::List<string>* mv,
              list::List<Piece*>* cp,
              pair<int, int>& td,
              pair<int, int> to,
              bool& player,
              pair<int, int>& checkmate,
              short& castled)
{
  auto pcf = bd[td.first][td.second];
  auto pct = bd[to.first][to.second];
  bool cap = false;
  if (!pcf) {
    cout << "no piece under cursor\n";
    td = {-1, -1};
    return;
  }
  if (pcf->isWhite() != player) {
    cout << "it's not your turn\n";
    td = {-1, -1};
    return;
  }
  // castling
  short cast = player ? 1 : 2;
  if (castled < 3 && castled != cast && pcf->getType() == 'K' && !pct) {
    string move = "";
    char form = castling(bd, pcf, to);
    if (form == 'K') {
      castled = castled > 0 ? 3 : cast;
      move = "0-0";
      // make king move
      bd[td.first][td.second+2] = pcf;
      pcf->makeMove(td.first, td.second+2);
      bd[td.first][td.second] = nullptr;
      // make rook move
      auto rook = bd[td.first][7];
      bd[td.first][td.second+1] = rook;
      rook->makeMove(td.first, td.second+1);
      bd[td.first][7] = nullptr;
      if (check(bd, !player)) { // gives opponent check
        if (resolveCheck(bd, !player)) {
          move.append(1, '+');
        } else { // cannot get out of check
          move.append(1, '#');
          mv->push_front(move);
          cout << mv->peek(1) << "\n";
          checkmate = getKing(bd, !player);
          return;
        }
      }
      // complete move
      mv->push_front(move);
      cout << mv->peek(1) << "\n";
      td = {-1, -1};
      player = !player;
      return;
    } else if (form == 'Q') {
      castled = castled > 0 ? 3 : cast;
      move = "0-0-0";
      // make king move
      bd[td.first][td.second-2] = pcf;
      pcf->makeMove(td.first, td.second-2);
      bd[td.first][td.second] = nullptr;
      // make rook move
      auto rook = bd[td.first][0];
      bd[td.first][td.second-1] = rook;
      rook->makeMove(td.first, td.second-1);
      bd[td.first][0] = nullptr;
      if (check(bd, !player)) { // gives opponent check
        if (resolveCheck(bd, !player)) {
          move.append(1, '+');
        } else { // cannot get out of check
          move.append(1, '#');
          mv->push_front(move);
          cout << mv->peek(1) << "\n";
          checkmate = getKing(bd, !player);
          return;
        }
      }
      // complete move
      mv->push_front(move);
      cout << mv->peek(1) << "\n";
      td = {-1, -1};
      player = !player;
      return;
    } // else do nothing and continue with check for valid move
  }
  // valid move?
  if (pcf->isValid(bd, to.first, to.second)) {
    // can capture?
    if (pct && pct->isWhite() != pcf->isWhite()) {
      cap = true;
      cp->push_front(pct);
    } else if (pct) { // same color
      cout << "illegal move!\n";
      td = {-1, -1};
      return;
    }
    // legal move
    string move = convertFromBoard(cap, pcf, to);
    bd[to.first][to.second] = pcf;
    bd[td.first][td.second] = nullptr;
    pcf->makeMove(to.first, to.second);
    // check?
    if (check(bd, player)) { // gives itself check
      cout << "illegal move!\n";
      bd[to.first][to.second] = pct;
      bd[td.first][td.second] = pcf;
      pcf->makeMove(td.first, td.second);
      td = {-1, -1};
      return;
    }
    if (check(bd, !player)) { // gives opponent check
      if (resolveCheck(bd, !player)) {
        move.append(1, '+');
      } else { // cannot get out of check
        move.append(1, '#');
        mv->push_front(move);
        cout << mv->peek(1) << "\n";
        checkmate = getKing(bd, !player);
        return;
      }
    }
    mv->push_front(move);
    cout << mv->peek(1) << "\n";
    td = {-1, -1};
    player = !player;
  // illegal move
  } else {
    cout << "illegal move!\n";
    td = {-1, -1};
  }
}

// check wether a given check can be resolved
bool resolveCheck(vector<vector<Piece*>>& bd, bool white) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      auto current = bd[row][col];
      // get every piece of given color
      if (current && current->isWhite() == white) {
        for (int rr = 0; rr < 8; rr++) {
          for (int cc = 0; cc < 8; cc++) {
            if (current->isValid(bd, rr, cc)) {
              auto pct = bd[rr][cc];
              if (!pct || pct->isWhite() != white) {
                // make move and test for check
                bd[rr][cc] = current;
                current->makeMove(rr, cc);
                //todo: finds incorrect save moves
                if (!check(bd, white)) {
                  cout << "found save move: " <<  current->getType()
                       << (current->isWhite() ? 'w' : 'b')
                       << current->getRow() << current->getCol() << "\n";
                  bd[rr][cc] = pct;
                  current->makeMove(row, col);
                  return true;
                } else {
                  bd[rr][cc] = pct;
                  current->makeMove(row, col);
                }
              }
            }
          }
        }
      }
    }
  }
  return false;
}

// get opponents king coordinates
pair<int, int> getKing(vector<vector<Piece*>>& bd, bool white) {
  Piece* king;
  pair<int, int> coord;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      auto current = bd[row][col];
      if (current && current->getType() == 'K' &&
          current->isWhite() == white) {
        king = current;
      }
    }
  }
  if (!king) {
    throw domain_error{"found no king in getKing()"};
  } else {
    coord = make_pair(king->getRow(), king->getCol());
  }
  return coord;
}

// reset board for new game
void resetBoard(vector<vector<Piece*>>& bd,
                 list::List<string>* mv,
                 list::List<Piece*>* cp)
{
  // reset moves and captured pieces
  delete mv;
  mv = new list::List<string>;
  delete cp;
  cp = new list::List<Piece*>;
  // reset board
  for (auto rank : bd) {
    for (auto piece : rank) {
      delete piece;
    }
  }
  // set board to initial position
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

