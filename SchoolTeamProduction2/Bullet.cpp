#include "Bullet.h"
#include "CollisionMap.h"

Bullet::Bullet()
	:
	hitCount(0)
{
	size = Vector3(4, 4, 0);
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
	transform = {};
	rigidbody = {};
	liveFlag = false;
	liveFlag = false;
	objectTime = 1.0f;
	hitCount = 0;
}

void Bullet::Update()
{
	if (liveFlag)
	{
		int hitCountNowFrame = 0;
		Vector3 newVelocity = rigidbody.velocity;    //軸ごとに当たり判定をした結果の次フレーム用の速度ベクトル
		Vector2 hitChip;
		const Vector2& hitChipSize = { CollisionMap::GetCurrentMap()->GetChipSize().x,CollisionMap::GetCurrentMap()->GetChipSize().y };
		//x軸判定
		transform.position.x += rigidbody.velocity.x * objectTime;
		//0番マップチップ意外と当たっているか確認
		if (!CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::AIR))
		{
			//1番チップと当たっている場合はベクトルを反転させる
			if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS))
			{
				transform.position.x -= rigidbody.velocity.x * objectTime;
				newVelocity.x = -rigidbody.velocity.x;
				++hitCountNowFrame;
			}
			//左上向きチップとの当たり判定
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_LEFT_SLOPE, hitChip))
			{
				//チップ内にいるならチップにあった三角形の頂点を計算し、三角形と弾(点)で当たり判定を取る
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					//当たっていたなら速度ベクトルから反射ベクトルを作る
					if (rigidbody.velocity.x > 0)
					{
						if (rigidbody.velocity.y > 0)
						{
							newVelocity.x = -rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
						else
						{
							newVelocity.x = rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
					}
					else
					{
						newVelocity.x = -rigidbody.velocity.x;
					}
					transform.position.x -= rigidbody.velocity.x * objectTime;
					++hitCountNowFrame;
				}
			}
			//右上向きチップとの当たり判定
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_RIGHT_SLOPE, hitChip))
			{
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					if (rigidbody.velocity.x > 0)
					{
						newVelocity.x = -rigidbody.velocity.x;
					}
					else
					{
						if (rigidbody.velocity.y > 0)
						{
							newVelocity.x = -rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
						else
						{
							newVelocity.x = -rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
					}
					transform.position.x -= rigidbody.velocity.x * objectTime;
					++hitCountNowFrame;
				}
			}
			//左下向きチップとの当たり判定
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_LEFT_SLOPE, hitChip))
			{
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					if (rigidbody.velocity.x > 0)
					{
						if (rigidbody.velocity.y > 0)
						{
							newVelocity.x = -rigidbody.velocity.y;
							newVelocity.y = -rigidbody.velocity.x;
						}
						else
						{
							newVelocity.x = rigidbody.velocity.y;
							newVelocity.y = -rigidbody.velocity.x;
						}
					}
					else
					{
						newVelocity.x = -rigidbody.velocity.x;
					}
					transform.position.x -= rigidbody.velocity.x * objectTime;
					++hitCountNowFrame;
				}
			}
			//右下向きチップとの当たり判定
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_RIGHT_SLOPE, hitChip))
			{
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					if (rigidbody.velocity.x > 0)
					{
						newVelocity.x = -rigidbody.velocity.x;
					}
					else
					{
						if (rigidbody.velocity.y > 0)
						{
							newVelocity.x = -rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
						else
						{
							newVelocity.x = rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
					}
					transform.position.x -= rigidbody.velocity.x * objectTime;
					++hitCountNowFrame;
				}
			}
		}

		//y軸判定
		transform.position.y += rigidbody.velocity.y * objectTime;
		//0番マップチップ意外と当たっているか確認
		if (!CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::AIR))
		{
			//1番チップと当たっている場合はベクトルを反転させる
			if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS))
			{
				transform.position.y -= rigidbody.velocity.y * objectTime;
				newVelocity.y = -rigidbody.velocity.y;
				++hitCountNowFrame;
			}
			//左上向きチップとの当たり判定
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_LEFT_SLOPE, hitChip))
			{
				//チップ内にいるならチップにあった三角形の頂点を計算し、三角形と弾(点)で当たり判定を取る
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					//当たっていたなら速度ベクトルから反射ベクトルを作る
					if (rigidbody.velocity.y > 0)
					{
						newVelocity.y = -rigidbody.velocity.y;
					}
					else
					{
						if (rigidbody.velocity.x > 0)
						{
							newVelocity.x = rigidbody.velocity.y;
							newVelocity.y = -rigidbody.velocity.x;
						}
						else
						{
							newVelocity.x = rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
					}
					transform.position.y -= rigidbody.velocity.y * objectTime;
					++hitCountNowFrame;
				}
			}
			//右上向きチップとの当たり判定
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_RIGHT_SLOPE, hitChip))
			{
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					if (rigidbody.velocity.y > 0)
					{
						newVelocity.y = -rigidbody.velocity.y;
					}
					else
					{
						if (rigidbody.velocity.x > 0)
						{
							newVelocity.x = -rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
						else
						{
							newVelocity.x = -rigidbody.velocity.y;
							newVelocity.y = -rigidbody.velocity.x;
						}
					}
					transform.position.y -= rigidbody.velocity.y * objectTime;
					++hitCountNowFrame;
				}
			}
			//左下向きチップとの当たり判定
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_LEFT_SLOPE, hitChip))
			{
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					if (rigidbody.velocity.y > 0)
					{
						if (rigidbody.velocity.x > 0)
						{
							newVelocity.x = -rigidbody.velocity.y;
							newVelocity.y = -rigidbody.velocity.x;
						}
						else
						{
							newVelocity.x = -rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
					}
					else
					{
						newVelocity.y = -rigidbody.velocity.y;
					}
					transform.position.y -= rigidbody.velocity.y * objectTime;
					++hitCountNowFrame;
				}
			}
			//右下向きチップとの当たり判定
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::DOWN_RIGHT_SLOPE, hitChip))
			{
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					if (rigidbody.velocity.y > 0)
					{
						if (rigidbody.velocity.x > 0)
						{
							newVelocity.x = rigidbody.velocity.y;
							newVelocity.y = rigidbody.velocity.x;
						}
						else
						{
							newVelocity.x = rigidbody.velocity.y;
							newVelocity.y = -rigidbody.velocity.x;
						}
					}
					else
					{
						newVelocity.y = -rigidbody.velocity.y;
					}
					transform.position.y -= rigidbody.velocity.y * objectTime;
					++hitCountNowFrame;
				}
			}
		}

		//現在フレームでのヒットカウントが1以上ならヒット数を1増やす
		if (hitCountNowFrame > 0)
		{
			++hitCount;
		}

		//速度ベクトルの更新
		rigidbody.velocity = newVelocity;
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
