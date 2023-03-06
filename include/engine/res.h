#ifndef RESOURCE_H
#define RESOURCE_H

#include <map>
using std::map;
#include <string>
#include <memory>
using std::string;
using std::shared_ptr;
using std::weak_ptr;

struct res
{
	virtual ~res(){}
};

namespace g
{
	map<string, weak_ptr<res>> res_map;
}

template<typename T, typename... Args>
shared_ptr<T> res_mksp(const string &key, Args&&... args)
{
	shared_ptr<T> cr = make_shared<T>(std::forward<Args>(args)...);
    g::res_map[key] = cr;
    return dynamic_pointer_cast<T>(g::res_map[key].lock());
}
template<typename T>
shared_ptr<T> res_getsp(const string &key)
{
	if (g::res_map.find(key) == g::res_map.end() || g::res_map[key].use_count() == 0)
		return res_mksp<T>(key);
	return dynamic_pointer_cast<T>(g::res_map[key].lock());
}



#endif