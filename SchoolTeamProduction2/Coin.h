#pragma once
#include "GameObject.h"
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"
class Coin : public GameObject
{
private:
	static Dx12_Mesh mesh;
	static MeshData meshData;
	Dx12_CBuffer<DirectX::XMMATRIX>* cbData;
	float angleY;
public:
	Coin();
	~Coin();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	static void StaticLoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
};