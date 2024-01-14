#include "DataPool.h"
#include <map>

namespace DataPool
{
	std::map<std::string, void*> g_dataPoolMap;
}

const void* const DataPool::GetData(std::string key)
{
	if (g_dataPoolMap.count(key) == 0) { return nullptr; }
	return g_dataPoolMap[key];
}

void DataPool::AddData(std::string key, void* value)
{
	if (g_dataPoolMap.count(key) != 0) { return; }
	g_dataPoolMap[key] = value;
}

void DataPool::ClearAll()
{
	g_dataPoolMap.clear();
}
