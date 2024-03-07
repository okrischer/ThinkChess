#pragma once
#include <vector>

using namespace std;

// public interface for pieces
class Piece {
public:
  virtual ~Piece() {}
  virtual char getType() = 0;
  virtual int getValue() = 0;
  virtual bool isWhite() = 0;
  virtual int getRow() = 0;
  virtual int getCol() = 0;
  virtual void makeMove(int r, int c) = 0;
  virtual bool isValid(const vector<vector<Piece*>>& bd, int r, int c) = 0;
};


class King : public Piece {
public:
  ~King() {}
  King(bool w, int r, int c) :
    type{'K'}, white{w}, value{0}, row{r}, col{c} {}

  char getType() override { return type; }
  int getValue() override { return value; }
  bool isWhite() override { return white; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void makeMove(int r, int c) override { row = r; col = c; }
  bool isValid(const vector<vector<Piece*>>& bd, int r, int c) override;

private:
  char type;
  bool white;
  int value;
  int row;
  int col;
};


class Queen : public Piece {
public:
  ~Queen() {}
  Queen(bool w, int r, int c) :
    type{'Q'}, white{w}, value{900}, row{r}, col{c} {}

  char getType() override { return type; }
  int getValue() override { return value; }
  bool isWhite() override { return white; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void makeMove(int r, int c) override { row = r; col = c; }
  bool isValid(const vector<vector<Piece*>>& bd, int r, int c) override;

private:
  char type;
  bool white;
  int value;
  int row;
  int col;
};


class Rook : public Piece {
public:
  ~Rook() {}
  Rook(bool w, int r, int c) :
    type{'R'}, white{w}, value{500}, row{r}, col{c} {}

  char getType() override { return type; }
  int getValue() override { return value; }
  bool isWhite() override { return white; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void makeMove(int r, int c) override { row = r; col = c; }
  bool isValid(const vector<vector<Piece*>>& bd, int r, int c) override;

private:
  char type;
  bool white;
  int value;
  int row;
  int col;
};


class Bishop : public Piece {
public:
  ~Bishop() {}
  Bishop(bool w, int r, int c) :
    type{'B'}, white{w}, value{300}, row{r}, col{c} {}

  char getType() override { return type; }
  int getValue() override { return value; }
  bool isWhite() override { return white; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void makeMove(int r, int c) override { row = r; col = c; }
  bool isValid(const vector<vector<Piece*>>& bd, int r, int c) override;

private:
  char type;
  bool white;
  int value;
  int row;
  int col;
};


class Knight : public Piece {
public:
  ~Knight() {}
  Knight(bool w, int r, int c) :
    type{'N'}, white{w}, value{300}, row{r}, col{c} {}

  char getType() override { return type; }
  int getValue() override { return value; }
  bool isWhite() override { return white; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void makeMove(int r, int c) override { row = r; col = c; }
  bool isValid(const vector<vector<Piece*>>& bd, int r, int c) override;

private:
  char type;
  bool white;
  int value;
  int row;
  int col;
};


class Pawn : public Piece {
public:
  ~Pawn() {}
  Pawn(bool w, int r, int c) :
    type{'P'}, white{w}, value{100}, row{r}, col{c} {}

  char getType() override { return type; }
  int getValue() override { return value; }
  bool isWhite() override { return white; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void makeMove(int r, int c) override { row = r; col = c; }
  bool isValid(const vector<vector<Piece*>>& bd, int r, int c) override;

private:
  char type;
  bool white;
  int value;
  int row;
  int col;
};
