#include "ShootEnemy.h"
#include"CollisionMap.h"

Vector3 ShootEnemy::pPos = {};
ShootEnemy::ShootEnemy()
{
	size = Vector3(64, 64, 0);
}

ShootEnemy::~ShootEnemy()
{
	delete cbData;
}

void ShootEnemy::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{

	cbData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);
	//メッシュデータのロード
	meshData = {};
	loader->LoadMeshData("Resources/Model/shootEnemy/", "shootEnemy", meshData);
	//メッシュの生成
	mesh.Create(device, &meshData);

	bullet.LoadAsset(device, heap, loader);
}

void ShootEnemy::Initialize()
{
	hp = 3;
	hitCount = 20;
	
	cbData->Map(DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z));
	bullet.Initialize();
}

void ShootEnemy::Update()
{
	
#pragma region 弾の処理

	
		Vector3 direction = pPos - transform.position;
		direction = direction.Normalize();

		if (!bullet.GetLiveFlag())
		{
			bullet.SetLiveFlag(true);
			bullet.SetTransform({ transform.position, {}, {} });
			bullet.SetRigidbody({ Vector3(direction.x * 10,direction.y * 10,0) });
		}
		const int MAX_HIT_COUNT = 1;
		if (bullet.GetHitCount() == MAX_HIT_COUNT)
		{
			bullet.SetTransform({ transform.position,{},{} });
			bullet.SetRigidbody({ Vector3(0,0,0) });
			bullet.Initialize();
		}
		bullet.Update();
#pragma endregion

		//HPが０になったらliveがFALSEになる
		if (hp < 1)
		{
			liveFlag = false;
		}

		hitCount++;
		//printf("%f\n", hp);

		//座標を反映
		cbData->Map(DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z));
	
}

void ShootEnemy::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cbData->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cbData->GetHeap()->GetSRVHandleForGPU(meshData.materialData.texture->GetSRVNumber()));
	if (liveFlag)
	{
		
		if (hitCount < 20)
		{
			if (hitCount % 5 == 0)
			{
				mesh.Draw(cmdList);
			}
		}
		else
		{
			mesh.Draw(cmdList);
		}
		bullet.Draw(cmdList);
	}
}

GameObject * ShootEnemy::GetBullet()
{
	return &bullet;
}

void ShootEnemy::DamageHP(float a)
{
	if (hitCount > 20)
	{
		hp -= a;
		hitCount = 0;
	}
}


