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
  void printContents() const;
  void printWall() const;

private:
  bool isPatternLineFull(int row) const;
  int numPatternLines = 5;

  vector<vector<Tile>> patternLines;
  vector<Tile> floorLine;
  vector<vector<int>> wall;
  const vector<vector<string>> wallColors = {
      {"Blue", "Yellow", "Red", "Black", "Green"},
      {"Green", "Blue", "Yellow", "Red", "Black"},
      {"Black", "Green", "Blue", "Yellow", "Red"},
      {"Red", "Black", "Green", "Blue", "Yellow"},
      {"Yellow", "Red", "Black", "Green", "Blue"}};
};

class ScoringSystem {
public:
  void scorePlacedTile(int row, int col, const vector<vector<int>> &wall);
  void scoreFloorLine(const vector<Tile> &floorLine);
  int getScore() const;

private:
  int score = 0;
};

#endif // AZUL_PLAYER_H
