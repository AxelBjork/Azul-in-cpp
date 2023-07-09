#include "azul_player.h"
#include "azul_game.h"

// PlayerBoard class
PlayerBoard::PlayerBoard() {
  // Initialize pattern lines
  for (int i = 0; i < numPatternLines; ++i) {
    patternLines.emplace_back(vector<Tile>(i + 1));
  }
  for (int i = 0; i < numPatternLines; ++i) {
    wall.emplace_back(vector<Tile>(numPatternLines));
  }
}

bool PlayerBoard::addTilesToPatternLine(int row, const vector<Tile> &tiles) {
  if (row < 0 || row >= 5) {
    return false;
  }

  // Check if the row is empty or contains the same color
  const Tile &firstTile = patternLines[row][0];
  if (!firstTile.getColor().empty() &&
      firstTile.getColor() != tiles[0].getColor()) {
    return false;
  }

  // Check if there is enough space in the row
  int emptySpaces =
      count_if(patternLines[row].begin(), patternLines[row].end(),
               [](const Tile &tile) { return tile.getColor().empty(); });
  if (tiles.size() > emptySpaces) {
    return false;
  }

  // Add tiles to the pattern line
  for (const Tile &tile : tiles) {
    auto it = find_if(patternLines[row].begin(), patternLines[row].end(),
                      [](const Tile &tile) { return tile.getColor().empty(); });
    *it = tile;
  }

  return true;
}

void PlayerBoard::addTilesToFloorLine(const vector<Tile> &tiles) {
  floorLine.insert(floorLine.end(), tiles.begin(), tiles.end());
}

void PlayerBoard::endOfRound(ScoringSystem &scoringSystem) {
  // Move completed pattern lines to the wall
  for (int i = 0; i < 5; ++i) {
    if (isPatternLineFull(i)) {
      Tile score_tile = patternLines[i][0];
      wall[i].push_back(score_tile);
      scoringSystem.scorePlacedTile(i, wall[i].size() - 1, wall);
      patternLines[i] = vector<Tile>(i + 1);
    }
  }

  // Clear the floor line and score negative points
  scoringSystem.scoreFloorLine(floorLine);
  floorLine.clear();
}

void PlayerBoard::printContents() const {
  for (int row = 0; row < numPatternLines; ++row) {
    if (patternLines[row].empty()) {
      cout << "Board Row " << to_string(row) << " is empty" << endl;
      return;
    }
    printTileCounts(patternLines[row], "Board Row " + to_string(row));
  }
}

bool PlayerBoard::isPatternLineFull(int row) const {
  return all_of(patternLines[row].begin(), patternLines[row].end(),
                [](const Tile &tile) { return !tile.getColor().empty(); });
}

// ScoringSystem class
void ScoringSystem::scorePlacedTile(int row, int col,
                                    const vector<vector<Tile>> &wall) {
  int points = 0;
  string tileColor = wall[row][col].getColor();

  // Check horizontally
  int horizontalTiles = 1;
  for (int i = col - 1; i >= 0 && wall[row][i].getColor() == tileColor; --i) {
    ++horizontalTiles;
  }
  for (int i = col + 1; i < 5 && wall[row][i].getColor() == tileColor; ++i) {
    ++horizontalTiles;
  }

  // Check vertically
  int verticalTiles = 1;
  for (int i = row - 1; i >= 0 && wall[i][col].getColor() == tileColor; --i) {
    ++verticalTiles;
  }
  for (int i = row + 1; i < 5 && wall[i][col].getColor() == tileColor; ++i) {
    ++verticalTiles;
  }

  if (horizontalTiles > 1) {
    points += horizontalTiles;
  }
  if (verticalTiles > 1) {
    points += verticalTiles;
  }

  score += points;
}

void ScoringSystem::scoreFloorLine(const vector<Tile> &floorLine) {
  const vector<int> penalties = {-1, -1, -2, -2, -2, -3, -3};

  int penaltyPoints = 0;
  for (size_t i = 0; i < floorLine.size() && i < penalties.size(); ++i) {
    penaltyPoints += penalties[i];
  }

  score += penaltyPoints;
}

int ScoringSystem::getScore() const { return score; }