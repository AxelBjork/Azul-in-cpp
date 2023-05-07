#ifndef AZUL_PLAYER_H
#define AZUL_PLAYER_H

#include "azul_game.h"
#include <algorithm>
#include <string>
#include <vector>


using std::string;
using std::vector;

class PlayerBoard {
public:
  PlayerBoard();

  bool addTilesToPatternLine(int row, const vector<Tile> &tiles);
  void addTilesToFloorLine(const vector<Tile> &tiles);
  void endOfRound(class ScoringSystem &scoringSystem);

private:
  bool isPatternLineFull(int row) const;

  vector<vector<Tile>> patternLines;
  vector<Tile> floorLine;
  vector<vector<Tile>> wall;
};

class ScoringSystem {
public:
  void scorePlacedTile(int row, int col, const vector<vector<Tile>> &wall);
  void scoreFloorLine(const vector<Tile> &floorLine);
  int getScore() const;

private:
  int score = 0;
};

#endif // AZUL_PLAYER_H
