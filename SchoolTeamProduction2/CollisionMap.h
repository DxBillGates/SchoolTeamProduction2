#pragma once
#include"GameObject.h"
#include"Map.h"

class CollisionMap
{
	static Map* currentMap;
public:
	static void SetCurrentMap(Map* pMap);
	static bool CollisionCheckMapChipAndGameObject(GameObject& gameObject);    //ゲームオブジェクトがいるマップチップのタイルを判定し単純な当たり判定を行う
	static bool CollisionCheckMapChipAndGameObjectFourCorner(GameObject& gameObject, MapChipData chip = GLASS);    //ゲームオブジェクトの四隅とチップを判定しどこが当たっているかを返す
	static bool CollisionCheckMapChipAndGameObjectFourCorner(GameObject& gameObject, MapChipData chip,std::vector<int>& m);
	static bool CollisionCheckTriangleAndGameObject(GameObject& gameObject, Vector3 a, Vector3 b, Vector3 c);
};

