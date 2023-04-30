#ifndef TREE_FILE_PARSE_H
#define TREE_FILE_PARSE_H

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"
#include "Gameplay/Object/Node.h"
#include "StringParse.h"

template<typename T>
shared_ptr<T> TreeFileParse(const string& path)
{
    shared_ptr<T> result = make_shared<T>();
    Node* lastNode = nullptr;
    int lastDepth = 0;

    ifstream f(path);
    string line;
    while (getline(f, line))
    {
        if (!IsValidString(line) || IsCommentString(line))
        {
            continue;
        }
        int depth = GetStringIndent(line);
        if (lastNode == nullptr)
        {
            if (depth == 0)
            {
                Node* node = ((Node*)result.get())->AddChild<T>();
                node->name = line;
                lastNode = node;
                lastDepth = depth;
            }
            else
            {
                LOG("failed to load node tree !")
            }
        }
        else if (depth == lastDepth)
        {
            Node* node = ((Node*)lastNode->parent)->AddChild<T>();
            node->name = line;
            lastNode = node;
            lastDepth = depth;
        }
        else if(depth < lastDepth)
        {
            int offset = lastDepth - depth;
            Node* parent = lastNode->parent;
            for (int i = 0; i < offset; i++)
            {
                parent = (T*)(parent->parent);
            }
            Node* node = ((Node*)parent)->AddChild<T>();
            node->name = line;
            lastNode = node;
            lastDepth = depth;
        }
        else if(depth > lastDepth)
        {
            if (depth - lastDepth == 1)
            {
                Node* node = ((Node*)lastNode)->AddChild<T>();
                node->name = line;
                lastNode = node;
                lastDepth = depth;
            }
            else
            {
                LOG("failed to load node tree")
            }
        }
    }
    return result;
}

#endif