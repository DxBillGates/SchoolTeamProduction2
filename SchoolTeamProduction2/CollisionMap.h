#pragma once
#include"GameObject.h"
#include"Map.h"

class CollisionMap
{
	static Map* currentMap;
public:
	CollisionMap();
	~CollisionMap();
	static void SetCurrentMap(Map* pMap);
	static Map* GetCurrentMap();
	static bool CollisionCheckMapChipAndGameObject(GameObject& gameObject);    //ゲームオブジェクトがいるマップチップのタイルを判定し単純な当たり判定を行う
	static bool CollisionCheckMapChipAndGameObjectFourCorner(GameObject& gameObject, MapChipData chip = GLASS);    //ゲームオブジェクトの四隅とチップを判定しどこが当たっているかを返す
	static bool CollisionCheckMapChipAndGameObjectFourCorner(GameObject& gameObject, MapChipData chip,std::vector<int>& m);
	static bool CollisionCheckMapChipAndGameObjectFourCorner(GameObject& gameObject, MapChipData chip,Vector2& v);
	static bool CollisionCheckTriangleAndGameObject(GameObject& gameObject, Vector3 a, Vector3 b, Vector3 c);
	static bool CollisionCheckTriangleAndRectangle(GameObject& gameObject, Vector3 a, Vector3 b, Vector3 c);
};

