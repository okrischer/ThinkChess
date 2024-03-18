#include "position.hpp"
#include <cctype>
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

// convert files to colums
int fileToCol(char file) {
  int col = file - 97;
  return col;
}

// convert ranks to rows
int rankToRow(char rank) {
  int row = 56 - rank;
  return row;
}

// parse move-string and return coordinates
vector<int> parseMove(string move) {
  vector<int> coords(5, 0);
  if (isupper(move[0])) move.erase(0, 1);
  for (int i = 0; i < 5; i++) {
    if (i == 0 || i == 3) { // file
      coords[i] = fileToCol(move[i]);
    } else if (i == 1 || i == 4) { // rank
      coords[i] = rankToRow(move[i]);
    } else { // captured?
      move[i] == 'x' ? coords[i] = 1 : coords[i] = 0;
    }
  }
  return coords;
}

// test for check
bool check(const vector<vector<Piece*>>& bd, bool white) {
  Piece* king = nullptr;
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

// check wether a given check can be resolved
bool resolveCheck(vector<vector<Piece*>>& bd, bool player) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      auto current = bd[row][col];
      // get every piece of given color
      if (current && current->isWhite() == player) {
        for (int rr = 0; rr < 8; rr++) {
          for (int cc = 0; cc < 8; cc++) {
            if (current->isValid(bd, rr, cc)) {
              auto pct = bd[rr][cc];
              if (!pct || pct->isWhite() != player) {
                // make move and test for check
                bd[rr][cc] = current;
                current->makeMove(rr, cc);
                bd[row][col] = nullptr;
                if (!check(bd, player)) {
                  // reset move
                  bd[rr][cc] = pct;
                  bd[row][col] = current;
                  current->makeMove(row, col);
                  return true;
                } else {
                  // reset move
                  bd[rr][cc] = pct;
                  bd[row][col] = current;
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

// evaluate board
pair<int, int> evaluateBoard(const vector<vector<Piece*>>& bd) {
  int white = 0;
  int black = 0;
  
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      auto pc = bd[row][col];
      if (pc) {
        pc->isWhite() ? white += pc->getValue() : black += pc->getValue();
      }
    }
  }
  pair<int, int> eval = make_pair(white, black);
  return eval;
}

// print board for debug
void printBoard(const vector<vector<Piece*>>& bd) {
  cout << "\n";
  for (int row = 0; row < 8; row++) {
    string rank = "";
    for (int col = 0; col < 8; col++) {
      auto pc = bd[row][col];
      if (pc) {
        rank.append(1, pc->getType());
        rank.append(1, ' ');
      } else rank.append(". ");
    }
    cout << rank << "\n";
  }
  cout << "\n";
}

