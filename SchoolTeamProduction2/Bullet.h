#pragma once
#include "GameObject.h"
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"
class Bullet : public GameObject
{
private:
	Dx12_Mesh mesh;
	Dx12_CBuffer<DirectX::XMMATRIX>* cbData;
	int hitCount;
public:
	Bullet();
	~Bullet();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	int GetHitCount();
};
