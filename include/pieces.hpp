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
  virtual bool isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) = 0;
};


class King : public Piece {
public:
  ~King() {}
  King(bool w, int r, int c) : type{'k'}, white{w}, value{0},
                              captured{false}, row{r}, col{c} {}

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  bool isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) override;

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

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  bool isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) override;

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

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  bool isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) override;

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

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  bool isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) override;

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

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  bool isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) override;

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

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  bool isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) override;

private:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;
};
