#ifndef SPLIT_H
#define SPLIT_H

#include "SDK/STL/STL.h"


void Split(const string &src, char tag, vector<string>& dst)
{
    size_t it = 0;
    size_t pos;
    while ((pos = src.find(tag, it)) != string::npos)
    {
        dst.push_back(src.substr(it, pos-it));
        it = pos+1;
    }
    dst.push_back(src.substr(it, src.size()-it));
}
vector<string> Split(const string &src, char tag)
{
    vector<string> dst;
    Split(src, tag, dst);
    return dst;
}
void StringToVectorString(const string& src, vector<string>& dst)
{
    return Split(src, '\n', dst);
}
vector<string> StringToVectorString(const string& src)
{
    vector<string> dst;
    StringToVectorString(src, dst);
    return dst;
}
void VectorStringToString(const vector<string>& src, string& dst)
{
    for (const auto& line : src)
    {
        dst += line + '\n';
    }
}
string VectorStringToString(const vector<string>& src)
{
    string dst;
    VectorStringToString(src, dst);
    return dst;
}
void StreamLineToVector(const string& src, vector<string>& dst)
{
    istringstream iss(src);
    string s;
    while (iss >> s)
    {
        dst.push_back(s);
    }
}
vector<string> StreamLineToVector(const string& src)
{
    vector<string> dst;
    StreamLineToVector(src, dst);
    return dst;
}
string RemoveLineSpace(const string& src)
{
    string dst = src;
    dst.erase(remove_if(dst.begin(), dst.end(), isspace), dst.end());
    return dst;
}
bool IsLineComment(const string& line) {
    string src = RemoveLineSpace(line);
    if (src.size() < 2) 
    {
        return false;
    }
    if (src.substr(0, 2) == "//") 
    {
        return true;
    }
    return false;
}
bool IsLineValid(const string& line)
{
    string src = RemoveLineSpace(line);
    if (src.size() == 0)
    {
        return false;
    }
    return true;
}
int GetLineIndent(const string& src)
{
    if (src == "")
    {
        return 0;
    }
    size_t index = src.find_first_not_of(' ');
    if (index == string::npos)
    {
        return 0;
    }
    int depth = index / 4;
    return depth;
}
string GetLineInRange(const string& src, const string& token)
{
    size_t beg, end;
    beg = src.find_first_of(token);
    if (beg == string::npos)
    {
        return "";
    }
    end = src.find_last_of(token);
    if (end == string::npos)
    {
        return "";
    }
    return src.substr(beg+1, end-(beg+1));
}
string ToLower(std::string src) 
{
    transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}
#endif