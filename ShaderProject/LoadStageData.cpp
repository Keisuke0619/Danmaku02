#include "LoadStageData.h"
#include "Object.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "CsvLoader.h"

using namespace std;

static list<TMapTile> MapTile;



list<TMapTile>* LoadStageData(std::string stagePath)
{
    MapTile.clear();
    auto list = CSVLoad(stagePath);
    int x,y;
    y = 0;
    for (auto line : list)
    {
        x = -1; // ��������ɃC���N�������g���邽��-1�X�^�[�g�B
        for (auto content : line)
        {
            x++;
            int id = stoi(content);
            if (id == 0) continue;
            TMapTile tile;
            tile.x = x;
            tile.y = y;
            tile.id = id;
            MapTile.push_back(tile);
        }
        y++;
    }
    return &MapTile;
}

