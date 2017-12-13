#pragma once

#include <map>

template <class T_KEY, class T_VALUE>
class ObjectStore
{
public:
	T_VALUE GetByID(const T_KEY&) const;
	void Add(T_KEY, T_VALUE);
	bool Remove(T_KEY);
	bool HAS_KEY(T_KEY);

private:
	std::map<T_KEY,T_VALUE> store;
};