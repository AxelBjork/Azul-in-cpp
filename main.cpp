// Azul game

#include <ctime>
#include <iostream>
#include <map>
#include <random>
#include <vector>

// Tile class
class Tile {
public:
  Tile(const std::string &color) : color(color) {}

  std::string getColor() const { return color; }

private:
  std::string color;
};

std::map<std::string, int> printTileCounts(const std::vector<Tile> &tiles,
                                           const std::string &containerName) {
  std::map<std::string, int> colorCounts;
  const std::vector<std::string> colors = {"Black", "Red", "Blue", "White",
                                           "Green"};

  for (const std::string &color : colors) {
    colorCounts[color] = 0;
  }

  for (const Tile &tile : tiles) {
    colorCounts[tile.getColor()]++;
  }
  std::cout << containerName << " contents:" << std::endl;
  for (const auto &colorCount : colorCounts) {
    std::cout << colorCount.first << ": " << colorCount.second << std::endl;
  }
  return colorCounts;
}

// TileBag class
class TileBag {
public:
  TileBag() {
    initTiles();
    std::srand(std::time(0));
  }

  Tile drawRandomTile() {
    if (tiles.empty()) {
      throw std::runtime_error("No tiles left in the bag");
    }

    int randomIndex = std::rand() % tiles.size();
    Tile randomTile = tiles[randomIndex];
    tiles.erase(tiles.begin() + randomIndex);
    return randomTile;
  }

  void printContents() const { printTileCounts(tiles, "Tile Bag"); }

private:
  std::vector<Tile> tiles;

  void initTiles() {
    const std::vector<std::string> colors = {"Black", "Red", "Blue", "White",
                                             "Green"};
    int tilesPerColor = 20;

    for (const std::string &color : colors) {
      for (int i = 0; i < tilesPerColor; ++i) {
        tiles.emplace_back(color);
      }
    }
  };
};

// CentralMarket class
class CentralMarket {
public:
  void addTile(const Tile &tile) { tiles.push_back(tile); }

  std::vector<Tile> takeTilesOfColor(const std::string &color) {
    std::vector<Tile> takenTiles;

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
  std::vector<Tile> tiles;
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

  std::vector<Tile> takeTilesOfColor(const std::string &color) {
    std::vector<Tile> takenTiles;

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
  std::vector<Tile> tiles;
};

int main() {
  TileBag tileBag;
  FactoryDisplay factoryDisplay(tileBag);
  CentralMarket centralMarket;

  // Refill the factory display
  factoryDisplay.refill(tileBag);

  // Take all Red tiles from the factory display
  std::vector<Tile> takenTiles = factoryDisplay.takeTilesOfColor("Red");

  // Add the remaining tiles to the central market
  for (const Tile &tile : factoryDisplay.takeTilesOfColor("")) {
    centralMarket.addTile(tile);
  }

  // Take all Black tiles from the central market
  std::vector<Tile> takenTilesFromMarket =
      centralMarket.takeTilesOfColor("Black");

  // Print the contents of TileBag, FactoryDisplay, and CentralMarket
  tileBag.printContents();
  factoryDisplay.printContents();
  centralMarket.printContents();

  return 0;
}