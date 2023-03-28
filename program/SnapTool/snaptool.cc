#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;
#define LOG(arg) cout << arg << endl;

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
int main(int argc, char **argv)
{
    if (argc != 4)
    {
        LOG("argc is : " << "path + begID + endID");
        return 0;
    }
    string path = argv[1];
    int begID = atoi(argv[2]);
    int endID = atoi(argv[3]);
    vector<vector<string>> csv;
    ReadCSV(path, csv, true);
    vector<vector<string>> dc;
    for (int i = 1; i < csv.size(); i++)
    {
        if (csv[i][0] != "")
        {
            dc.push_back(csv[i]);
        }
    }
    int face = 0;
    int inst_face = 0;
    float shaders_busy = 0;
    int clocks = 0;
    int draw = 0;
    int full_face = 0;
    int full_inst_face = 0;
    float full_shaders_busy = 0;
    int full_clocks = 0;
    int full_draw = 0;

    for (int i = 0; i < dc.size(); i++)
    {
        int count = 0, primcount = 0;
        if (dc[i][1] == "glDrawElements")
        {
            string params = dc[i][2].substr(1, dc[i][2].size()-2);
            vector<string> paramVec;
            SplitToVector(params, ";", paramVec);
            for (int j = 0; j < paramVec.size(); j++)
            {
                vector<string> paramItem;
                SplitToVector(paramVec[j], "=", paramItem);
                if (paramItem[0] == "count")
                {
                    count = stoi(paramItem[1]);
                }
            }
        }
        if (dc[i][1] == "glDrawElementsInstanced")
        {
            string params = dc[i][2].substr(1, dc[i][2].size()-2);
            vector<string> paramVec;
            SplitToVector(params, ";", paramVec);
            for (int j = 0; j < paramVec.size(); j++)
            {
                vector<string> paramItem;
                SplitToVector(paramVec[j], "=", paramItem);
                if (paramItem[0] == "count")
                {
                    count = stoi(paramItem[1]);
                }
                if (paramItem[0] == "primcount")
                {
                    primcount = stoi(paramItem[1]);
                }
            }
        }
        
        if (i >= begID && i <= endID)
        {
            face += count * (primcount == 0 ? 1 : primcount);
            inst_face += count;
            
            if (dc[i][5] != "")
            {
                shaders_busy += stof(dc[i][5]);
                clocks += stoi(dc[i][6]);
                draw++;
            }
        }
        full_face += count * (primcount == 0 ? 1 : primcount);
        full_inst_face += count;
        
        if (dc[i][5] != "")
        {
            full_shaders_busy += stof(dc[i][5]);
            full_clocks += stoi(dc[i][6]);
            full_draw++;
        }
    }
    LOG("dc size    \t" << dc.size())    
    LOG("dc range\t" << endID - begID)    
    LOG("dc range %\t" << ((float)(endID - begID) / (float)dc.size()) * 100)
    LOG("range face\t" << face)    
    LOG("range inst_face\t" << inst_face)
    LOG("full_face\t" << full_face)
    LOG("full_inst_face\t" << full_inst_face)
    LOG("range clocks\t" << clocks)
    LOG("full_clocks\t" << full_clocks)
    LOG("range clocks %\t" << ((float)clocks / (float)full_clocks) * 100.0f)
    LOG("shaders busy %\t" << shaders_busy / draw)
    LOG("full busy %\t" << full_shaders_busy / full_draw)
    
    return 0;
}