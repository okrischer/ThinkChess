#pragma once

#include "pieces.hpp"
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
void setValidMoves(const vector<vector<Piece*>>& bd,
                   vector<vector<short>>& vm,
                   Piece* pc);


// make a move
bool makeMove(vector<vector<Piece*>>& bd,
              vector<string>& mv,
              vector<Piece*>& cp,
              pair<int, int>& td,
              pair<int, int> to,
              bool& player,
              pair<int, int>& checkmate,
              short& castled);


// test for check
bool check(const vector<vector<Piece*>>& bd, Piece* pc);

// check wether a given check can be resolved
bool resolveCheck(vector<vector<Piece*>>& bd, bool player);

// test wether castling is possible
char castling(vector<vector<Piece*>>& bd, Piece* king, pair<int, int> to);

// reset board for new game
void resetBoard(vector<vector<Piece*>>& bd,
                vector<string>& mv,
                vector<Piece*>& cp);


// print board for debug
void printBoard(const vector<vector<Piece*>>& bd);
