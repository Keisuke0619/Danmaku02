#include "LoadStageData.h"
#include "Object.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

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

void LoadStageData(std::string stagePath)
{
    ifstream ifs("data.csv");

    string line;
    while (getline(ifs, line)) {

        vector<string> strvec = split(line, ',');

        for (int i = 0; i < strvec.size(); i++) {
            printf("%5d\n", stoi(strvec.at(i)));
        }

    }
    
}