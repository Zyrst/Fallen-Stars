#pragma once

#include <string>
#include <vector>

struct PuzzleData
{
	std::string foldername;
	int rings;
};

struct PlatformData
{
	std::string filename;
};

namespace LevelTracker
{
	std::vector<PuzzleData> getPuzzles();
	std::vector<PlatformData> getPlatformLevels();
};

