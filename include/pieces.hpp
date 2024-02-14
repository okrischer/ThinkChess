#pragma once
#include <vector>

// public interface for pieces
class Piece {
public:
  virtual ~Piece() {}
  virtual char getType() = 0;
  virtual char getValue() = 0;
  virtual bool isWhite() = 0;
  virtual bool isCaptured() = 0;
  virtual std::vector<std::vector<short>> validMoves(std::vector<std::vector<Piece*>>& bd) = 0;
};

class King : public Piece {
public:
  ~King() {}
  King(bool w, int r, int c) : type{'k'}, white{w}, value{0},
                              captured{false}, row{r}, col{c} {}

  char getType() override {
    return type;
  }

  char getValue() override {
    return value;
  }

  bool isWhite() override {
    return white;
  }

  bool isCaptured() override {
    return captured;
  }

  std::vector<std::vector<short>> validMoves(std::vector<std::vector<Piece*>>& bd) override {
    std::vector<std::vector<short>> moves(8, std::vector<short>(8, 0));
    for (int r = 0; r < 8; r++) {
      for (int c = 0; c < 8; c++) {
        if (abs(r-row) <= 1 && abs(c-col) <= 1) {
          auto pc = bd[r][c];
          if (!pc) moves[r][c] = 1;
          else if (pc->isWhite() != white) moves[r][c] = 2;
        }
      }
    }
    // todo: castling, need check and moves
    return moves;
  }

private:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;
};

class Rook : public Piece {
public:
  ~Rook() {}
  Rook(bool w, int r, int c) : type{'r'}, white{w}, value{4},
                             captured{false}, row{r}, col{c} {}

  char getType() override {
    return type;
  }

  char getValue() override {
    return value;
  }

  bool isWhite() override {
    return white;
  }

  bool isCaptured() override {
    return captured;
  }

  std::vector<std::vector<short>> validMoves(std::vector<std::vector<Piece*>>& bd) override {
    std::vector<std::vector<short>> moves(8, std::vector<short>(8, 0));
    for (int r = 0; r < 8; r++) {
      for (int c = 0; c < 8; c++) {
        // same row
        if (r == row) {
          bool valid = true;
          auto pc = bd[r][c];
          if (c < col) {
            for (int cc = c+1; cc < col; cc++) {
              if (bd[r][cc]) {
                valid = false;
                break;
              }
            }
            if (valid) {
              if (!pc) moves[r][c] = 1;
              else if (pc->isWhite() != white) moves[r][c] = 2;
            }
          } else if (c > col) {
            for (int cc = col+1; cc < c; cc++) {
              if (bd[r][cc]) {
                valid = false;
                break;
              }
            }
            if (valid) {
              if (!pc) moves[r][c] = 1;
              else if(pc->isWhite() != white) moves[r][c] = 2;
            }
          }
        }
        // same column
        else if (c == col) {
          bool valid = true;
          auto pc = bd[r][c];
          if (r < row) {
            for (int rr = r+1; rr < row; rr++) {
              if (bd[rr][c]) {
                valid = false;
                break;
              }
            }
            if (valid) {
              if (!pc) moves[r][c] = 1;
              else if(pc->isWhite() != white) moves[r][c] = 2;
            }
          } else if (r > row) {
            for (int rr = row+1; rr < r; rr++) {
              if (bd[rr][c]) {
                valid = false;
                break;
              }
            }
            if (valid) {
              if (!pc) moves[r][c] = 1;
              else if(pc->isWhite() != white) moves[r][c] = 2;
            }
          }
        }
      }
    }
    return moves;
  }

private:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;
};

class Bishop : public Piece {
public:
  ~Bishop() {}
  Bishop(bool w, int r, int c) : type{'b'}, white{w}, value{3},
                               captured{false}, row{r}, col{c} {}

  char getType() override {
    return type;
  }

  char getValue() override {
    return value;
  }

  bool isWhite() override {
    return white;
  }

  bool isCaptured() override {
    return captured;
  }

  std::vector<std::vector<short>> validMoves(std::vector<std::vector<Piece*>>& bd) override {
    std::vector<std::vector<short>> moves(8, std::vector<short>(8, 0));
    bool blocked = false;
    for (int r = row-1; r >= 0 && !blocked; r--) {  // upper half
      for (int c = 0; c < col; c++) {               // left half
        if (r-c == row-col) {
          auto pc = bd[r][c];
          if (!pc) moves[r][c] = 1;
          else {
            if (pc->isWhite() != white) moves[r][c] = 2;
            blocked = true;
          }
        }
      }
    }
    blocked = false;
    for (int r = row-1; r >= 0 && !blocked; r--) {  // upper half
      for (int c = col+1; c < 8; c++) {             // right half
        if (r+c == row+col) {
          auto pc = bd[r][c];
          if (!pc) moves[r][c] = 1;
          else {
            if (pc->isWhite() != white) moves[r][c] = 2;
            blocked = true;
          }
        }
      }
    }
    blocked = false;
    for (int r = row+1; r < 8 && !blocked; r++) {  // lower half
      for (int c = 0; c < col; c++) {              // left half
        if (r+c == row+col) {
          auto pc = bd[r][c];
          if (!pc) moves[r][c] = 1;
          else {
            if (pc->isWhite() != white) moves[r][c] = 2;
            blocked = true;
          }
        }
      }
    }
    blocked = false;
    for (int r = row+1; r < 8 && !blocked; r++) {  // lower half
      for (int c = col+1; c < 8; c++) {            // right half
        if (c-r == col-row) {
          auto pc = bd[r][c];
          if (!pc) moves[r][c] = 1;
          else {
            if (pc->isWhite() != white) moves[r][c] = 2;
            blocked = true;
          }
        }
      }
    }
    return moves;
  }

private:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;
};

class Queen : public Piece {
public:
  ~Queen() {}
  Queen(bool w, int r, int c) : type{'q'}, white{w}, value{8},
                              captured{false}, row{r}, col{c} {}

  char getType() override {
    return type;
  }

  char getValue() override {
    return value;
  }

  bool isWhite() override {
    return white;
  }

  bool isCaptured() override {
    return captured;
  }

  std::vector<std::vector<short>> validMoves(std::vector<std::vector<Piece*>>& bd) override {
    std::vector<std::vector<short>> moves(8, std::vector<short>(8, 0));
    // create rook and bishop on this position
    Piece* rook = new Rook(white, row, col);
    Piece* bishop = new Bishop(white, row, col);
    // get valid moves from rook and bishop
    auto movesRook = rook->validMoves(bd);
    auto movesBishop = bishop->validMoves(bd);
    // merge valid moves
    for (int r = 0; r < 8; r++) {
      for (int c = 0; c < 8; c++) {
        if (movesRook[r][c] > 0) moves[r][c] = movesRook[r][c];
        if (movesBishop[r][c] > 0) moves[r][c] = movesBishop[r][c];
      }
    }
    delete rook;
    delete bishop;
    return moves;
  }

private:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;
};

class Knight : public Piece {
public:
  ~Knight() {}
  Knight(bool w, int r, int c) : type{'n'}, white{w}, value{3},
                               captured{false}, row{r}, col{c} {}

  char getType() override {
    return type;
  }

  char getValue() override {
    return value;
  }

  bool isWhite() override {
    return white;
  }

  bool isCaptured() override {
    return captured;
  }

  std::vector<std::vector<short>> validMoves(std::vector<std::vector<Piece*>>& bd) override {
    std::vector<std::vector<short>> moves(8, std::vector<short>(8, 0));
    for (int r = 0; r < 8; r++) {
      for (int c = 0; c < 8; c++) {
        if ((abs(r-row) == 1 && abs(c-col) == 2) || (abs(r-row) == 2 && abs(c-col) == 1)) {
          auto pc = bd[r][c];
          if (!pc) moves[r][c] = 1;
          else if (pc->isWhite() != white) moves[r][c] = 2;
        }
      }
    }
    return moves;
  }

private:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;
};

class Pawn : public Piece {
public:
  ~Pawn() {}
  Pawn(bool w, int r, int c) : type{'p'}, white{w}, value{1},
                             captured{false}, row{r}, col{c} {}

  char getType() override {
    return type;
  }

  char getValue() override {
    return value;
  }

  bool isWhite() override {
    return white;
  }

  bool isCaptured() override {
    return captured;
  }

  std::vector<std::vector<short>> validMoves(std::vector<std::vector<Piece*>>& bd) override {
    std::vector<std::vector<short>> moves(8, std::vector<short>(8, 0));
    if (white) {
      // first move
      if (row == 6) {
        auto pc = bd[row-1][col];
        if (!pc) moves[row-1][col] = 1;
        auto pd = bd[row-2][col];
        if (!pc && !pd) moves[row-2][col] = 1;
      }
      // every move
      if (row > 0) {
        auto pc = bd[row-1][col];
        if (!pc) moves[row-1][col] = 1;
        if (col > 0) {
          pc = bd[row-1][col-1];
          if (pc && pc->isWhite() != white) moves[row-1][col-1] = 2;
        }
        if (col < 7) {
          pc = bd[row-1][col+1];
          if (pc && pc->isWhite() != white) moves[row-1][col+1] = 2;
        }
      }
      // todo: en passant, need last move
    } else { // black
      // first move
      if (row == 1) {
        auto pc = bd[row+1][col];
        if (!pc) moves[row+1][col] = 1;
        auto pd = bd[row+2][col];
        if (!pc && !pd) moves[row+2][col] = 1;
      }
      // every move
      if (row < 7) {
        auto pc = bd[row+1][col];
        if (!pc) moves[row+1][col] = 1;
        if (col > 0) {
          pc = bd[row+1][col-1];
          if (pc && pc->isWhite() != white) moves[row+1][col-1] = 2;
        }
        if (col < 7) {
          pc = bd[row+1][col+1];
          if (pc && pc->isWhite() != white) moves[row+1][col+1] = 2;
        }
      }
      // todo: en passant, need last move
    }
    return moves;
  }

private:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;
};
