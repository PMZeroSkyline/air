#ifndef PREPROCESSOR
#define PREPROCESSOR

#include "SDK/STL/STL.h"
#include "Core/Parse/StringParse.h"
#include "Platform/File/BinFileRead.h"
#include "Platform/File/FileDirectory.h"

string PreprocessorFile(string path)
{
    vector<string> lines = StringToVectorString(move(StringFromFile(path)));
    fs::path filePath = path;
    CDWorkspacePath("air", filePath.parent_path());

    vector<string> output;
    for (const string& line : lines)
    {
        if (!IsLineValid(line) || IsLineComment(line))
        {
            continue;;
        }

        vector<string> vs = StreamLineToVector(line);

        if (vs.size() < 2)
        {
            output.push_back(line);
            continue;
        }
        if (vs[0] == "#include")
        {
            string includePath = vs[1];
            includePath = GetLineInRange(includePath, "\"");
            string includeStr = StringFromFile(includePath);
            output.push_back(move(includeStr));
            continue;
        }
        output.push_back(line);
    }
    string dst = VectorStringToString(output);

    CDWorkspacePath("air");

    fs::path inputFilePath = path;
    inputFilePath = inputFilePath.parent_path() / "Cache" / inputFilePath.filename();
    ofstream ofile(inputFilePath);
    ofile << dst;

    return dst;
};

#endif