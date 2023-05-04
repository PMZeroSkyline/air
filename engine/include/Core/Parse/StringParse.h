#ifndef SPLIT_H
#define SPLIT_H

#include "SDK/STL/STL.h"

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
bool IsCommentString(const string& inStr) {
    string str = inStr;
    RemoveSpace(str);
    if (str.size() < 2) 
    {
        return false;
    }
    if (str.substr(0, 2) == "//") 
    {
        return true;
    }
    return false;
}
bool IsValidString(const string& inStr)
{
    string str = inStr;
    RemoveSpace(str);
    if (str.size() == 0)
    {
        return false;
    }
    return true;
}
int GetStringIndent(const string& str)
{
    if (str == "")
    {
        return 0;
    }
    size_t index = str.find_first_not_of(' ');
    if (index == string::npos)
    {
        return 0;
    }
    int depth = index / 4;
    return depth;
}
string GetBracketContent(const string& src, const string& bracket)
{
    size_t beg, end;
    beg = src.find_first_of(bracket);
    end = src.find_last_of(bracket);
    if (beg != string::npos && end != string::npos)
    {
        return src.substr(beg+1, end-beg-1);
    }
    return "";
}
string ToLower(std::string str) 
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}
#endif