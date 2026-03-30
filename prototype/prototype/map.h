#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <raylib.h>
#include <iostream>

struct Map
{
public:
    bool Load(const std::string& filename, int tileSize);
    bool IsWall(int x, int y) const;

    void Tile();
    void Draw() const;

    bool IsPlatform(int x, int y) const;
    char GetTile(int x, int y) const;

    int GetWidth() const;
    int GetHeight() const;
    int GetTileSize() const { return tileSize; }

    Vector2 player1SpawnPos = { 0, 0 };
    Vector2 player2SpawnPos = { 0, 0 };


private:
    std::vector<std::string> map;
    int tileSize = 32;

};