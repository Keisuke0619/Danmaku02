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
    // �}�b�v����x���ꂢ�ɂ���B
    MapTile.clear();
    // �w���CSV�����[�h
    auto list = CSVLoad(stagePath);
    int x,y;
    y = 0;
    g_loadStageMaxX = 0;
    g_loadStageMaxY = 0;
    // ���[�h�������X�g�����ƂɃf�[�^���쐬����B
    for (auto line : list)
    {
        x = -1; // ��������ɃC���N�������g���邽��-1�X�^�[�g�B
        for (auto content : line)
        {
            x++;
            int id = stoi(content);
            if (id == 0) continue;

            // �f�[�^���Z�b�g���A�o�^
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
    // �X�e�[�W�����[�h
    auto stageDatas = LoadStageData(stagePath);

    // �I�t�Z�b�g�����ƂɎl���؂̃��[�g�T�C�Y���w��
    int left    = -offsetX - 1;
    int top     = -offsetY - 1;
    int right   = g_loadStageMaxX + left + 2;
    int bottom  = g_loadStageMaxY + top  + 2;
    // ��x�l���؂����ꂢ�ɁB
    CCollisionSystem::Delete();
    // ���[�g�T�C�Y�ƍő�[�x���w�肵�A�����B
    CCollisionSystem::GetIns()->Create(6, left, top, right, bottom);

    // ���[�h�����X�e�[�W�f�[�^�����ƂɃu���b�N�𐶐�
    for (auto data : *stageDatas)
    {
        new CWall(data.x, data.y, data.id, offsetX, offsetY);
    }
}

