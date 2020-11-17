#include "Coin.h"
Dx12_Mesh Coin::mesh = {};
MeshData Coin::meshData = {};

Coin::Coin():cbData(nullptr),angleY(0)
{
	size = Vector3(64, 64, 64);
}

Coin::~Coin()
{
	delete cbData;
}

void Coin::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cbData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);
}

void Coin::Initialize()
{
	liveFlag = true;
}

void Coin::Update()
{
	angleY += 0.01f;
	cbData->Map({ DirectX::XMMatrixScaling(size.x/2,size.y/2,size.z/2) * DirectX::XMMatrixRotationY(angleY) * DirectX::XMMatrixTranslation(transform.position.x,transform.position.y,transform.position.z) });
}

void Coin::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cbData->Set(cmdList);
	mesh.Draw(cmdList);
}

void Coin::StaticLoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/", "COIN", meshData);
	mesh.Create(device, &meshData);
}
