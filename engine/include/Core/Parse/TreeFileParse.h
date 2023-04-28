#ifndef TREE_FILE_PARSE_H
#define TREE_FILE_PARSE_H

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"
#include "Gameplay/Object/Node.h"
#include "StringParse.h"

void TreeFileParse(const string& path, Node* result)
{
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
        size_t index = line.find_first_not_of(' ');
        int depth = index / 4;
        if (lastNode == nullptr)
        {
            if (depth == 0)
            {
                Node* node = result->AddChild<Node>();
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
            Node* node = lastNode->parent->AddChild<Node>();
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
                parent = parent->parent;
            }
            Node* node = parent->AddChild<Node>();
            node->name = line;
            lastNode = node;
            lastDepth = depth;
        }
        else if(depth > lastDepth)
        {
            if (depth - lastDepth == 1)
            {
                Node* node = lastNode->AddChild<Node>();
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
}

#endif