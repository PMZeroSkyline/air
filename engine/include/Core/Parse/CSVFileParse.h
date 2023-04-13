#ifndef CSV_FILE_PARSE_H
#define CSV_FILE_PARSE_H

#include "Platform/StringEdit/StringEdit.h"
#include "platform/Split/Split.h"
#include "SDK/STL/STL.h"

void CSVFileParse(const string &path, vector<vector<string>> &csv, bool removeSpace = false)
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

#endif
