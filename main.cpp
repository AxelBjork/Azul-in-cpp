// Azul game

#include <algorithm>
#include <ctime>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <vector>

using namespace std;

// Tile class
class Tile {
public:
  Tile(const string &color) : color(color) {}

  string getColor() const { return color; }

private:
  string color;
};

// Helper functions
map<string, int> getTileCounts(const vector<Tile> &tiles) {
  map<string, int> colorCounts;
  const vector<string> colors = {"Black",  "Red",   "Blue",
                                 "Yellow", "Green", "First Player"};

  for (const string &color : colors) {
    colorCounts[color] = 0;
  }

  for (const Tile &tile : tiles) {
    colorCounts[tile.getColor()]++;
  }

  return colorCounts;
}

void printTileCounts(const vector<Tile> &tiles, const string &containerName) {
  map<string, int> colorCounts = getTileCounts(tiles);

  cout << containerName << " contents: [ ";
  // All colors should be printed in one line
  for (const auto &colorCount : colorCounts) {
    if (colorCount.second == 0) {
      continue;
    }
    cout << colorCount.first << ": " << colorCount.second << " ";
  }
  cout << "]" << endl;
}

// TileBag class
class TileBag {
public:
  TileBag() {
    initTiles();
    srand(time(0));
  }

  Tile drawRandomTile() {
    if (tiles.empty()) {
      throw runtime_error("No tiles left in the bag");
    }

    int randomIndex = rand() % tiles.size();
    Tile randomTile = tiles[randomIndex];
    tiles.erase(tiles.begin() + randomIndex);
    return randomTile;
  }

  void printContents() const { printTileCounts(tiles, "Tile Bag"); }

private:
  vector<Tile> tiles;

  void initTiles() {
    const vector<string> colors = {"Black", "Red", "Blue", "Yellow", "Green"};
    int tilesPerColor = 20;

    for (const string &color : colors) {
      for (int i = 0; i < tilesPerColor; ++i) {
        tiles.emplace_back(color);
      }
    }
  }
};

// CentralMarket class
class CentralMarket {
public:
  // center_taken 1 penalty point for first player
  bool center_taken = false;
  void addTile(const Tile &tile) { tiles.push_back(tile); }

  vector<Tile> takeTilesOfColor(const string &color) {
    vector<Tile> takenTiles;

    for (auto it = tiles.begin(); it != tiles.end();) {
      if (it->getColor() == color) {
        takenTiles.push_back(*it);
        it = tiles.erase(it);
      } else {
        ++it;
      }
    }
    if (center_taken == false) {
      Tile penalty_tile = tiles[5];
      center_taken = true;
      takenTiles.push_back(penalty_tile);
    }

    return takenTiles;
  }

  bool isEmpty() const { return tiles.empty(); }
  map<string, int> getContents() const { return getTileCounts(tiles); }
  void printContents() const { printTileCounts(tiles, "Central Market"); }

  vector<Tile> tiles;
};

// FactoryDisplay class
class FactoryDisplay {
public:
  FactoryDisplay(TileBag &tileBag, int display_id) : display_id(display_id) {
    refill(tileBag);
  }
  void refill(TileBag &tileBag) {
    tiles.clear();
    for (int i = 0; i < 4; ++i) {
      tiles.push_back(tileBag.drawRandomTile());
    }
  }

  pair<vector<Tile>, vector<Tile>> takeTilesOfColor(const string &color) {
    vector<Tile> takenTiles;
    vector<Tile> discardedTiles;

    for (auto it = tiles.begin(); it != tiles.end();) {
      if (it->getColor() == color) {
        takenTiles.push_back(*it);
        it = tiles.erase(it);
      } else {
        discardedTiles.push_back(*it);
        it = tiles.erase(it);
      }
    }

    return make_pair(takenTiles, discardedTiles);
  }

  bool isEmpty() const { return tiles.empty(); }
  map<string, int> getContents() const { return getTileCounts(tiles); }
  void printContents() const {
    if (tiles.empty()) {
      cout << "Factory Display " << to_string(display_id) << " is empty"
           << endl;
      return;
    }
    printTileCounts(tiles, "Factory Display " + to_string(display_id));
  }

  vector<Tile> tiles;

private:
  int display_id;
};

// Factory class
class Factory {
public:
  Factory(int numFactoryDisplays) : centralMarket() {
    tileBag = make_shared<TileBag>();
    for (int i = 0; i < numFactoryDisplays; ++i) {
      factoryDisplays.push_back(make_shared<FactoryDisplay>(*tileBag, i));
    }
  }

  void refillDisplays() {
    for (const auto &display : factoryDisplays) {
      if (display->isEmpty()) {
        display->refill(*tileBag);
      }
    }
  }

  vector<Tile> takeDisplayTiles(int displayId, const string &color) {
    shared_ptr<FactoryDisplay> display = factoryDisplays[displayId];

    if (!any_of(
            display->tiles.begin(), display->tiles.end(),
            [&color](const Tile &tile) { return tile.getColor() == color; })) {
      return vector<Tile>();
    } else {
      auto [takenTiles, discardedTiles] = display->takeTilesOfColor(color);
      for (const Tile &tile : discardedTiles) {
        centralMarket.addTile(tile);
      }
      return takenTiles;
    }
  }

  vector<Tile> takeCenterTiles(const string &color) {
    vector<Tile> takenTiles = centralMarket.takeTilesOfColor(color);
    return takenTiles;
  }

  map<string, int> getContents(int displayId = -1) const {
    if (displayId == -1) {
      return centralMarket.getContents();
    } else {
      return factoryDisplays[displayId]->getContents();
    }
  }

  void printContents() const {
    tileBag->printContents();
    centralMarket.printContents();
    for (const auto &display : factoryDisplays) {
      display->printContents();
    }
  }

  // Add other methods as needed to interact with FactoryDisplay and
  vector<shared_ptr<FactoryDisplay>> factoryDisplays;
  CentralMarket centralMarket;

private:
  shared_ptr<TileBag> tileBag;
};

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