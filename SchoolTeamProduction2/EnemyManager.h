#include "GameObject.h"
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"
#include"PrototypeMap.h"
#include"ShootEnemy.h"
#include"MoveEnemy.h"

#pragma once
class EnemyManager
{

private:
	std::vector<ShootEnemy*> shootEnemy;
	std::vector<MoveEnemy*> moveEnemy;
public:
	EnemyManager();
	~EnemyManager();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	std::vector<ShootEnemy*>* GetShootEnemy();
	std::vector<MoveEnemy*>* GetMoveEnemy();
	
};

