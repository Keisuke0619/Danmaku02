#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> split(std::string& input, char delimiter);
std::vector<std::vector<std::string>> CSVLoad(std::string path, bool isDeleteHeader = false);
