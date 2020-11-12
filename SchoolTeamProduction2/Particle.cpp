#include "Particle.h"
Dx12_Mesh Particle::mesh = {};
Particle::Particle()
{
}

Particle::~Particle()
{
	delete cbData;
}

void Particle::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cbData = new Dx12_CBuffer<ParticleCBData>(device, heap, 1);
}

void Particle::Initialize()
{
	transform = {};
	rigidbody = {};
	liveFlag = false;
	size = {};
	objectTime = 1.0f;
}

void Particle::Update()
{
	transform.position = {640,-360,0};
	transform.rotation = { 0,0,0 };
	transform.scale = { 1,1,1 };
	color = { 1,1,1,1 };
}

void Particle::Draw(ID3D12GraphicsCommandList * cmdList)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
	mat *= DirectX::XMMatrixRotationZ(transform.rotation.z);
	mat *= DirectX::XMMatrixRotationX(transform.rotation.x);
	mat *= DirectX::XMMatrixRotationY(transform.rotation.y);
	mat *= DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
	cbData->Map({ mat,color });

	cbData->Set(cmdList);
	mesh.Draw(cmdList);
}

void Particle::LoadAsset_Mesh(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	MeshData cubeData;
	loader->LoadMeshData("Resources/Model/", "CUBE", cubeData);
	mesh.Create(device, &cubeData);
}

void Particle::SetColor(DirectX::XMFLOAT4 c)
{
	color = c;
}
