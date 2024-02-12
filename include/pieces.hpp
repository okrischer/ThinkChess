// public interface for pieces
class Piece {
public:
  virtual ~Piece() {}
  virtual char get_type() = 0;
  virtual bool isWhite() = 0;
  virtual void move(int r, int c) = 0;
};

class King : public Piece {
public:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;

  King(bool w, int r, int c) : type{'k'}, white{w}, value{0},
                              captured{false}, row{r}, col{c} {}

  char get_type() override {
    return type;
  }

  bool isWhite() override {
    return white;
  }

  void move(int r, int c) override {
  }
};

class Queen : public Piece {
public:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;

  Queen(bool w, int r, int c) : type{'q'}, white{w}, value{8},
                              captured{false}, row{r}, col{c} {}

  char get_type() override {
    return type;
  }

  bool isWhite() override {
    return white;
  }

  void move(int r, int c) override {
  }
};

class Rook : public Piece {
public:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;

  Rook(bool w, int r, int c) : type{'r'}, white{w}, value{4},
                             captured{false}, row{r}, col{c} {}

  char get_type() override {
    return type;
  }

  bool isWhite() override {
    return white;
  }

  void move(int r, int c) override {
  }
};

class Knight : public Piece {
public:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;

  Knight(bool w, int r, int c) : type{'n'}, white{w}, value{3},
                               captured{false}, row{r}, col{c} {}

  char get_type() override {
    return type;
  }

  bool isWhite() override {
    return white;
  }

  void move(int r, int c) override {
  }
};

class Bishop : public Piece {
public:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;

  Bishop(bool w, int r, int c) : type{'b'}, white{w}, value{3},
                               captured{false}, row{r}, col{c} {}

  char get_type() override {
    return type;
  }

  bool isWhite() override {
    return white;
  }

  void move(int r, int c) override {
  }
};

class Pawn : public Piece {
public:
  char type;
  bool white;
  short value;
  bool captured;
  int row;
  int col;

  Pawn(bool w, int r, int c) : type{'p'}, white{w}, value{1},
                             captured{false}, row{r}, col{c} {}

  char get_type() override {
    return type;
  }

  bool isWhite() override {
    return white;
  }

  void move(int r, int c) override {
  }
};
