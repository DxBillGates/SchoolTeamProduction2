#include "EnemyManager.h"
#include"CollisionMap.h"

EnemyManager::EnemyManager()
{

}
EnemyManager::~EnemyManager()
{
	for (const auto& x : shootEnemy)
	{
		delete x;
	}
	for (const auto& x : moveEnemy)
	{
		delete x;
	}
}
void EnemyManager::LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader)
{
	shootEnemy.resize(2);
	moveEnemy.resize(2);

	const std::vector<Vector2>& shootEnemyCount = CollisionMap::GetCurrentMap()->GetEnemiesChip();
	const std::vector<Vector2>& moveEnemyCount = CollisionMap::GetCurrentMap()->GetMoveEnemiesChip();

	for (int i = 0; i < shootEnemy.size(); ++i)
	{
		shootEnemy[i] = new ShootEnemy();
	}
	for (int i = 0; i < moveEnemy.size(); ++i)
	{
		moveEnemy[i] = new MoveEnemy();
	}
	/*for (int i = 0; i < (int)shootEnemyCount.size(); ++i)
	{
		shootEnemy.emplace_back(new ShootEnemy());
	}
	for (int i = 0; i < (int)moveEnemyCount.size(); ++i)
	{
		moveEnemy.emplace_back(new MoveEnemy());
	}*/
	for (auto& x : shootEnemy)
	{
		x->LoadAsset(device, heap, loader);
	}
	for (auto& x : moveEnemy)
	{
		x->LoadAsset(device, heap, loader);
	}

}
void EnemyManager::Initialize()
{
	const std::vector<Vector2>& normalEnemyCount = CollisionMap::GetCurrentMap()->GetEnemiesChip();
	const std::vector<Vector2>& moveEnemyCount = CollisionMap::GetCurrentMap()->GetMoveEnemiesChip();
	
	for (int i = 0; i < (int)shootEnemy.size(); ++i)
	{
		shootEnemy[i]->SetTransform({ {}, {}, {} });
		shootEnemy[i]->SetLiveFlag(false);
	}
	for (int i = 0; i < (int)normalEnemyCount.size(); ++i)
	{
		shootEnemy[i]->SetTransform({ {normalEnemyCount[i].x * 64 + 32.0f, -(normalEnemyCount[i].y * 64 + 32.0f),0 }, {}, {} });
		shootEnemy[i]->SetLiveFlag(true);
	}
	for (int i = 0; i < (int)moveEnemy.size(); ++i)
	{
		moveEnemy[i]->SetTransform({ {}, {}, {} });
		moveEnemy[i]->SetLiveFlag(false);
	}
	for (int i = 0; i < (int)moveEnemyCount.size(); ++i)
	{
		moveEnemy[i]->SetTransform({ {moveEnemyCount[i].x * 64 + 32.0f, -(moveEnemyCount[i].y * 64 + 32.0f),0 }, {}, {} });
		moveEnemy[i]->SetLiveFlag(true);
	}
	for (auto& x : shootEnemy)
	{
		x->Initialize();
	}
	for (auto& x : moveEnemy)
	{
		x->Initialize();
	}
}
void EnemyManager::Update()
{
	for (auto& x : shootEnemy)
	{
		x->Update();
	}
	for (auto& x : moveEnemy)
	{
		x->Update();
	}
}
void EnemyManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (auto& x : shootEnemy)
	{
		x->Draw(cmdList);
	}
	for (auto& x : moveEnemy)
	{
		x->Draw(cmdList);
	}
}

std::vector<ShootEnemy*>* EnemyManager::GetShootEnemy()
{
	return &shootEnemy;
}

std::vector<MoveEnemy*>* EnemyManager::GetMoveEnemy()
{
	return &moveEnemy;
}
