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
		Vector3 newVelocity = rigidbody.velocity;    //�����Ƃɓ����蔻����������ʂ̎��t���[���p�̑��x�x�N�g��
		Vector2 hitChip;
		const Vector2& hitChipSize = { CollisionMap::GetCurrentMap()->GetChipSize().x,CollisionMap::GetCurrentMap()->GetChipSize().y };
		//x������
		transform.position.x += rigidbody.velocity.x * objectTime;
		//0�ԃ}�b�v�`�b�v�ӊO�Ɠ������Ă��邩�m�F
		if (!CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::AIR))
		{
			//1�ԃ`�b�v�Ɠ������Ă���ꍇ�̓x�N�g���𔽓]������
			if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS))
			{
				transform.position.x -= rigidbody.velocity.x * objectTime;
				newVelocity.x = -rigidbody.velocity.x;
				++hitCountNowFrame;
			}
			//��������`�b�v�Ƃ̓����蔻��
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_LEFT_SLOPE, hitChip))
			{
				//�`�b�v���ɂ���Ȃ�`�b�v�ɂ������O�p�`�̒��_���v�Z���A�O�p�`�ƒe(�_)�œ����蔻������
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					//�������Ă����Ȃ瑬�x�x�N�g�����甽�˃x�N�g�������
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
			//�E������`�b�v�Ƃ̓����蔻��
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
			//���������`�b�v�Ƃ̓����蔻��
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
			//�E�������`�b�v�Ƃ̓����蔻��
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

		//y������
		transform.position.y += rigidbody.velocity.y * objectTime;
		//0�ԃ}�b�v�`�b�v�ӊO�Ɠ������Ă��邩�m�F
		if (!CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::AIR))
		{
			//1�ԃ`�b�v�Ɠ������Ă���ꍇ�̓x�N�g���𔽓]������
			if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS))
			{
				transform.position.y -= rigidbody.velocity.y * objectTime;
				newVelocity.y = -rigidbody.velocity.y;
				++hitCountNowFrame;
			}
			//��������`�b�v�Ƃ̓����蔻��
			else if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::UP_LEFT_SLOPE, hitChip))
			{
				//�`�b�v���ɂ���Ȃ�`�b�v�ɂ������O�p�`�̒��_���v�Z���A�O�p�`�ƒe(�_)�œ����蔻������
				Vector3 a, b, c;
				a = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y), 0);
				b = Vector3((float)hitChip.x * hitChipSize.x + hitChipSize.x, -((float)hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				c = Vector3((float)hitChip.x * hitChipSize.x, -(float)(hitChip.y * hitChipSize.y + hitChipSize.y), 0);
				if (CollisionMap::CollisionCheckTriangleAndGameObject(*this, a, b, c))
				{
					//�������Ă����Ȃ瑬�x�x�N�g�����甽�˃x�N�g�������
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
			//�E������`�b�v�Ƃ̓����蔻��
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
			//���������`�b�v�Ƃ̓����蔻��
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
			//�E�������`�b�v�Ƃ̓����蔻��
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

		//���݃t���[���ł̃q�b�g�J�E���g��1�ȏ�Ȃ�q�b�g����1���₷
		if (hitCountNowFrame > 0)
		{
			++hitCount;
		}

		//���x�x�N�g���̍X�V
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
