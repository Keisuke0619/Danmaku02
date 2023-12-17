#include "LoadStageData.h"
#include "Object.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

static list<TMapTile> MapTile;

vector<string> split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

list<TMapTile>* LoadStageData(std::string stagePath)
{
    ifstream ifs(stagePath);
    MapTile.clear();
    string line;
    int y = 0;
    while (getline(ifs, line)) {

        vector<string> strvec = split(line, ',');

        for (int x = 0; x < strvec.size(); x++) {
            int id = stoi(strvec.at(x));
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