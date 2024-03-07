#include "display.hpp"

using namespace std;

// calculates and returns the timer string
string getTime(unsigned t) {
  string result = "";
  unsigned h = 0;
  unsigned m = t / 60;
  unsigned s = t % 60;
  if (m >= 60) {
    h = m / 60;
    m = m % 60;
  }
  if (h < 10) {
    result.append(1, '0');
    result.append(to_string(h));
  } else {
    result.append(to_string(h));
  }
  result.append(1, ':');
  if (m < 10) {
    result.append(1, '0');
    result.append(to_string(m));
  } else {
    result.append(to_string(m));
  }
  result.append(1, ':');
  if (s < 10) {
    result.append(1, '0');
    result.append(to_string(s));
  } else {
    result.append(to_string(s));
  }
  return result;
}

// set valid moves for display
void setValidMoves(const Position& pos,
                   vector<vector<short>>& vm,
                   pair<int, int> field)
{
  auto pc = pos.board[field.first][field.second];
  if (!pc) return;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (pc->isValid(pos.board, row, col)) {
        auto current = pos.board[row][col];
        if (!current) vm[row][col] = 1;
        else if (pc->isWhite() != current->isWhite()) vm[row][col] = 2;
      }
    }
  }
}

// reset board for new game
void resetBoard(Position& pos) {
  // reset moves and captured pieces
  pos.moves.clear();
  pos.captured.clear();
  // reset board
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      pos.board[row][col] = nullptr;
    }
  }
  // set board to initial position
  // rank 8 (black)
  pos.board[0][0] = new Rook(0,0,0);
  pos.board[0][1] = new Knight(0,0,1);
  pos.board[0][2] = new Bishop(0,0,2);
  pos.board[0][3] = new Queen(0,0,3);
  pos.board[0][4] = new King(0,0,4);
  pos.board[0][5] = new Bishop(0,0,5);
  pos.board[0][6] = new Knight(0,0,6);
  pos.board[0][7] = new Rook(0,0,7);
  // rank 7 (black)
  pos.board[1][0] = new Pawn(0,1,0);
  pos.board[1][1] = new Pawn(0,1,1);
  pos.board[1][2] = new Pawn(0,1,2);
  pos.board[1][3] = new Pawn(0,1,3);
  pos.board[1][4] = new Pawn(0,1,4);
  pos.board[1][5] = new Pawn(0,1,5);
  pos.board[1][6] = new Pawn(0,1,6);
  pos.board[1][7] = new Pawn(0,1,7);
  // rank 2 (white)
  pos.board[6][0] = new Pawn(1,6,0);
  pos.board[6][1] = new Pawn(1,6,1);
  pos.board[6][2] = new Pawn(1,6,2);
  pos.board[6][3] = new Pawn(1,6,3);
  pos.board[6][4] = new Pawn(1,6,4);
  pos.board[6][5] = new Pawn(1,6,5);
  pos.board[6][6] = new Pawn(1,6,6);
  pos.board[6][7] = new Pawn(1,6,7);
  // rank 1 (white)
  pos.board[7][0] = new Rook(1,7,0);
  pos.board[7][1] = new Knight(1,7,1);
  pos.board[7][2] = new Bishop(1,7,2);
  pos.board[7][3] = new Queen(1,7,3);
  pos.board[7][4] = new King(1,7,4);
  pos.board[7][5] = new Bishop(1,7,5);
  pos.board[7][6] = new Knight(1,7,6);
  pos.board[7][7] = new Rook(1,7,7);
}
