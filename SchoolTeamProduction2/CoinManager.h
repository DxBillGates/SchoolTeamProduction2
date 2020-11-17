#pragma once
#include "Coin.h"
#include <vector>

class CoinManager
{
private:
	std::vector<Coin> coins;
public:
	CoinManager();
	~CoinManager();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* pCmdList);
	std::vector<Coin>* GetCoins();
};
