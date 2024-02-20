#include "pieces.hpp"
#include <vector>
using namespace std;

bool King::isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) {
  bool valid = abs(r-row) <= 1 && abs(c-col) <= 1;
  return valid;
}

bool Knight::isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) {
  bool valid = (abs(r-row) == 1 && abs(c-col) == 2) ||
               (abs(r-row) == 2 && abs(c-col) == 1);
  return valid;
}

bool Rook::isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) {
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

bool Bishop::isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) {
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

bool Queen::isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) {
  auto rook = new Rook(white, row, col);
  auto bishop = new Bishop(white, row, col);
  bool valid = rook->isValid(bd, r, c) || bishop->isValid(bd, r, c);
  delete rook;
  delete bishop;
  return valid;
}

bool Pawn::isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) {
  bool valid = false;
  if (white) {
    if (r == row-1 && c == col) { // move
      auto pc = bd[r][c];
      if (!pc) valid = true;
    }
    if (row == 6 && r == 4 && c == col) { // initial move
      auto pc = bd[r][c];
      if (!pc) valid = true;
    }
    if (r == row-1 && (c == col-1 || c == col+1)) { // capture
      auto pc = bd[r][c];
      if (pc && pc->isWhite() != white) valid = true;
    }
  } else { // black
    if (r == row+1 && c == col) { // move
      auto pc = bd[r][c];
      if (!pc) valid = true;
    }
    if (row == 1 && r == 3 && c == col) { // initial move
      auto pc = bd[r][c];
      if (!pc) valid = true;
    }
    if (r == row+1 && (c == col-1 || c == col+1)) { // capture
      auto pc = bd[r][c];
      if (pc && pc->isWhite() != white) valid = true;
    }
  }
  return valid;
}

