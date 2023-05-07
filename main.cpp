// Azul game
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "azul_game.h"

using namespace std;

int main() {
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

  return 0;
}