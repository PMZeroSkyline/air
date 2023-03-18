#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

void SplitToVector(const string &target, const string &splitTag, vector<string> &out)
{
    size_t offset = 0;
    size_t found;
    while ((found = target.find(splitTag, offset)) != string::npos)
    {
        out.push_back(target.substr(offset, found-offset));
        offset = found+1;
    }
    out.push_back(target.substr(offset, target.size()-offset));
}
void RemoveSpace(string &str)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}
void ReadCSV(const string &path, vector<vector<string>> &csv, bool removeSpace = false)
{
    ifstream csvFile(path);
    string line;
    while (getline(csvFile, line))
    {
        if (removeSpace)
            RemoveSpace(line);
        vector<string> v;
        csv.push_back(v);
        SplitToVector(line, ",", csv.back());
    }
}
int GetFaces(vector<vector<string>> &csv, int min, int max)
{
    int count = 0;
    for (auto vs : csv)
    {
        if (vs[0] == "ID")
            continue;    
        if (vs[0] == "")
            continue;
        if (stoi(vs[0]) < min)
            continue;
        if (stoi(vs[0]) > max)
            break;
        if (vs[1] == "glDrawElementsInstanced" || vs[1] == "glDrawElements")
        {
            string props = vs[2].substr(1, vs[2].size()-2);
            vector<string> propVec;
            SplitToVector(props, ";", propVec);
            if (vs[1] == "glDrawElementsInstanced")
            {
                int c, pc;
                for_each(propVec.begin(), propVec.end(), [&c, &pc](string &prop){
                    vector<string> keyValueVec;
                    SplitToVector(prop, "=", keyValueVec);
                    if (keyValueVec[0] == "count")
                        c = stoi(keyValueVec[1]);
                    if (keyValueVec[0] == "primcount")
                        pc = stoi(keyValueVec[1]);
                });
                count += c*pc;
            }
            if (vs[1] == "glDrawElements")
            {
                for_each(propVec.begin(), propVec.end(), [&count](string &prop){
                    vector<string> keyValueVec;
                    SplitToVector(prop, "=", keyValueVec);
                    if (keyValueVec[0] == "count")
                        count += stoi(keyValueVec[1]);
                });
            }
        }
    }
    return count / 3;
}
int main()
{
    
    vector<vector<string>> csv;
    ReadCSV("1.csv", csv, true);
    
    
    return 0;
}
