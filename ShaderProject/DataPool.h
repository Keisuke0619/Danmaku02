#pragma once

#include <string>

namespace DataPool
{
	const void* const GetData(std::string key);
	void AddData(std::string key, void* value);
	void ClearAll();
}