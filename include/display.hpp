#pragma once

#include "pieces.hpp"
#include <string>
#include <utility>
#include <vector>

using namespace std;

// calculates and returns the timer string
string getTime(unsigned t);

// returns a material evaluation for both players
pair<short, short> evaluate(const vector<vector<Piece*>>& bd);
