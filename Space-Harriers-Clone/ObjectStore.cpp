#include "ObjectStore.h"

template<class T_KEY, class T_VALUE>
T_VALUE ObjectStore<T_KEY, T_VALUE>::GetByID(const T_KEY & key) const
{
	return store[key];
}

template<class T_KEY, class T_VALUE>
void ObjectStore<T_KEY, T_VALUE>::Add(T_KEY key, T_VALUE value)
{
	store[key] = value;
}

template<class T_KEY, class T_VALUE>
bool ObjectStore<T_KEY, T_VALUE>::Remove(T_KEY key)
{
	std::map<T_KEY, T_VALUE>::iterator it = store.find();
	bool has_key = it != store.end();
	if (has_key)
		store.erase(it);
	return has_key;
}

template<class T_KEY, class T_VALUE>
bool ObjectStore<T_KEY, T_VALUE>::HAS_KEY(T_KEY)
{
	std::map<T_KEY, T_VALUE>::iterator it = store.find();
	return it != store.end();
}