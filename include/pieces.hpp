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
  virtual int getRow() = 0;
  virtual int getCol() = 0;
  virtual void capture() = 0;
  virtual void makeMove(int r, int c) = 0;
  virtual bool isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) = 0;
};


class King : public Piece {
public:
  ~King() {}
  King(bool w, int r, int c) : type{'K'}, white{w}, value{0},
                              captured{false}, row{r}, col{c} {}

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void capture() override { captured = true; }
  void makeMove(int r, int c) override { row = r; col = c; }
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
  Queen(bool w, int r, int c) : type{'Q'}, white{w}, value{8},
                              captured{false}, row{r}, col{c} {}

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void capture() override { captured = true; }
  void makeMove(int r, int c) override { row = r; col = c; }
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
  Rook(bool w, int r, int c) : type{'R'}, white{w}, value{4},
                             captured{false}, row{r}, col{c} {}

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void capture() override { captured = true; }
  void makeMove(int r, int c) override { row = r; col = c; }
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
  Bishop(bool w, int r, int c) : type{'B'}, white{w}, value{3},
                               captured{false}, row{r}, col{c} {}

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void capture() override { captured = true; }
  void makeMove(int r, int c) override { row = r; col = c; }
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
  Knight(bool w, int r, int c) : type{'N'}, white{w}, value{3},
                               captured{false}, row{r}, col{c} {}

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void capture() override { captured = true; }
  void makeMove(int r, int c) override { row = r; col = c; }
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
  Pawn(bool w, int r, int c) : type{'P'}, white{w}, value{1},
                             captured{false}, row{r}, col{c} {}

  char getType() override { return type; }
  char getValue() override { return value; }
  bool isWhite() override { return white; }
  bool isCaptured() override { return captured; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void capture() override { captured = true; }
  void makeMove(int r, int c) override { row = r; col = c; }
  bool isValid(std::vector<std::vector<Piece*>>& bd, int r, int c) override;

private:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;
};
