#include "CoinManager.h"
#include "CollisionMap.h"

CoinManager::CoinManager()
{
	coins.resize(3);
}

CoinManager::~CoinManager()
{
	coins.clear();
}

void CoinManager::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	Coin::StaticLoadAsset(device, heap, loader);
	for (auto& c : coins)
	{
		c.LoadAsset(device, heap, loader);
	}

}

void CoinManager::Initialize()
{
	const std::vector<Vector2>& coinChips = CollisionMap::GetCurrentMap()->GetCoinChips();
	const Vector3& size = coins[0].GetSize();
	for (int i = 0; i < (int)coinChips.size(); ++i)
	{
		if ((int)coins.size() < (int)coinChips.size())
		{
			assert(0);
		}
		coins[i].SetTransform({ Vector3(coinChips[i].x * size.x + size.x / 2,-(coinChips[i].y * size.y + size.y / 2),0),{}, {} });
		coins[i].Initialize();
	}
}

void CoinManager::Update()
{
	for (auto& c : coins)
	{
		if (c.GetLiveFlag())
		{
			c.Update();
		}
	}
}

void CoinManager::Draw(ID3D12GraphicsCommandList * pCmdList)
{
	for (auto& c : coins)
	{
		if (c.GetLiveFlag())
		{
			c.Draw(pCmdList);
		}
	}
}

std::vector<Coin>* CoinManager::GetCoins()
{
	return &coins;
}
