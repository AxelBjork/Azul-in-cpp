#ifndef AZUL_GAME_H
#define AZUL_GAME_H

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
  Tile(const string &color);

  string getColor() const;

private:
  string color;
};

// Helper functions
map<string, int> getTileCounts(const vector<Tile> &tiles);
void printTileCounts(const vector<Tile> &tiles, const string &containerName);

// TileBag class
class TileBag {
public:
  TileBag();

  Tile drawRandomTile();
  void printContents() const;

private:
  vector<Tile> tiles;

  void initTiles();
};

// CentralMarket class
class CentralMarket {
public:
  bool center_taken = false;
  void addTile(const Tile &tile);

  vector<Tile> takeTilesOfColor(const string &color);

  bool isEmpty() const;
  map<string, int> getContents() const;
  void printContents() const;

  vector<Tile> tiles;
};

// FactoryDisplay class
class FactoryDisplay {
public:
  FactoryDisplay(TileBag &tileBag, int display_id);

  void refill(TileBag &tileBag);

  pair<vector<Tile>, vector<Tile>> takeTilesOfColor(const string &color);

  bool isEmpty() const;
  map<string, int> getContents() const;
  void printContents() const;

  vector<Tile> tiles;

private:
  int display_id;
};

// Factory class
class Factory {
public:
  Factory(int numFactoryDisplays);

  void refillDisplays();

  vector<Tile> takeDisplayTiles(int displayId, const string &color);

  vector<Tile> takeCenterTiles(const string &color);

  map<string, int> getContents(int displayId = -1) const;

  void printContents() const;

  vector<shared_ptr<FactoryDisplay>> factoryDisplays;
  CentralMarket centralMarket;

private:
  shared_ptr<TileBag> tileBag;
};

#endif // AZUL_GAME_H
