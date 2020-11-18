#include "MoveEnemy.h"
#include"CollisionMap.h"

Vector3 MoveEnemy::pPos = {};
MoveEnemy::MoveEnemy()
{
	size = Vector3(64, 64, 0);
}
MoveEnemy::~MoveEnemy()
{
	delete cbData;
}
void MoveEnemy::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cbData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);
	//メッシュデータのロード
	meshData = {};
	loader->LoadMeshData("Resources/Model/MoveEnemy/", "moveEnemy", meshData);
	//メッシュの生成
	mesh.Create(device, &meshData);
}
void MoveEnemy::Initialize()
{
	direction = {};
	a = 0;
	hp = 3;
	hitCount = 20;//点滅の長さと無敵時間

	cbData->Map(DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z));
}
void MoveEnemy::Update()
{
	
#pragma region マップとの当たり判定


	transform.position.x += rigidbody.velocity.x;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS) || CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GROUND))
	{
		transform.position.x -= rigidbody.velocity.x;
		rigidbody.velocity.x = 0;
	}

	transform.position.y += rigidbody.velocity.y;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS) || CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GROUND))
	{
		transform.position.y -= rigidbody.velocity.y;
		rigidbody.velocity.y = 0;
	}

#pragma endregion

#pragma region 動き
		Vector3 dir = pPos - transform.position;
		direction = dir.Normalize();

		float range = 1;//ホバリングの振れ幅

		//範囲外の時はホバリングして待機
		if (!Hit(pPos, transform.position, 150))
		{
			rigidbody.velocity.y = sin(a) * range;

			a += 0.05f;//ホバリングの速さ
		}
		else
		{
			//一定の範囲内に入ったら突進
			rigidbody.velocity += direction / 5;
		}
		//体力が0になったら
		if (hp < 1)
		{
			liveFlag = false;
		}

		hitCount++;
#pragma endregion

		//ポジションの変更を反映
		cbData->Map(DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z));
}

void MoveEnemy::Draw(ID3D12GraphicsCommandList * cmdList)
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
	}
}

bool MoveEnemy::Hit(Vector3 pos1, Vector3 pos2, float r)
{
	float sum = r * 2;
	float hitX = pos1.x - pos2.x;
	float hitY = pos1.y - pos2.y;
	float a = hitX * hitX + hitY * hitY;
	if (a < sum*sum)
	{
		return true;
	}
	return false;
}
void MoveEnemy::DamageHP(float a)
{
	if (hitCount > 20)//無敵時間
	{
		hp -= a;
		hitCount = 0;
	}
}
void MoveEnemy::Bounce()
{
	rigidbody.velocity -= direction * 20;
}

