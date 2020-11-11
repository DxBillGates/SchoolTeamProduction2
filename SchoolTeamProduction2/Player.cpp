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
}

void Player::Initialize()
{
	bullet.Initialize();
	jumpFlag = false;
	direction = true;

	Vector2 pos = CollisionMap::GetCurrentMap()->GetPlayerChip();
	transform.position = Vector3(pos.x * size.x + 32.0f, -(pos.y * size.y + 32.0f), 0);
	objectTime = 1.0f;
}

void Player::Update()
{
	float G = 0.8f;    //�d��
	float JUMP_POWER = 16;    //�W�����v��
	float MOVE_POWER = 8;    //�ړ���

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


	//�W�����v�t���O��true����Ȃ��Ƃ��ɃX�y�[�X�L�[�������ƃW�����v
	if (keyboard->CheakHitKey(Key::SPACE))
	{
		if (!jumpFlag)
		{
			jumpFlag = true;
			rigidbody.velocity.y = JUMP_POWER;
		}
	}
	//x���Ɉړ��ʂ����Z����x����
	//�������Ă�������Z���Ȃ��������Ƃɂ���
	transform.position.x += rigidbody.velocity.x * objectTime;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
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
	//�e���g���Ȃ���ԂȂ�L�����̍��W�Ɏ����Ă���
	if (!bullet.GetLiveFlag())
	{
		bullet.SetTransform({ {transform.position.x,transform.position.y + 8,0 }, {}, {} });
	}
	//�g����e�Ȃ�L�����̌����̕����ɔ�΂�
	if (!bullet.GetLiveFlag())
	{
		bullet.SetLiveFlag(true);
		if (direction)bullet.SetRigidbody({ Vector3(16.0f,8,0) });
		else bullet.SetRigidbody({ Vector3(-2.5f,8,0) });
	}
	const int MAX_HIT_COUNT = 40;
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

GameObject * Player::GetBullet()
{
	return &bullet;
}
