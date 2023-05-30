#ifndef SHARED_MAP_H
#define SHARED_MAP_H

#include "SDK/STL/STL.h"

template<typename T>
class SharedMap
{
public:
    map<string, shared_ptr<T>> m;
    shared_ptr<T> Get(const string &key)
    {
        auto it = m.find(key);
        if (it != m.end() && it->second.use_count() != 0)
        {
            return it->second;
        }
        return shared_ptr<T>();
    }
    void Set(const string &key, shared_ptr<T> value)
    {
        m[key] = value;
    }
};


#endif