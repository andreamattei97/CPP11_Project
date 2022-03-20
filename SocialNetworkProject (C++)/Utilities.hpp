#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <functional>
#include <algorithm>
#include <list>
#include <memory>

using namespace std;

template<typename T>
using condition = function<bool(const T&)>;

inline bool Contains(const list<int>& data, int element)
{
	return find(data.begin(), data.end(), element) != data.end();
}

#endif

