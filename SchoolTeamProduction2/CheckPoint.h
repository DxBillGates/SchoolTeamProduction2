#pragma once
#include "GameObject.h"
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"

class CheckPoint : public GameObject
{
private:
	static Dx12_Mesh mesh;
	static MeshData meshData;
	Dx12_CBuffer<DirectX::XMMATRIX>* cbData;
public:
	CheckPoint();
	~CheckPoint();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* pCmdList);
	static void StaticLoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
};
