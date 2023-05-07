#include <iostream>
#include <vector>
#include <random>
#include <ctime>

// Tile class
class Tile
{
public:
    Tile(const std::string &color) : color(color) {}

    std::string getColor() const
    {
        return color;
    }

private:
    std::string color;
};

// TileBag class
class TileBag
{
public:
    TileBag()
    {
        initTiles();
        std::srand(std::time(0));
    }

    Tile drawRandomTile()
    {
        if (tiles.empty())
        {
            throw std::runtime_error("No tiles left in the bag");
        }

        int randomIndex = std::rand() % tiles.size();
        Tile randomTile = tiles[randomIndex];
        tiles.erase(tiles.begin() + randomIndex);
        return randomTile;
    }

private:
    std::vector<Tile> tiles;

    void initTiles()
    {
        const std::vector<std::string> colors = {"Black", "Red", "Blue", "White", "Green"};
        int tilesPerColor = 20;

        for (const std::string &color : colors)
        {
            for (int i = 0; i < tilesPerColor; ++i)
            {
                tiles.emplace_back(color);
            }
        }
    }
};