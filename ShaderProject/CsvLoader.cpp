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
    // �߂�l�̊�
    vector<vector<string>> ret;
    ifstream ifs(path);
    string line;
    // ��s���ǂݍ���
    while (getline(ifs, line))
    {
        // �w�b�_�[�𖳎����鏈���B��s��΂��B
        if (isDeleteHeader)
        {
            isDeleteHeader = false;
            continue;
        }
        // �ǂݍ��񂾈�s���R���}��؂�ɂ��āA���̔z���߂�l�̊�ɒǉ��B
        vector<string> len = split(line, ',');
        ret.push_back(len);
    }
    // �I�����A�߂�l��Ԃ��B
    ifs.close();
    return ret;
}