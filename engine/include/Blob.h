#ifndef BLOB_H
#define BLOB_H

#include "Core.h"

template<typename T>
class Blob
{
public:
    map<string, weak_ptr<T>> m;
    shared_ptr<T> Get(const string &key)
    {
        auto it = m.find(key);
        if (it != m.end() && it->second.use_count() != 0)
        {
            return it->second.lock();
        }
        return shared_ptr<T>();
    }
    void Set(const string &key, shared_ptr<T> value)
    {
        m[key] = value;
    }
};

#endif