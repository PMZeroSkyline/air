#ifndef CSV_FILE_PARSE_H
#define CSV_FILE_PARSE_H

#include "StringParse.h"
#include "SDK/STL/STL.h"

vector<vector<string>> ParseCSV(const string &path)
{
    vector<vector<string>> csv;
    ifstream f(path);
    string line;
    while (getline(f, line))
    {
        csv.push_back(move(Split(line, ',')));
    }
    return csv;
}

// void CSVFileParse(const string &path, vector<vector<string>> &csv, bool removeSpace = false)
// {
//     ifstream csvFile(path);
//     string line;
//     while (getline(csvFile, line))
//     {
//         if (removeSpace)
//             RemoveSpace(line);
//         vector<string> v;
//         csv.push_back(v);
//         // SplitToVector(line, ",", csv.back());
//         csv.back() = move(Split(line, ','));
//     }
// }

#endif
