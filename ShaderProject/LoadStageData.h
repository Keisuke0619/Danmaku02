#pragma once
#include <iostream>
#include <list>

struct TMapTile
{
	int x;
	int y;
	unsigned id;
};
std::list<TMapTile>* LoadStageData(std::string stagePath);