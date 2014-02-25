#include "LevelTracker.h"

#include <iostream>
#include <fstream>

std::vector<PuzzleData> LevelTracker::getPuzzles()
{
	std::vector<PuzzleData> puzzles;

	std::string textfilename = "Assets/Puzzles.txt";
	std::ifstream file (textfilename);
	std::string delimiter = ":";
	std::string line;
	if (file.is_open())
	{
		while ( getline (file,line) )
		{
			std::cout << line << std::endl;

			if(line.size() > 0 && line.front() == '#') continue;
			
			int splitIndex = line.find(delimiter);
			std::string foldername = line.substr(0, splitIndex);
			std::string ringsStr = line.substr(splitIndex + 1, line.size());
			try
			{
				int rings = std::stoi(ringsStr);
				PuzzleData puzzleData;
				puzzleData.foldername = foldername;
				puzzleData.rings = rings;
				puzzles.push_back(puzzleData);
				std::cout << "Puzzle added" << std::endl;
			}
			catch(const std::exception&){}
		}
		file.close();
	}
	else std::cout << "Unable to open " << textfilename << std::endl; 

	return puzzles;
}

std::vector<PlatformData> LevelTracker::getPlatformLevels()
{
	std::vector<PlatformData> platformLevels;

	std::string textfilename = "Assets/PlatformLevels.txt";
	std::ifstream file (textfilename);
	std::string delimiter = ":";
	std::string line;
	if (file.is_open())
	{
		while ( getline (file,line) )
		{
			std::cout << line << std::endl;

			if(line.size() > 0 && line.front() == '#') continue;

			PlatformData platformData;
			platformData.filename = line;
			platformLevels.push_back(platformData);

			std::cout << "Level added" << std::endl;
		}
		file.close();
	}
	else std::cout << "Unable to open " << textfilename << std::endl; 

	return platformLevels;
}