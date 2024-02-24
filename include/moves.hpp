#pragma once

#include "pieces.hpp"
#include "list.hpp"
#include <utility>
#include <vector>
#include <string>

using namespace std;

// convert columns to files
char colToFile(int col);

// convert rows to ranks
char rowToRank(int row);

// get board coordinates
pair<int, int> getField(int x, int y);

// convert board coordinates to moves notation
string convertFromBoard(bool cap, Piece* from, pair<int, int> to);

// set valid moves for display
void setValidMoves(vector<vector<Piece*>>& bd,
                   vector<vector<short>>& vm,
                   Piece* pc);

// make a move
void makeMove(vector<vector<Piece*>>& bd,
              list::List<string>* mv,
              list::List<Piece*>* cp,
              pair<int, int>& td,
              pair<int, int> to,
              bool& player,
              pair<int, int>& checkmate);

// test for check
bool check(vector<vector<Piece*>>& bd, Piece* pc);

// test wether a given check can be resolved
bool resolveCheck(vector<vector<Piece*>>& bd, bool white);

// get opponents king coordinates
pair<int, int> getKing(vector<vector<Piece*>>& bd, bool white);

// reset board for new game
void resetBoard(vector<vector<Piece*>>& bd,
                 list::List<string>* mv,
                 list::List<Piece*>* cp);