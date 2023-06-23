#ifndef SDK_STL_H
#define SDK_STL_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <tuple>
#include <limits>
#include <filesystem>
#include <initializer_list>
#include <cmath>
#include <functional>
#include <algorithm>
#include <sstream>
using std::cout;
using std::endl;
using std::begin;
using std::end;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::tuple;
using std::map;
using std::list;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::weak_ptr;
using std::ostream;
using std::to_string;
using std::move;
using std::dynamic_pointer_cast;
using std::numeric_limits;
using std::pair;
using std::make_pair;
using std::forward;
using std::min;
using std::max;
using std::clamp;
using std::string_view;
using std::stringstream;
using std::istringstream;
using std::ostringstream;
namespace fs = std::filesystem;
using std::initializer_list;
using std::function;
using std::sort;


#define IsSharedType(sp, type) auto& r = *sp.get(); typeid(r) == typeid(type)
#define IsType(p, type) typeid(*p) == typeid(type)



#endif