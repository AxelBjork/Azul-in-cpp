// Azul game

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

map<string, int> getTileCounts(const vector<Tile> &tiles) {
  map<string, int> colorCounts;
  const vector<string> colors = {"Black", "Red", "Blue", "Yellow", "Green"};

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

  cout << containerName << " contents:" << endl;
  for (const auto &colorCount : colorCounts) {
    cout << colorCount.first << ": " << colorCount.second << endl;
  }
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

    return takenTiles;
  }

  bool isEmpty() const { return tiles.empty(); }

  void printContents() const { printTileCounts(tiles, "Central Market"); }

private:
  vector<Tile> tiles;
};

// FactoryDisplay class
class FactoryDisplay {
public:
  FactoryDisplay(TileBag &tileBag) { refill(tileBag); }

  void refill(TileBag &tileBag) {
    tiles.clear();
    for (int i = 0; i < 4; ++i) {
      tiles.push_back(tileBag.drawRandomTile());
    }
  }

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

    return takenTiles;
  }

  bool isEmpty() const { return tiles.empty(); }

  void printContents() const { printTileCounts(tiles, "Factory Display"); }

private:
  vector<Tile> tiles;
};

// Factory class
class Factory {
public:
  Factory(int numFactoryDisplays) : centralMarket() {
    tileBag = make_shared<TileBag>();
    for (int i = 0; i < numFactoryDisplays; ++i) {
      factoryDisplays.push_back(make_shared<FactoryDisplay>(*tileBag));
    }
  }

  void refillDisplays() {
    for (const auto &display : factoryDisplays) {
      if (display->isEmpty()) {
        display->refill(*tileBag);
      }
    }
  }

  void transferTilesToCentralMarket() {
    for (const auto &display : factoryDisplays) {
      for (const Tile &tile : display->takeTilesOfColor("")) {
        centralMarket.addTile(tile);
      }
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
  // CentralMarket

private:
  shared_ptr<TileBag> tileBag;
  vector<shared_ptr<FactoryDisplay>> factoryDisplays;
  CentralMarket centralMarket;
};

int main() {
  int numFactoryDisplays = 3;
  Factory factory(numFactoryDisplays);

  // Refill factory displays
  factory.refillDisplays();

  // Print contents of factory
  factory.printContents();

  // Perform other game logic, such as taking tiles from displays or central
  // market

  return 0;
}
