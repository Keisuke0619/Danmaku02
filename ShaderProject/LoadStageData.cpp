#include "LoadStageData.h"
#include "CsvLoader.h"
#include "Wall.h"
#include "CollisionSystem.h"
using namespace std;

static list<TMapTile> MapTile;

int g_loadStageMaxX;
int g_loadStageMaxY;

list<TMapTile>* LoadStageData(std::string stagePath)
{
    MapTile.clear();
    auto list = CSVLoad(stagePath);
    int x,y;
    y = 0;
    g_loadStageMaxX = 0;
    g_loadStageMaxY = 0;
    for (auto line : list)
    {
        x = -1; // 処理より先にインクリメントするため-1スタート。
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
            if (g_loadStageMaxX < x) g_loadStageMaxX = x;
        }
        if (g_loadStageMaxY < y) g_loadStageMaxY = y;
        y++;

    }
    return &MapTile;
}

void CreateStage(std::string stagePath, int offsetX, int offsetY)
{
    auto stageDatas = LoadStageData(stagePath);

    int left    = -offsetX - 1;
    int top     = -offsetY - 1;
    int right   = g_loadStageMaxX + left + 2;
    int bottom  = g_loadStageMaxY + top  + 2;
    CCollisionSystem::Delete();
#ifdef  _DEBUG
    CCollisionSystem::GetIns()->Create(6, left, top, right, bottom);
#else
    CCollisionSystem::GetIns()->Create(8, left, top, right, bottom);
#endif //  _DEBUG

    for (auto data : *stageDatas)
    {
        new CWall(data.x, data.y, data.id, offsetX, offsetY);
    }
}

