#include "azul_game.h"

// Tile class
Tile::Tile() : color("") {} // Add a default constructor

Tile::Tile(const string &color) : color(color) {}

string Tile::getColor() const { return color; }

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
TileBag::TileBag() {
  initTiles();
  srand(time(0));
}

Tile TileBag::drawRandomTile() {
  if (tiles.empty()) {
    throw runtime_error("No tiles left in the bag");
  }

  int randomIndex = rand() % tiles.size();
  Tile randomTile = tiles[randomIndex];
  tiles.erase(tiles.begin() + randomIndex);
  return randomTile;
}

void TileBag::printContents() const { printTileCounts(tiles, "Tile Bag"); }

void TileBag::initTiles() {
  const vector<string> colors = {"Black", "Red", "Blue", "Yellow", "Green"};
  int tilesPerColor = 20;

  for (const string &color : colors) {
    for (int i = 0; i < tilesPerColor; ++i) {
      tiles.emplace_back(color);
    }
  }
}

// CentralMarket class
void CentralMarket::addTile(const Tile &tile) { tiles.push_back(tile); }

vector<Tile> CentralMarket::takeTilesOfColor(const string &color) {
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
    Tile penalty_tile = tiles[5]; // todo fix this is not a new tile
    center_taken = true;
    takenTiles.push_back(penalty_tile);
  }

  return takenTiles;
}

bool CentralMarket::isEmpty() const { return tiles.empty(); }

map<string, int> CentralMarket::getContents() const {
  return getTileCounts(tiles);
}

void CentralMarket::printContents() const {
  printTileCounts(tiles, "Central Market");
}

// FactoryDisplay class
FactoryDisplay::FactoryDisplay(TileBag &tileBag, int display_id)
    : display_id(display_id) {
  refill(tileBag);
}

void FactoryDisplay::refill(TileBag &tileBag) {
  tiles.clear();
  for (int i = 0; i < 4; ++i) {
    tiles.push_back(tileBag.drawRandomTile());
  }
}
pair<vector<Tile>, vector<Tile>>
FactoryDisplay::takeTilesOfColor(const string &color) {
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

bool FactoryDisplay::isEmpty() const { return tiles.empty(); }

map<string, int> FactoryDisplay::getContents() const {
  return getTileCounts(tiles);
}

void FactoryDisplay::printContents() const {
  if (tiles.empty()) {
    cout << "Factory Display " << to_string(display_id) << " is empty" << endl;
    return;
  }
  printTileCounts(tiles, "Factory Display " + to_string(display_id));
}

// Factory class
Factory::Factory(int numFactoryDisplays) : centralMarket() {
  tileBag = make_shared<TileBag>();
  for (int i = 0; i < numFactoryDisplays; ++i) {
    factoryDisplays.push_back(make_shared<FactoryDisplay>(*tileBag, i));
  }
}

void Factory::refillDisplays() {
  for (const auto &display : factoryDisplays) {
    if (display->isEmpty()) {
      display->refill(*tileBag);
    }
  }
}

vector<Tile> Factory::takeDisplayTiles(int displayId, const string &color) {
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

vector<Tile> Factory::takeCenterTiles(const string &color) {
  vector<Tile> takenTiles = centralMarket.takeTilesOfColor(color);
  return takenTiles;
}

map<string, int> Factory::getContents(int displayId) const {
  if (displayId == -1) {
    return centralMarket.getContents();
  } else {
    return factoryDisplays[displayId]->getContents();
  }
}

void Factory::printContents() const {
  tileBag->printContents();
  centralMarket.printContents();
  for (const auto &display : factoryDisplays) {
    display->printContents();
  }
}