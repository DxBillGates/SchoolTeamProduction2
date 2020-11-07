#include "Player.h"
#include "CollisionMap.h"

Player::Player()
{
	size = Vector3(64, 64, 0);
}

Player::~Player()
{
	delete cbData;
}

void Player::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	//メッシュデータのロード
	MeshData meshData = {};
	loader->CreateModelData_Plane(size.x, size.y, meshData);
	//メッシュの生成
	mesh.Create(device, &meshData);
	cbData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);
}

void Player::Initialize()
{
	transform.position = Vector3(96, -96, 0);
}

void Player::Update()
{
	rigidbody.velocity = Vector3();
	if (keyboard->CheakHitKey(Key::A))
	{
		rigidbody.velocity += Vector3(-32, 0, 0);
	}
	if (keyboard->CheakHitKey(Key::D))
	{
		rigidbody.velocity += Vector3(32, 0, 0);
	}
	if (keyboard->CheakHitKey(Key::W))
	{
		rigidbody.velocity += Vector3(0, 32, 0);
	}
	if (keyboard->CheakHitKey(Key::S))
	{
		rigidbody.velocity += Vector3(0, -32, 0);
	}

	//x軸に移動量を加算し一度判定
	//当たっていたら加算をなかったことにする
	transform.position.x += rigidbody.velocity.x;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
	{
		transform.position.x -= rigidbody.velocity.x;
	}
	//y軸に移動量を加算しもう一度判定
	//当たっていたら加算をなかったことにする
	transform.position.y += rigidbody.velocity.y;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
	{
		transform.position.y -= rigidbody.velocity.y;
	}
}

void Player::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cbData->Map({ DirectX::XMMatrixTranslation(transform.position.x,transform.position.y,transform.position.z) });
	cbData->Set(cmdList);
	mesh.Draw(cmdList);
}

void Player::SetInputDevice(Keyboard * pKeyboard, Xinput * pCtrler)
{
	keyboard = pKeyboard;
	ctrler = pCtrler;
}
