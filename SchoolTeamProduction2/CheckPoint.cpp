#include "CheckPoint.h"
Dx12_Mesh CheckPoint::mesh = {};
MeshData CheckPoint::meshData = {};

CheckPoint::CheckPoint() :cbData(nullptr)
{
	size = Vector3(64, 64, 64);
}

CheckPoint::~CheckPoint()
{
	delete cbData;
}

void CheckPoint::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cbData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);
}

void CheckPoint::Initialize()
{
}

void CheckPoint::Update()
{
	cbData->Map({ DirectX::XMMatrixScaling(size.x / 2,size.y / 2,size.z / 2) * DirectX::XMMatrixTranslation(transform.position.x,transform.position.y ,transform.position.z) });
}

void CheckPoint::Draw(ID3D12GraphicsCommandList * pCmdList)
{
	cbData->Set(pCmdList);
	mesh.Draw(pCmdList);
}

void CheckPoint::StaticLoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/", "CHECK_POINT", meshData);
	mesh.Create(device, &meshData);
}
