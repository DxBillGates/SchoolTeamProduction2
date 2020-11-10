#include "Bullet.h"
#include "CollisionMap.h"

Bullet::Bullet()
	:
	hitCount(0)
{
	size = Vector3(16, 16, 0);
}

Bullet::~Bullet()
{
	delete cbData;
}

void Bullet::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	MeshData meshData;
	loader->CreateModelData_Plane(size.x, size.y, meshData);
	mesh.Create(device, &meshData);
	cbData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);
}

void Bullet::Initialize()
{
	hitCount = 0;
	liveFlag = false;
}

void Bullet::Update()
{
	if (liveFlag)
	{
		//ヒット数に応じて速度が上昇
		transform.position += rigidbody.velocity;
		//マップチップとの当たり判定
		//当たっていたらベクトルを反転させヒット数を増やす
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
		{
			transform.position -= rigidbody.velocity;
			rigidbody.velocity = Vector3(-rigidbody.velocity.x, -rigidbody.velocity.y, 0);
			++hitCount;
		}
		//ヒットしたチップの場所を格納
		std::vector<int> hitChip;
		//左上向きの斜面との当たり判定
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_LEFT_SLOPE, hitChip))
		{
			if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, Vector3((float)hitChip[0] * 64 + 63, -((float)hitChip[1] * 64.0f), 0), Vector3((float)hitChip[0] * 64.0f + 63.0f, -((float)hitChip[1] * 64.0f + 63.0f), 0), Vector3((float)hitChip[0] * 64.0f, -(float)(hitChip[1] * 64.0f + 63.0f), 0)))
			{
				transform.position -= rigidbody.velocity;
				if (rigidbody.velocity.x > 0 || rigidbody.velocity.y < 0)
				{
					rigidbody.velocity = Vector3(rigidbody.velocity.y, rigidbody.velocity.x, 0);
				}
				else
				{
					rigidbody.velocity = Vector3(-rigidbody.velocity.x, -rigidbody.velocity.y, 0);
				}
				++hitCount;
			}
		}
		//右上向きの斜面との当たり判定
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_RIGHT_SLOPE, hitChip))
		{
			if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, Vector3((float)hitChip[0] * 64, -((float)hitChip[1] * 64.0f), 0), Vector3((float)hitChip[0] * 64.0f + 63.0f, -((float)hitChip[1] * 64.0f + 63.0f), 0), Vector3((float)hitChip[0] * 64.0f, -(float)(hitChip[1] * 64.0f + 63.0f), 0)))
			{
				transform.position -= rigidbody.velocity;
				if (rigidbody.velocity.x < 0 || rigidbody.velocity.y < 0)
				{
					rigidbody.velocity = Vector3(-rigidbody.velocity.y, -rigidbody.velocity.x, 0);
				}
				else
				{
					rigidbody.velocity = Vector3(-rigidbody.velocity.x, -rigidbody.velocity.y, 0);
				}
				++hitCount;
			}
		}
		//左下向き斜面との当たり判定
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_LEFT_SLOPE, hitChip))
		{
			if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, Vector3((float)hitChip[0] * 64, -((float)hitChip[1] * 64.0f), 0), Vector3((float)hitChip[0] * 64.0f + 63.0f, -((float)hitChip[1] * 64.0f), 0), Vector3((float)hitChip[0] * 64.0f + 63.0f, -(float)(hitChip[1] * 64.0f + 63.0f), 0)))
			{
				transform.position -= rigidbody.velocity;
				if (rigidbody.velocity.x > 0 || rigidbody.velocity.y > 0)
				{
					rigidbody.velocity = Vector3(-rigidbody.velocity.y, -rigidbody.velocity.x, 0);
				}
				else
				{
					rigidbody.velocity = Vector3(-rigidbody.velocity.x, -rigidbody.velocity.y, 0);
				}
				++hitCount;
			}
		}
		//右下向きの斜面との当たり判定
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_RIGHT_SLOPE, hitChip))
		{
			if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, Vector3((float)hitChip[0] * 64, -((float)hitChip[1] * 64.0f), 0), Vector3((float)hitChip[0] * 64.0f + 63.0f, -((float)hitChip[1] * 64.0f), 0), Vector3((float)hitChip[0] * 64.0f, -(float)(hitChip[1] * 64.0f + 63.0f), 0)))
			{
				transform.position -= rigidbody.velocity;
				if (rigidbody.velocity.x < 0 || rigidbody.velocity.y > 0)
				{
					rigidbody.velocity = Vector3(rigidbody.velocity.y, rigidbody.velocity.x, 0);
				}
				else
				{
					rigidbody.velocity = Vector3(-rigidbody.velocity.x, -rigidbody.velocity.y, 0);
				}
				++hitCount;
			}
		}
	}
}

void Bullet::Draw(ID3D12GraphicsCommandList * cmdList)
{
	if (liveFlag)
	{
		cbData->Map({ DirectX::XMMatrixTranslation(transform.position.x,transform.position.y,transform.position.z) });
		cbData->Set(cmdList);
		mesh.Draw(cmdList);
	}
}

int Bullet::GetHitCount()
{
	return hitCount;
}
