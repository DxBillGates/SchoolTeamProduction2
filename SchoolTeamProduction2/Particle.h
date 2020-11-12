#pragma once
#include "GameObject.h"
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"
struct ParticleCBData
{
	DirectX::XMMATRIX model;
	DirectX::XMFLOAT4 color;
};
class Particle : public GameObject
{
private:
	static Dx12_Mesh mesh;
	Dx12_CBuffer<ParticleCBData>* cbData;
	DirectX::XMFLOAT4 color;
public:
	Particle();
	~Particle();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	static void LoadAsset_Mesh(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void SetColor(DirectX::XMFLOAT4 c);
};
