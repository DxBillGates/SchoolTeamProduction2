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
}

void Player::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	//���b�V���f�[�^�̃��[�h
	MeshData meshData = {};
	loader->CreateModelData_Plane(size.x, size.y, meshData);
	//���b�V���̐���
	mesh.Create(device, &meshData);
	cbData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);

	bullets.resize(32);
	for (auto& b : bullets)
	{
		b.LoadAsset(device, heap, loader);
	}

}

void Player::Initialize()
{
	jumpFlag = false;
	direction = true;

	Vector2 pos = CollisionMap::GetCurrentMap()->GetPlayerChip();
	transform.position = Vector3(pos.x * size.x + 32.0f, -(pos.y * size.y + 32.0f), 0);
	objectTime = 1.0f;
	frameCount = 0;
	life = 10;
	invincivleFlag = false;
	invincivleCount = 0;
	for (auto& b : bullets)
	{
		b.Initialize();
	}
}

void Player::Update()
{
	const float G = 0.8f;    //�d��
	const float JUMP_POWER = 16;    //�W�����v��
	const float MOVE_POWER = 8;    //�ړ���
	const int MAX_HIT_COUNT = 4;    //�e�̍ő�q�b�g��
	const int DIAGONAL_SHOT_BULLET_NUM = 8;    //�΂ߑł��̈�x�ɏo����
	const int INTERVAL = 60;    //�e�𔭎˂���Ԋu
	const int INVINCIBLE_TIME = 30;    //���G����

	//�΂߂���
	if (jumpFlag)
	{
		if (keyboard->KeyPressTrigger(Key::SPACE))
		{
			std::vector<Vector2> bulletvels;
			std::vector<Vector2> bulletposies;
			//�e���΂��������v�Z
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


	//�e�֌W
	//�e���g���Ȃ���ԂȂ�L�����̍��W�Ɏ����Ă���
	//�g����e�Ȃ�L�����̌����̕����ɔ�΂�
	for (auto& b : bullets)
	{
		if (!b.GetLiveFlag())
		{
			if (frameCount % INTERVAL * objectTime == 0)
			{
				if (direction)
				{
					b.SetTransform({ transform.position + Vector3(48,0,0),{},{} });
					b.SetRigidbody({ Vector3(4.0f,0,0) });
				}
				else
				{
					b.SetTransform({ transform.position - Vector3(48,0,0),{},{} });
					b.SetRigidbody({ Vector3(-4.0f,0,0) });
				}
				b.SetLiveFlag(true);
				break;
			}
		}
	}
	//�e�̍X�V
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

	//�W�����v�t���O��true����Ȃ��Ƃ��ɃX�y�[�X�L�[�������ƃW�����v
	if (keyboard->CheakHitKey(Key::SPACE))
	{
		if (!jumpFlag)
		{
			jumpFlag = true;
			rigidbody.velocity.y = JUMP_POWER;
		}
	}
	//�ړ�
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

	//x���Ɉړ��ʂ����Z����x����
	//�������Ă�������Z���Ȃ��������Ƃɂ���
	transform.position.x += rigidbody.velocity.x * objectTime;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this, MapChipData::GLASS))
	{
		transform.position.x -= rigidbody.velocity.x * objectTime;
		rigidbody.velocity.x = 0;
	}
	//���x�x�N�g���ɏd�͂����Z
	rigidbody.velocity.y -= G * objectTime;
	//y���Ɉړ��ʂ����Z��������x����
	//�������Ă�������Z���Ȃ��������Ƃɂ���
	transform.position.y += rigidbody.velocity.y * objectTime;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
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
}

void Player::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cbData->Map({ DirectX::XMMatrixTranslation(transform.position.x,transform.position.y,transform.position.z) });
	cbData->Set(cmdList);
	if (!invincivleFlag)
	{
		mesh.Draw(cmdList);
	}
	else
	{
		if (invincivleCount % 2)
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
