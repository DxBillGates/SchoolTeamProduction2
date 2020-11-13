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

	bullet.LoadAsset(device, heap, loader);
	bullets.resize(16);
	for (auto& b : bullets)
	{
		b.LoadAsset(device, heap, loader);
	}

}

void Player::Initialize()
{
	bullet.Initialize();
	jumpFlag = false;
	direction = true;

	Vector2 pos = CollisionMap::GetCurrentMap()->GetPlayerChip();
	transform.position = Vector3(pos.x * size.x + 32.0f, -(pos.y * size.y + 32.0f), 0);
	objectTime = 1.0f;
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
	const int DIAGONAL_SHOT_BULLET_NUM = 4;    //�΂ߑł��̈�x�ɏo����

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

	//�΂߂���
	if (jumpFlag)
	{
		if (keyboard->KeyPressTrigger(Key::SPACE))
		{
			std::vector<Vector2> bulletvels;
			//�e���΂��������v�Z
			for (int i = 0; i < DIAGONAL_SHOT_BULLET_NUM; ++i)
			{
				Vector2 vel;
				vel.x = sin(DirectX::XM_2PI / DIAGONAL_SHOT_BULLET_NUM * i + 3.14f / 4.0f);
				vel.y = cos(DirectX::XM_2PI / DIAGONAL_SHOT_BULLET_NUM * i + 3.14f / 4.0f);
				vel = vel.Normalize();
				bulletvels.emplace_back(vel);
			}

			for (int i = 0, count = 0; i < (int)bullets.size(); ++i)
			{
				if (count >= DIAGONAL_SHOT_BULLET_NUM)
				{
					break;
				}
				if (!bullets[i].GetLiveFlag())
				{
					bullets[i].SetLiveFlag(true);
					bullets[i].SetRigidbody({ {bulletvels[count].x,bulletvels[count].y,0} });
					++count;
				}
			}
		}
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


	for (auto& b : bullets)
	{
		if (!b.GetLiveFlag())
		{
			b.SetTransform({ transform.position,{},{} });
		}
		if (b.GetHitCount() == MAX_HIT_COUNT)
		{
			b.SetTransform({ transform.position,{},{} });
			b.SetRigidbody({ Vector3(0,0,0) });
			b.Initialize();
		}
		b.Update();
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


	//�e�֌W
	//�e���g���Ȃ���ԂȂ�L�����̍��W�Ɏ����Ă���
	if (!bullet.GetLiveFlag())
	{
		bullet.SetTransform({ {transform.position.x,transform.position.y,0 }, {}, {} });
	}
	//�g����e�Ȃ�L�����̌����̕����ɔ�΂�
	if (!bullet.GetLiveFlag())
	{
		bullet.SetLiveFlag(true);
		if (direction)bullet.SetRigidbody({ Vector3(4.0f,0,0) });
		else bullet.SetRigidbody({ Vector3(-4.0f,0,0) });
	}
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
	for (auto& b : bullets)
	{
		b.Draw(cmdList);
	}
	bullet.Draw(cmdList);
}

void Player::SetInputDevice(Keyboard * pKeyboard, Xinput * pCtrler)
{
	keyboard = pKeyboard;
	ctrler = pCtrler;
}

GameObject * Player::GetBullet()
{
	return &bullet;
}
