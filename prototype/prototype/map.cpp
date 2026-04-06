#include "map.h"

void Map::Tile()
{
    std::vector<std::string> original = map;

    for (int y = 0; y < (int)map.size(); y++) {
        for (int x = 0; x < (int)map[y].size(); x++) {

            if (original[y][x] == '=') {

                bool leftSame = (x > 0 && original[y][x - 1] == '=');
                bool rightSame = (x < original[y].size() - 1 && original[y][x + 1] == '=');

                if (!leftSame) {
                    map[y][x] = 'L';
                }
                else if (!rightSame) {
                    map[y][x] = 'R';
                }
                else {
                    map[y][x] = '=';
                }
            }
            if (original[y][x] == '-') {

                bool leftSame = (x > 0 && original[y][x - 1] == '-');
                bool rightSame = (x < original[y].size() - 1 && original[y][x + 1] == '-');

                if (!leftSame) {
                    map[y][x] = 'l';
                }
                else if (!rightSame) {
                    map[y][x] = 'r';
                }
                else {
                    map[y][x] = '-';
                }
            }
        }
    }
}

bool Map::Load(const std::string& filename, int tileSize)
{
    this->tileSize = tileSize;
    map.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error " << filename << "\n";
        return false;
    }

    std::string line;
    int y = 0;

    while (std::getline(file, line)) {

        for (int x = 0; x < (int)line.size(); x++) {

            if (line[x] == '1') {
                player1SpawnPos = {
                    (float)(x * tileSize),
                    (float)(y * tileSize)
                };
                line[x] = '.';
            }

            if (line[x] == '2') {
                player2SpawnPos = {
                    (float)(x * tileSize),
                    (float)(y * tileSize)
                };
                line[x] = '.';
            }
        }
        map.push_back(line);
        y++;

    }

    file.close();

    Tile();

    return true;
}

void Map::Draw() const
{
    for (int y = 0; y < GetHeight(); y++) {
        for (int x = 0; x < GetWidth(); x++) {

            char tile = map[y][x];

            if (tile == '#') {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, DARKGRAY);
            }
            else if (tile == 'L') {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, BLUE);
            }
            else if (tile == 'R') {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, GREEN);
            }
            else if (tile == 'l') {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, BLUE);
            }
            else if (tile == 'r') {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, GREEN);
            }
            else if (tile == '=') {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, BROWN);
            }
            else if (tile == '-') {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, YELLOW);
            }
        }
    }
}


char Map::GetTile(int x, int y) const
{
    if (x < 0 || y < 0 || y >= GetHeight() || x >= GetWidth())
        return '#';

    return map[y][x];
}

bool Map::IsPlatform(int x, int y) const
{
    char tile = GetTile(x, y);
    return (tile == 'L' || tile == 'R' || tile == '=');
}

bool Map::IspassPlatform(int x, int y) const
{
    char tile = GetTile(x, y);
    return (tile == 'l' || tile == 'r' || tile == '-');
}

int Map::GetWidth() const
{
    return map.empty() ? 0 : (int)map[0].size();
}

int Map::GetHeight() const
{
    return (int)map.size();
}