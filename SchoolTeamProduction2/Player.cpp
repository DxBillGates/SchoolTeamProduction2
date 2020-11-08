#include "Player.h"
#include "CollisionMap.h"

Player::Player()
	:
	jumpFlag(false)
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

	bullet.LoadAsset(device, heap, loader);
}

void Player::Initialize()
{
	bullet.Initialize();
	transform.position = Vector3(96, -96, 0);
	jumpFlag = false;
	direction = true;
}

void Player::Update()
{
	const float G = 0.8f;    //重力
	const float JUMP_POWER = 16;    //ジャンプ力
	const float MOVE_POWER = 8;    //移動量

	rigidbody.velocity.x = 0;
	if (keyboard->CheakHitKey(Key::A))
	{
		rigidbody.velocity += Vector3(-MOVE_POWER, 0, 0);
		direction = false;
	}
	if (keyboard->CheakHitKey(Key::D))
	{
		rigidbody.velocity += Vector3(MOVE_POWER, 0, 0);
		direction = true;
	}


	//ジャンプフラグがtrueじゃないときにスペースキーを押すとジャンプ
	if (keyboard->CheakHitKey(Key::SPACE))
	{
		if (!jumpFlag)
		{
			jumpFlag = true;
			rigidbody.velocity.y = JUMP_POWER;
		}
	}
	//x軸に移動量を加算し一度判定
	//当たっていたら加算をなかったことにする
	transform.position.x += rigidbody.velocity.x;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
	{
		transform.position.x -= rigidbody.velocity.x;
		rigidbody.velocity.x = 0;
	}

	//速度ベクトルに重力を加算
	rigidbody.velocity.y -= G;
	//y軸に移動量を加算しもう一度判定
	//当たっていたら加算をなかったことにする
	transform.position.y += rigidbody.velocity.y;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
	{
		transform.position.y -= rigidbody.velocity.y;
		if (jumpFlag && rigidbody.velocity.y < 0)
		{
			jumpFlag = false;
		}
		rigidbody.velocity.y = 0;
	}
	//弾が使えない状態ならキャラの座標に持ってくる
	if (!bullet.GetLiveFlag())
	{
		bullet.SetTransform({ {transform.position.x,transform.position.y + 8,0 }, {}, {}});
	}
	//使える弾ならキャラの向きの方向に飛ばす
	if (!bullet.GetLiveFlag())
	{
		bullet.SetLiveFlag(true);
		if (direction)bullet.SetRigidbody({ Vector3(16,0,0) });
		else bullet.SetRigidbody({ Vector3(-16,0,0) });
	}
	const int MAX_HIT_COUNT = 9;
	if (bullet.GetHitCount() == MAX_HIT_COUNT)
	{
		bullet.SetTransform({ transform.position,{},{} });
		bullet.SetRigidbody({ Vector3(0,0,0) });
		bullet.Initialize();
	}
	bullet.Update();
}

void Player::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cbData->Map({ DirectX::XMMatrixTranslation(transform.position.x,transform.position.y,transform.position.z) });
	cbData->Set(cmdList);
	mesh.Draw(cmdList);

	bullet.Draw(cmdList);
}

void Player::SetInputDevice(Keyboard * pKeyboard, Xinput * pCtrler)
{
	keyboard = pKeyboard;
	ctrler = pCtrler;
}
