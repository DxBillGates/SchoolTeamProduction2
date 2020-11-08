#include "CollisionMap.h"
Map* CollisionMap::currentMap = nullptr;
void CollisionMap::SetCurrentMap(Map * pMap)
{
	currentMap = pMap;
}

bool CollisionMap::CollisionCheckMapChipAndGameObject(GameObject & gameObject)
{
	//�Q�[���I�u�W�F�N�g��������Ƃ���̃}�b�v�`�b�v�̂ǂ̃^�C���ɑ��݂��邩���`�F�b�N
	//�`�F�b�N�ɕK�v�ȏ����Q��
	const std::vector<std::vector<int>>& refMapChip = currentMap->GetMapChipData();
	const Vector3& mapChipSize = currentMap->GetChipSize();
	const Vector3& gameObjectPosition = gameObject.GetTransform().position;
	int x, y, z = 0;
	x = (int)(gameObjectPosition.x / mapChipSize.x);
	y = -(int)(gameObjectPosition.y / mapChipSize.y);
	//����
	if (refMapChip[y][x] != MapChipData::AIR)
	{
		return true;
	}
	return false;
}

bool CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(GameObject & gameObject,MapChipData chip)
{
	const std::vector<std::vector<int>>& refMapChip = currentMap->GetMapChipData();
	const Vector3& mapChipSize = currentMap->GetChipSize();
	const Vector3& gameObjectPosition = gameObject.GetTransform().position;
	const Vector3& gameObjectSize = gameObject.GetSize();
	int rx = 0, ty = 0;
	int lx = 0, by = 0;

	//�Q�[���I�u�W�F�N�g��4���_
	rx = (int)((gameObjectPosition.x - 1 + gameObjectSize.x / 2.0f) / mapChipSize.x);
	lx = (int)((gameObjectPosition.x - gameObjectSize.x / 2.0f) / mapChipSize.x);
	ty = -(int)((gameObjectPosition.y + gameObjectSize.y / 2.0f) / mapChipSize.y);
	by = -(int)((gameObjectPosition.y + 1 - gameObjectSize.y / 2.0f) / mapChipSize.y);

	if (refMapChip[by][rx] == chip || refMapChip[by][lx] == chip || refMapChip[ty][lx] == chip || refMapChip[ty][rx] == chip)
	{
		return true;
	}
	return false;
}
