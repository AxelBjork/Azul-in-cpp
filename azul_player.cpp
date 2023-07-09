#include "azul_player.h"
#include "azul_game.h"

// PlayerBoard class
PlayerBoard::PlayerBoard() {
  // Initialize pattern lines
  for (int i = 0; i < numPatternLines; ++i) {
    patternLines.emplace_back(vector<Tile>(i + 1));
  }
  for (int i = 0; i < numPatternLines; ++i) {
    wall.emplace_back(vector<int>(numPatternLines));
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

  auto it = patternLines[row].begin();
  auto tileIt = tiles.begin();

  // Add tiles to the pattern line until full
  while (tileIt != tiles.end() && it != patternLines[row].end()) {
    if ((*it).getColor().empty()) {
      *it = *tileIt;
      ++tileIt;
    }
    ++it;
  }

  // Add remaining tiles to the floor line
  if (tileIt != tiles.end()) {
    addTilesToFloorLine(vector<Tile>(tileIt, tiles.end()));
  }
  cout << "Added " << to_string(tiles.size()) << " tiles to row " << row
       << endl;
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
      // Find the right column based on color
      for (int j = 0; j < 5; ++j) {
        if (wallColors[i][j] == score_tile.getColor()) {
          if (wall[i][j] == 1) {
            // If the tile is already on the wall, move it to the floor line
            addTilesToFloorLine(vector<Tile>(1, score_tile));
            cout << "Tile already on the wall, moving to floor line" << endl;
            break;
          }
          wall[i][j] = 1;
          scoringSystem.scorePlacedTile(i, j, wall);
          break;
        }
      }
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
  printTileCounts(floorLine, "Floor Line");
}

void PlayerBoard::printWall() const {
  cout << "Player Wall" << endl;
  for (int row = 0; row < numPatternLines; ++row) {
    for (int col = 0; col < numPatternLines; ++col) {
      cout << wall[row][col] << " ";
    }
    cout << endl;
  }
}

bool PlayerBoard::isPatternLineFull(int row) const {
  return all_of(patternLines[row].begin(), patternLines[row].end(),
                [](const Tile &tile) { return !tile.getColor().empty(); });
}

// ScoringSystem class
void ScoringSystem::scorePlacedTile(int row, int col,
                                    const vector<vector<int>> &wall) {
  int points = -1;

  // Check horizontally
  int horizontalTiles = 1;
  for (int i = col - 1; i >= 0 && wall[row][i] == 1; --i) {
    ++horizontalTiles;
  }
  for (int i = col + 1; i < 5 && wall[row][i] == 1; ++i) {
    ++horizontalTiles;
  }

  // Check vertically
  int verticalTiles = 1;
  for (int i = row - 1; i >= 0 && wall[i][col] == 1; --i) {
    ++verticalTiles;
  }
  for (int i = row + 1; i < 5 && wall[i][col] == 1; ++i) {
    ++verticalTiles;
  }

  // Add points (always horizontal points)
  points += horizontalTiles;
  points += verticalTiles;
  cout << "Placed tile at (" << row << ", " << col << ") for " << points
       << " points" << endl;
  score += points;
}

void ScoringSystem::scoreFloorLine(const vector<Tile> &floorLine) {
  const vector<int> penalties = {-1, -1, -2, -2, -2, -3, -3};

  int penaltyPoints = 0;
  for (size_t i = 0; i < floorLine.size() && i < penalties.size(); ++i) {
    penaltyPoints += penalties[i];
  }
  cout << "Floor line penalty: " << penaltyPoints << endl;
  score += penaltyPoints;
}

int ScoringSystem::getScore() const { return score; }
