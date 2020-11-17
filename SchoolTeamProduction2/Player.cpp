#include "Player.h"
#include "CollisionMap.h"

Player::Player()
	:
	jumpFlag(false),
	direction(true)
{
	size = Vector3(64, 64, 0);
}

Player::~Player()
{
	delete cbData;
	delete hpTex;
	for (auto& h : hpCBDatas)
	{
		delete h;
	}
}

void Player::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	//メッシュデータのロード
	MeshData meshData = {};
	loader->LoadMeshData("Resources/Model/", "player",meshData);
	//loader->CreateModelData_Plane(size.x, size.y, meshData);
	//メッシュの生成
	mesh.Create(device, &meshData);
	cbData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);

	bullets.resize(32);
	for (auto& b : bullets)
	{
		b.LoadAsset(device, heap, loader);
	}

	SpriteData spriteData = {};
	loader->CreateModelData_Plane(64, 64, spriteData);
	hpSprite.Create(device, &spriteData);
	hpTex = loader->LoadTexture("Resources/Texture/", "ハート.png");
	const int MAX_HP = 10;
	hpCBDatas.resize(MAX_HP);
	for (int i = 0; i < MAX_HP; ++i)
	{
		hpCBDatas[i] = new Dx12_CBuffer<AnimetionData>(device, heap, 1);
	}
}

void Player::Initialize()
{
	const int MAX_HP = 10;
	jumpFlag = false;
	direction = true;
	liveFlag = true;
	Vector2 pos = CollisionMap::GetCurrentMap()->GetPlayerChip();
	transform.position = Vector3(pos.x * size.x + 32.0f, -(pos.y * size.y + 32.0f), 0);
	objectTime = 1.0f;
	frameCount = 0;
	life = MAX_HP;
	invincivleFlag = false;
	invincivleCount = 0;
	for (auto& b : bullets)
	{
		b.Initialize();
	}
	angleY = 3.14f;
}

void Player::Update()
{
	const float G = 0.8f;    //重力
	const float JUMP_POWER = 16;    //ジャンプ力
	const float MOVE_POWER = 8;    //移動量
	const int MAX_HIT_COUNT = 4;    //弾の最大ヒット数
	const int DIAGONAL_SHOT_BULLET_NUM = 8;    //斜め打ちの一度に出す量
	const int INTERVAL = 60;    //弾を発射する間隔
	const int INVINCIBLE_TIME = 30;    //無敵時間
	Vector2 hpTexSize = Vector2((float)hpTex->GetMetadata()->width, (float)hpTex->GetMetadata()->height);
	rigidbody.velocity.x = 0;

	if (liveFlag)
	{
		//斜めうち
		if (jumpFlag)
		{
			if (keyboard->KeyPressTrigger(Key::SPACE))
			{
				std::vector<Vector2> bulletvels;
				std::vector<Vector2> bulletposies;
				//弾を飛ばす方向を計算
				for (int i = 0; i < DIAGONAL_SHOT_BULLET_NUM; ++i)
				{
					Vector2 vel;
					Vector2 pos;
					vel.x = sin(DirectX::XM_2PI / DIAGONAL_SHOT_BULLET_NUM * i + 3.14f / 4.0f);
					vel.y = cos(DirectX::XM_2PI / DIAGONAL_SHOT_BULLET_NUM * i + 3.14f / 4.0f);
					pos.x = sin(DirectX::XM_2PI / DIAGONAL_SHOT_BULLET_NUM * i + 3.14f / 4.0f) + transform.position.x;
					pos.y = cos(DirectX::XM_2PI / DIAGONAL_SHOT_BULLET_NUM * i + 3.14f / 4.0f) + transform.position.y;
					vel = vel.Normalize();
					bulletvels.emplace_back(vel);
					bulletposies.emplace_back(pos);
				}

				for (int i = 0, count = 0; i < (int)bullets.size(); ++i)
				{
					if (count >= DIAGONAL_SHOT_BULLET_NUM)
					{
						break;
					}
					if (!bullets[i].GetLiveFlag())
					{
						bullets[i].SetTransform({ {bulletposies[count].x,bulletposies[count].y,0},{},{} });
						bullets[i].SetLiveFlag(true);
						bullets[i].SetRigidbody({ {bulletvels[count].x,bulletvels[count].y,0} });
						++count;
					}
				}
			}
		}


		//弾関係
		//弾が使えない状態ならキャラの座標に持ってくる
		//使える弾ならキャラの向きの方向に飛ばす
		for (auto& b : bullets)
		{
			if (!b.GetLiveFlag())
			{
				if (frameCount % INTERVAL * objectTime == 0)
				{
					if (direction)
					{
						b.SetTransform({ transform.position + Vector3(48,42,0),{},{} });
						b.SetRigidbody({ Vector3(4.0f,0,0) });
					}
					else
					{
						b.SetTransform({ transform.position - Vector3(48,-42,0),{},{} });
						b.SetRigidbody({ Vector3(-4.0f,0,0) });
					}
					b.SetLiveFlag(true);
					break;
				}
			}
		}
		//弾の更新
		for (auto& b : bullets)
		{
			if (!b.GetLiveFlag())
			{
				b.SetTransform({ transform.position,{},{} });
			}
			else
			{
				if (b.GetHitCount() > 0)
				{
					float distance = Vector3::Distance(transform.position, b.GetTransform().position);
					if (distance * distance <= size.x * b.GetSize().x)
					{
						if (!invincivleFlag)
						{
							life -= 0.5f;
							invincivleFlag = true;
							rigidbody.velocity += b.GetRigidbody().velocity + Vector3(0, 8, 0);

						}
						b.SetTransform({ transform.position,{},{} });
						b.SetRigidbody({ Vector3(0,0,0) });
						b.Initialize();
					}
				}
			}
			if (b.GetHitCount() == MAX_HIT_COUNT)
			{
				b.SetTransform({ transform.position,{},{} });
				b.SetRigidbody({ Vector3(0,0,0) });
				b.Initialize();
			}
			b.Update();
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
		//移動
		if (keyboard->CheakHitKey(Key::A))
		{
			rigidbody.velocity += Vector3(-MOVE_POWER, 0, 0);
			angleY = 0;
			direction = false;
		}
		if (keyboard->CheakHitKey(Key::D))
		{
			rigidbody.velocity += Vector3(MOVE_POWER, 0, 0);
			angleY = 3.14f;
			direction = true;
		}

		Vector2 hitChip;
		Vector3 hitChipSize = CollisionMap::GetCurrentMap()->GetChipSize();
		//x軸に移動量を加算し一度判定
		//当たっていたら加算をなかったことにする
		transform.position.x += rigidbody.velocity.x * objectTime;
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS))
		{
			transform.position.x -= rigidbody.velocity.x * objectTime;
			rigidbody.velocity.x = 0;
		}
		//速度ベクトルに重力を加算
		rigidbody.velocity.y -= G * objectTime;
		//y軸に移動量を加算しもう一度判定
		//当たっていたら加算をなかったことにする
		transform.position.y += rigidbody.velocity.y * objectTime;
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS))
		{
			transform.position.y -= rigidbody.velocity.y * objectTime;
			if (jumpFlag && rigidbody.velocity.y < 0)
			{
				jumpFlag = false;
			}
			rigidbody.velocity.y = 0;
		}

		if (invincivleFlag)
		{
			if (invincivleCount > INVINCIBLE_TIME)
			{
				invincivleFlag = false;
				invincivleCount = 0;
			}
			++invincivleCount;
		}
		++frameCount;
		printf("%f\n", life);

		//ライフオブジェクトのuvなどの更新
		for (int i = 0; i < (int)hpCBDatas.size(); ++i)
		{
			float ancerpoint = life - i;
			if (ancerpoint >= 1)
			{
				ancerpoint = 0;
			}
			else if (ancerpoint <= 0)
			{
				ancerpoint = 2;
			}
			else
			{
				ancerpoint = 1;
			}
			hpCBDatas[i]->Map({ DirectX::XMMatrixTranslation(i * 64.0f,0,0),{ancerpoint,0,hpTexSize.y,hpTexSize.y},{hpTexSize.x,hpTexSize.y,0,0} });
		}
	}

	if (life <= 0)
	{
		liveFlag = false;
	}
}

void Player::DrawSprite(ID3D12GraphicsCommandList * cmdList, Dx12_CBVSRVUAVHeap* heap)
{
	cmdList->SetGraphicsRootDescriptorTable(2, heap->GetSRVHandleForGPU(hpTex->GetSRVNumber()));
	for (auto& t : hpCBDatas)
	{
		t->Set(cmdList);
		hpSprite.Draw(cmdList);
	}
}

void Player::Draw(ID3D12GraphicsCommandList * cmdList)
{
	const int INVINCIBLE_FRAME = 5;    //点滅するフレーム = INVINCIBLE_TIME / INVINCIBLE_FRAME;
	cbData->Map({ DirectX::XMMatrixScaling(0.5f,0.5f,0.5f) * DirectX::XMMatrixRotationY(angleY) * DirectX::XMMatrixTranslation(transform.position.x,transform.position.y,transform.position.z) });
	cbData->Set(cmdList);
	if (!invincivleFlag)
	{
		mesh.Draw(cmdList);
	}
	else
	{
		if (invincivleCount % INVINCIBLE_FRAME == 0)
		{
			mesh.Draw(cmdList);
		}
	}
	for (auto& b : bullets)
	{
		b.Draw(cmdList);
	}
}

void Player::SetInputDevice(Keyboard * pKeyboard, Xinput * pCtrler)
{
	keyboard = pKeyboard;
	ctrler = pCtrler;
}

std::vector<Bullet>* Player::GetBullets()
{
	return &bullets;
}
