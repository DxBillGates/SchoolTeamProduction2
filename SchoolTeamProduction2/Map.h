#pragma once
#include"Dx12_CBVSRVUAVHeap.h"
#include"LoadContents.h"
#include"Vector.h"
#include<d3d12.h>
#include<vector>

enum MapChipData
{
	AIR,
	GLASS,
	GROUND,
	UP_LEFT_SLOPE,
	UP_RIGHT_SLOPE,
	DOWN_LEFT_SLOPE,
	DOWN_RIGHT_SLOPE,
	ENEMY,
	CHECK_POINT,
	MAX_DATA,
};

//基底クラス
class Map
{
protected:
	//マップチップデータ
	std::vector<std::vector<int>> mapChipData;
	Vector3 size;
public:
	Map();
	virtual ~Map();
	//マップロード関数
	void LoadMap(const char* fileName);
	//純粋仮想関数
	virtual void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader) = 0;
	virtual void Initialize() = 0;
	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
	//Getter、Setter
	std::vector<std::vector<int>>* GetMapChipPointer();    //マップチップのポインタを返す
	std::vector<std::vector<int>> GetMapChipData();    //マップチップのデータを返す
	Vector3 GetChipSize();    //チップ一つのサイズを返す
	Vector2 GetMapSize();    //マップのサイズを返す
	MapChipData GetMapChipNumber(int x, int y);    //指定チップの情報を返す
	void SetMapChip(int x, int y, MapChipData number = AIR);    //指定チップを変更
	void SetSize(Vector3 size);    //チップのサイズを変更
};

