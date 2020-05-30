#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>

bool file_exists(std::string path)
{
	if(FILE *file = fopen(path.c_str(), "r")){
		fclose(file);
		return true;
	}
	return false;
}

std::vector<std::string> read_file(std::string path)
{
	std::vector<std::string> res;
	std::ifstream f(path);
	std::string line;
	while(std::getline(f,line)){
		res.push_back(line);
	}
	return res;
}

void delete_file(std::string path)
{
	std::remove(path.c_str());
}
