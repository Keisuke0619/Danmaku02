#include "CsvLoader.h"

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

vector<vector<string>> CSVLoad(std::string path, bool isDeleteHeader)
{
    vector<vector<string>> ret;
    ifstream ifs(path);
    string line;
    while (getline(ifs, line))
    {
        if (isDeleteHeader)
        {
            isDeleteHeader = false;
            continue;
        }
        vector<string> len = split(line, ',');
        ret.push_back(len);
    }
    return ret;
}