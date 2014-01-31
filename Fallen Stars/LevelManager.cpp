#include "LevelManager.h"
#include <iostream>


LevelManager::LevelManager(std::string levelname)
{
	map = new Tmx::Map();
	map->ParseFile("Libraries/Assets/Maps/" + levelname + ".tmx");

	//Resourcemanager loads the tilesets here
	if(map->HasError())
	{
		std::cout<< map->GetErrorCode()<<std::endl;
		std::cout<< map->GetErrorText()<<std::endl;
	}
}

void LevelManager::Load()
{
	for(int i = 0; i < map->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = map->GetTileset(i);
		if (tileset->GetTiles().size() > 0)
		{
			const Tmx::Tile *tile = *(tileset->GetTiles().begin());

			
		}

	}

	for( unsigned int k = 0; k < map->GetLayers().size(); k++)
	{
		const Tmx::Layer* layer = map->GetLayer(k);

		mWidth	= layer->GetWidth();
		mHeight = layer->GetHeight();

		
		std::string layername = layer->GetName();

		if(layername == "collision")
		{
			//Shit happens here and we add the platforms we can walk on
		}

		//Tmx::MapTile();
	}



}