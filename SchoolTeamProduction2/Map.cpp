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
	//�}�b�v�`�b�v�̓ǂݍ���
	using namespace std;
	ifstream file;
	//�}�b�v�`�b�v�t�@�C�����J��
	file.open(fileName);
	string line;
	int lineNumber = 0;
	//�u,�v���΂��Đ�����ǂݍ���ł���
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
		}
		++lineNumber;
	}
}

std::vector<std::vector<int>>* Map::GetMapChipPointer()
{
	//�}�b�v�`�b�v�̃|�C���^��Ԃ�
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
