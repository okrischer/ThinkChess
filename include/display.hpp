#pragma once

#include "position.hpp"
#include <string>
#include <filesystem>


// get path for game to load
std::filesystem::path getGame(int fn, std::filesystem::path games);

// reset board for new game
void resetBoard(Position& pos);

// calculates and returns the timer string
std::string getTime(unsigned t);

// set valid moves for display
void setValidMoves(const Position& pos,
                   vector<vector<short>>& vm,
                   pair<int, int> field);
