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
    // 戻り値の器
    vector<vector<string>> ret;
    ifstream ifs(path);
    string line;
    // 一行ずつ読み込み
    while (getline(ifs, line))
    {
        // ヘッダーを無視する処理。一行飛ばす。
        if (isDeleteHeader)
        {
            isDeleteHeader = false;
            continue;
        }
        // 読み込んだ一行をコンマ区切りにして、その配列を戻り値の器に追加。
        vector<string> len = split(line, ',');
        ret.push_back(len);
    }
    // 終了し、戻り値を返す。
    ifs.close();
    return ret;
}