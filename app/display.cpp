#include "display.hpp"
#include <utility>

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

// returns a material evaluation for both players
pair<float, float> evaluate(const vector<vector<Piece*>>& bd) {
  float white = 0.f;
  float black = 0.f;
  
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      auto pc = bd[row][col];
      if (pc) {
        pc->isWhite() ? white += pc->getValue() : black += pc->getValue();
      }
    }
  }

  pair<float, float> eval = make_pair(white, black);
  return eval;
}
