#include "Map.h"
#include<fstream>
#include<sstream>
#include<string>

Map::Map()
{
}

Map::~Map()
{
}

void Map::LoadMap(const char * fileName)
{
	//マップチップの読み込み
	using namespace std;
	ifstream file;
	//マップチップファイルを開く
	file.open(fileName);
	string line;
	int lineNumber = 0;
	//「,」を飛ばして数字を読み込んでいく
	while (getline(file, line))
	{
		istringstream lineStream(line);
		int number = 0;
		string key;
		mapChipData.push_back(vector<int>());
		while (getline(lineStream, key, ','))
		{
			lineStream >> number;
			mapChipData[lineNumber].push_back(number);
			if (number == (int)ENEMY)
			{
				enemiesChip.push_back(Vector2((float)mapChipData[lineNumber].size()-1,(float)lineNumber));
				mapChipData[lineNumber][mapChipData[lineNumber].size() - 1] = AIR;
			}
			else if (number == PLAYER)
			{
				playerChip = Vector2((float)mapChipData[lineNumber].size() - 1, (float)lineNumber);
				mapChipData[lineNumber][mapChipData[lineNumber].size() - 1] = AIR;
			}
			else if (number == (int)MOVE_ENEMY)
			{
				enemiesChip2.push_back(Vector2((float)mapChipData[lineNumber].size() - 1, (float)lineNumber));
				mapChipData[lineNumber][mapChipData[lineNumber].size() - 1] = AIR;
			}
			else if (number == (int)CHECK_POINT)
			{
				checkPointChip.push_back(Vector2((float)mapChipData[lineNumber].size() - 1, (float)lineNumber));
				mapChipData[lineNumber][mapChipData[lineNumber].size() - 1] = AIR;
			}
			else if (number == (int)COIN)
			{
				coinChip.push_back(Vector2((float)mapChipData[lineNumber].size() - 1, (float)lineNumber));
				mapChipData[lineNumber][mapChipData[lineNumber].size() - 1] = AIR;
			}
		}
		++lineNumber;
	}
}

Vector2 Map::GetPlayerChip()
{
	return playerChip;
}

std::vector<Vector2> Map::GetEnemiesChip()
{
	return enemiesChip;
}

std::vector<Vector2> Map::GetMoveEnemiesChip()
{
	return enemiesChip2;
}

std::vector<Vector2> Map::GetCheckPointChips()
{
	return checkPointChip;
}

std::vector<Vector2> Map::GetCoinChips()
{
	return coinChip;
}

std::vector<std::vector<int>>* Map::GetMapChipPointer()
{
	//マップチップのポインタを返す
	return &mapChipData;
}

std::vector<std::vector<int>> Map::GetMapChipData()
{
	return mapChipData;
}

Vector3 Map::GetChipSize()
{
	return size;
}

Vector2 Map::GetMapSize()
{
	int width, height;
	width = (int)mapChipData[0].size();
	height = (int)mapChipData.size();
	return Vector2((float)width, (float)height);
}

MapChipData Map::GetMapChipNumber(int x, int y)
{
	return (MapChipData)mapChipData[y][x];
}

void Map::SetMapChip(int x, int y, MapChipData number)
{
	mapChipData[y][x] = (int)number;
}

void Map::SetSize(Vector3 size)
{
	this->size = size;
}
