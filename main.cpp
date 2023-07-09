// Azul game
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "azul_game.h"
#include "azul_player.h"

using namespace std;

void factoryDemo() {
  // Factory demo
  int numFactoryDisplays = 5;
  Factory factory(numFactoryDisplays);
  factory.printContents();
  // Refill and empty
  for (int round = 0; round < 5; ++round) {
    // Take all the tiles
    for (int id = 0; id < numFactoryDisplays; ++id) {
      map<string, int> displayContents = factory.getContents(id);
      if (!displayContents.empty()) {
        string topColor;
        int maxTiles = 0;
        for (const auto &colorCount : displayContents) {
          if (colorCount.second > maxTiles) {
            maxTiles = colorCount.second;
            topColor = colorCount.first;
          }
        }
        vector<Tile> myTiles = factory.takeDisplayTiles(id, topColor);
      }
    }
    factory.printContents();
    const vector<string> colors = {"Black", "Red", "Blue", "Yellow", "Green"};
    for (const string &color : colors) {
      vector<Tile> myTiles = factory.takeCenterTiles(color);
    }

    factory.printContents();
    factory.refillDisplays();
    factory.printContents();
  }
}

void playerDemo() {
  const vector<string> colors = {"Blue", "Yellow", "Red", "Black", "Green"};
  PlayerBoard player_board;
  ScoringSystem score_board;

  // Create 1-5 tiles of each color in each round, fill a selected line with
  // these tiles
  for (int round = 0; round < 3; ++round) {
    for (int step = 1; step <= 5; ++step) {
      // Create tiles
      string color = colors[(round + step - 1) % 5];
      Tile test_tile(color);
      vector<Tile> takenTiles;
      for (int i = 0; i < step; ++i) {
        takenTiles.push_back(test_tile);
      }

      // Add tiles to a pattern line
      player_board.addTilesToPatternLine(step - 1, takenTiles);
    }

    // Print the contents of the player board
    player_board.printContents();
    // End of round
    player_board.endOfRound(score_board);
    cout << "End of round " << round
         << ", current score: " << score_board.getScore() << endl
         << endl;
  }
  player_board.printWall();
}

int main() {
  if (0) {
    // Factory demo
    factoryDemo();
  }
  if (1) {
    // Factory demo
    playerDemo();
  }

  return 0;
}
