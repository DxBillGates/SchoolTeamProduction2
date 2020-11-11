#include "CollisionMap.h"
Map* CollisionMap::currentMap = nullptr;
void CollisionMap::SetCurrentMap(Map * pMap)
{
	currentMap = pMap;
}

Map * CollisionMap::GetCurrentMap()
{
	return currentMap;
}

bool CollisionMap::CollisionCheckMapChipAndGameObject(GameObject & gameObject)
{
	//ゲームオブジェクトが今いるところのマップチップのどのタイルに存在するかをチェック
	//チェックに必要な情報を参照
	const std::vector<std::vector<int>>& refMapChip = currentMap->GetMapChipData();
	const Vector3& mapChipSize = currentMap->GetChipSize();
	const Vector3& gameObjectPosition = gameObject.GetTransform().position;
	int x, y, z = 0;
	x = (int)(gameObjectPosition.x / mapChipSize.x);
	y = -(int)(gameObjectPosition.y / mapChipSize.y);
	//判定
	if (refMapChip[y][x] != MapChipData::AIR)
	{
		return true;
	}
	return false;
}

bool CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(GameObject & gameObject, MapChipData chip)
{
	const std::vector<std::vector<int>>& refMapChip = currentMap->GetMapChipData();
	const Vector3& mapChipSize = currentMap->GetChipSize();
	const Vector3& gameObjectPosition = gameObject.GetTransform().position;
	const Vector3& gameObjectSize = gameObject.GetSize();
	int rx = 0, ty = 0;
	int lx = 0, by = 0;

	//ゲームオブジェクトの4頂点
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

bool CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(GameObject & gameObject, MapChipData chip, std::vector<int>& m)
{
	const std::vector<std::vector<int>>& refMapChip = currentMap->GetMapChipData();
	const Vector3& mapChipSize = currentMap->GetChipSize();
	const Vector3& gameObjectPosition = gameObject.GetTransform().position;
	const Vector3& gameObjectSize = gameObject.GetSize();
	int rx = 0, ty = 0;
	int lx = 0, by = 0;

	//ゲームオブジェクトの4頂点
	rx = (int)((gameObjectPosition.x - 1 + gameObjectSize.x / 2.0f) / mapChipSize.x);
	lx = (int)((gameObjectPosition.x - gameObjectSize.x / 2.0f) / mapChipSize.x);
	ty = -(int)((gameObjectPosition.y + gameObjectSize.y / 2.0f) / mapChipSize.y);
	by = -(int)((gameObjectPosition.y + 1 - gameObjectSize.y / 2.0f) / mapChipSize.y);

	//ヒットしたマップチップの番号を返す
	if (refMapChip[by][rx] == chip)
	{
		m = { rx,by };
		return true;
	}
	if (refMapChip[by][lx] == chip)
	{
		m = { lx,by };
		return true;
	}
	if (refMapChip[ty][lx] == chip)
	{
		m = { rx,ty };
		return true;
	}
	if (refMapChip[ty][rx] == chip)
	{
		m = { lx,ty };
		return true;
	}
	return false;
}

bool CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(GameObject & gameObject, MapChipData chip, Vector2 & v)
{
	const std::vector<std::vector<int>>& refMapChip = currentMap->GetMapChipData();
	const Vector3& mapChipSize = currentMap->GetChipSize();
	const Vector3& gameObjectPosition = gameObject.GetTransform().position;
	const Vector3& gameObjectSize = gameObject.GetSize();
	int rx = 0, ty = 0;
	int lx = 0, by = 0;

	//ゲームオブジェクトの4頂点
	rx = (int)((gameObjectPosition.x - 1 + gameObjectSize.x / 2.0f) / mapChipSize.x);
	lx = (int)((gameObjectPosition.x - gameObjectSize.x / 2.0f) / mapChipSize.x);
	ty = -(int)((gameObjectPosition.y + gameObjectSize.y / 2.0f) / mapChipSize.y);
	by = -(int)((gameObjectPosition.y + 1 - gameObjectSize.y / 2.0f) / mapChipSize.y);

	//ヒットしたマップチップの番号を返す
	if (refMapChip[by][rx] == chip)
	{
		v = { (float)rx,(float)by };
		return true;
	}
	if (refMapChip[by][lx] == chip)
	{
		v = { (float)lx,(float)by };
		return true;
	}
	if (refMapChip[ty][lx] == chip)
	{
		v = { (float)rx,(float)ty };
		return true;
	}
	if (refMapChip[ty][rx] == chip)
	{
		v = { (float)lx,(float)ty };
		return true;
	}
	return false;
	return false;
}

bool CollisionMap::CollisionCheckTriangleAndGameObject(GameObject & gameObject, Vector3 a, Vector3 b, Vector3 c)
{
	//外積
	float ab_ao, bc_bo, ca_co;
	ab_ao = (b.x - a.x)*(gameObject.GetTransform().position.y - a.y) - (b.y - a.y)*(gameObject.GetTransform().position.x - a.x);
	bc_bo = (c.x - b.x)*(gameObject.GetTransform().position.y - b.y) - (c.y - b.y)*(gameObject.GetTransform().position.x - b.x);
	ca_co = (a.x - c.x)*(gameObject.GetTransform().position.y - c.y) - (a.y - c.y)*(gameObject.GetTransform().position.x - c.x);
	if (ab_ao > 0 && bc_bo > 0 && ca_co > 0 || ab_ao < 0 && bc_bo < 0 && ca_co < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
	//float area = 0.5f *(-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
	//float s = 1 / (2 * area)*(a.y*c.x - a.x * c.y + (c.y - a.y)*gameObject.GetTransform().position.x + (a.x - c.x)*gameObject.GetTransform().position.y);
	//float t = 1 / (2 * area)*(a.x*b.y - a.y * b.x + (a.y - b.y)*gameObject.GetTransform().position.x + (b.x - a.x)*gameObject.GetTransform().position.y);
	//if ((s > 0) && (s < 1) && (t > 0) && (t < 1) && (1 - s - t > 0) && (1 - s - t < 1))
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}
}
