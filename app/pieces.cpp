#include "pieces.hpp"
#include <vector>

using namespace std;

bool King::isValid(const vector<vector<Piece*>>& bd, int r, int c) {
  bool valid = abs(r-row) <= 1 && abs(c-col) <= 1;
  return valid;
}

bool Knight::isValid(const vector<vector<Piece*>>& bd, int r, int c) {
  bool valid = (abs(r-row) == 1 && abs(c-col) == 2) ||
               (abs(r-row) == 2 && abs(c-col) == 1);
  return valid;
}

bool Rook::isValid(const vector<vector<Piece*>>& bd, int r, int c) {
  bool valid = r == row || c == col;
  if (r == row && abs(c-col) > 1) { // same row
    if (c < col) { // left
      for (int cc = c+1; cc < col; cc++) {
        auto pc = bd[r][cc];
        if (pc) { valid = false; break; }
      }
    } else { // right
      for (int cc = col+1; cc < c; cc++) {
        auto pc = bd[r][cc];
        if (pc) { valid = false; break; }
      }
    }
  }
  if (c == col && abs(r-row) > 1) { // same column
    if (r < row) { // top
      for (int rr = r+1; rr < row; rr++) {
        auto pc = bd[rr][c];
        if (pc) { valid = false; break; }
      }
    } else { // down
      for (int rr = row+1; rr < r; rr++) {
        auto pc = bd[rr][c];
        if (pc) { valid = false; break; }
      }
    }
  }
  return valid;
}

bool Bishop::isValid(const vector<vector<Piece*>>& bd, int r, int c) {
  bool valid = r-c == row-col || r+c == row+col;
  if (r-c == row-col) { // same major diagonal
    if (r < row) { // upper
      for (int rr = row-1; rr > r; rr--) {
        for (int cc = col-1; cc > c; cc--) {
          if (rr-cc == row-col) {
            auto pc = bd[rr][cc];
            if (pc) { valid = false; break; }
          }
        }
      }
    } else { // lower
      for (int rr = row+1; rr < r; rr++) {
        for (int cc = col+1; cc < c; cc++) {
          if (rr-cc == row-col) {
            auto pc = bd[rr][cc];
            if (pc) { valid = false; break; }
          }
        }
      }
    }
  }
  if (r+c == row+col) { // same minor diagonal
    if (r < row) { // upper
      for (int rr = row-1; rr > r; rr--) {
        for (int cc = col+1; cc < c; cc++) {
          if (rr+cc == row+col) {
            auto pc = bd[rr][cc];
            if (pc) { valid = false; break; }
          }
        }
      }
    } else { // lower
      for (int rr = row+1; rr < r; rr++) {
        for (int cc = col-1; cc > c; cc--) {
          if (rr+cc == row+col) {
            auto pc = bd[rr][cc];
            if (pc) { valid = false; break; }
          }
        }
      }
    }
  }
  return valid;
}

bool Queen::isValid(const vector<vector<Piece*>>& bd, int r, int c) {
  auto rook = new Rook(white, row, col);
  auto bishop = new Bishop(white, row, col);
  bool valid = rook->isValid(bd, r, c) || bishop->isValid(bd, r, c);
  delete rook;
  delete bishop;
  return valid;
}

bool Pawn::isValid(const vector<vector<Piece*>>& bd, int r, int c) {
  bool valid = false;
  auto pc = bd[r][c];
  if (white) {
    // regular move
    if (r == row-1 && c == col) {
      if (!pc) valid = true;
    }
    // initial move
    if (row == 6 && r == 4 && c == col) {
      auto pc1 = bd[5][c];
      if (!pc1 && !pc) valid = true;
    }
    // capture
    if (r == row-1 && abs(col-c) == 1) {
      if (pc && pc->isWhite() != white) valid = true;
    }
    // en passant
    if (row == 3 && r == 2 && abs(col-c) == 1) {
      if (!pc) {
        auto pawn = bd[row][c];
        if (pawn && pawn->getType() == 'P' && pawn->isWhite() != white) {
          valid = true;
        }
      }
    }
  } else { // black
    // regular move
    if (r == row+1 && c == col) {
      if (!pc) valid = true;
    }
    // initial move
    if (row == 1 && r == 3 && c == col) {
      auto pc1 = bd[2][c];
      if (!pc1 && !pc) valid = true;
    }
    // capture
    if (r == row+1 && abs(col-c) == 1) {
      if (pc && pc->isWhite() != white) valid = true;
    }
    // en passant
    if (row == 4 && r == 5 && abs(col-c) == 1) {
      if (!pc) {
        auto pawn = bd[row][c];
        if (pawn && pawn->getType() == 'P' && pawn->isWhite() != white) {
          valid = true;
        }
      }
    }
  }
  return valid;
}

