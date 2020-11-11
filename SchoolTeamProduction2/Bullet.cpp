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
		transform.position.x += rigidbody.velocity.x;
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
		{
			transform.position.x -= rigidbody.velocity.x;
			rigidbody.velocity = Vector3(-rigidbody.velocity.x, rigidbody.velocity.y, 0);
			++hitCount;
		}
		transform.position.y += rigidbody.velocity.y;
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
		{
			transform.position.y -= rigidbody.velocity.y;
			rigidbody.velocity = Vector3(rigidbody.velocity.x, -rigidbody.velocity.y, 0);
			++hitCount;
		}
		//ヒットしたチップの場所を格納
		std::vector<int> hitChip;
		//左上向きの斜面との当たり判定
		if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_LEFT_SLOPE, hitChip))
		{
			if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, Vector3((float)hitChip[0] * 64 + 63, -((float)hitChip[1] * 64.0f), 0), Vector3((float)hitChip[0] * 64.0f + 64.0f, -((float)hitChip[1] * 64.0f + 64.0f), 0), Vector3((float)hitChip[0] * 64.0f, -(float)(hitChip[1] * 64.0f + 63.0f), 0)))
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
			if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, Vector3((float)hitChip[0] * 60, -((float)hitChip[1] * 60.0f), 0), Vector3((float)hitChip[0] * 68.0f + 68.0f, -((float)hitChip[1] * 68.0f + 68.0f), 0), Vector3((float)hitChip[0] * 64.0f, -(float)(hitChip[1] * 64.0f + 64.0f), 0)))
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

		//int hitCountNowFrame = 0;
		//Vector3 oldVelocity = rigidbody.velocity;
		//Vector3 newVelocity = oldVelocity;
		////軸ごとに当たり判定を取る
		////x軸判定
		//transform.position.x += rigidbody.velocity.x;
		//if (!CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::AIR))
		//{
		//	std::vector<int> hitChip;
		//	//ブロックごとに当たり判定を取る
		//	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS))
		//	{
		//		newVelocity.x = -rigidbody.velocity.x;
		//	}
		//	else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_LEFT_SLOPE, hitChip))
		//	{

		//	}
		//	else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_RIGHT_SLOPE, hitChip))
		//	{
		//		Vector3 a, b, c;
		//		const Vector3& chipSize = CollisionMap::GetCurrentMap()->GetChipSize();
		//		a = Vector3((float)hitChip[0] * chipSize.x, -((float)hitChip[1] * chipSize.y), 0);
		//		b = Vector3((float)hitChip[0] * chipSize.x + chipSize.x, -((float)hitChip[1] * chipSize.y + chipSize.y-1), 0);
		//		c = Vector3((float)hitChip[0] * chipSize.x, -(float)(hitChip[1] * chipSize.y + chipSize.y-1), 0);
		//		if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
		//		{
		//			if (rigidbody.velocity.x < 0)
		//			{
		//				newVelocity.x = -rigidbody.velocity.y;
		//				newVelocity.y = -rigidbody.velocity.x;
		//			}
		//			else
		//			{
		//				newVelocity.x = -rigidbody.velocity.x;
		//			}
		//		}
		//	}
		//	else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_LEFT_SLOPE, hitChip))
		//	{

		//	}
		//	else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_RIGHT_SLOPE, hitChip))
		//	{

		//	}
		//	//当たっているので速度ベクトルの加算をなかったことにする
		//	transform.position.x -= oldVelocity.x;
		//	++hitCountNowFrame;
		//}
		////y軸判定
		//transform.position.y += rigidbody.velocity.y;
		//if (!CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::AIR))
		//{
		//	std::vector<int> hitChip;
		//	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS))
		//	{
		//		newVelocity.y = -rigidbody.velocity.y;
		//	}
		//	else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_LEFT_SLOPE, hitChip))
		//	{

		//	}
		//	else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_RIGHT_SLOPE, hitChip))
		//	{
		//		Vector3 a, b, c;
		//		const Vector3& chipSize = CollisionMap::GetCurrentMap()->GetChipSize();
		//		a = Vector3((float)hitChip[0] * chipSize.x, -((float)hitChip[1] * chipSize.y), 0);
		//		b = Vector3((float)hitChip[0] * chipSize.x + chipSize.x, -((float)hitChip[1] * chipSize.y + chipSize.y - 1), 0);
		//		c = Vector3((float)hitChip[0] * chipSize.x, -(float)(hitChip[1] * chipSize.y + chipSize.y - 1), 0);
		//		if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
		//		{
		//			if (rigidbody.velocity.y < 0)
		//			{
		//				newVelocity.x = -rigidbody.velocity.y;
		//			}
		//			else
		//			{
		//				newVelocity.y = -rigidbody.velocity.y;
		//			}
		//		}
		//	}
		//	else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_LEFT_SLOPE, hitChip))
		//	{

		//	}
		//	else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_RIGHT_SLOPE, hitChip))
		//	{

		//	}
		//	transform.position.y -= oldVelocity.y;
		//	++hitCountNowFrame;
		//}

		//rigidbody.velocity = newVelocity;
		//if (hitCountNowFrame > 0)
		//{
		//	++hitCount;
		//}
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
