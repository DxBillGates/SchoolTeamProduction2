#include "Player.h"
#include "CollisionMap.h"

Player::Player()
	:
	jumpFlag(false)
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
}

void Player::Initialize()
{
	transform.position = Vector3(96, -96, 0);
	jumpFlag = false;
}

void Player::Update()
{
	const float G = 0.8f;    //�d��
	const float JUMP_POWER = 16;    //�W�����v��
	const float MOVE_POWER = 8;    //�ړ���

	rigidbody.velocity.x = 0;
	if (keyboard->CheakHitKey(Key::A))
	{
		rigidbody.velocity += Vector3(-MOVE_POWER, 0, 0);
	}
	if (keyboard->CheakHitKey(Key::D))
	{
		rigidbody.velocity += Vector3(MOVE_POWER, 0, 0);
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
	transform.position.x += rigidbody.velocity.x;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
	{
		transform.position.x -= rigidbody.velocity.x;
		rigidbody.velocity.x = 0;
	}

	//���x�x�N�g���ɏd�͂����Z
	rigidbody.velocity.y -= G;
	//y���Ɉړ��ʂ����Z��������x����
	//�������Ă�������Z���Ȃ��������Ƃɂ���
	transform.position.y += rigidbody.velocity.y;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
	{
		transform.position.y -= rigidbody.velocity.y;
		if (jumpFlag && rigidbody.velocity.y < 0)
		{
			jumpFlag = false;
		}
		rigidbody.velocity.y = 0;
	}
}

void Player::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cbData->Map({ DirectX::XMMatrixTranslation(transform.position.x,transform.position.y,transform.position.z) });
	cbData->Set(cmdList);
	mesh.Draw(cmdList);
}

void Player::SetInputDevice(Keyboard * pKeyboard, Xinput * pCtrler)
{
	keyboard = pKeyboard;
	ctrler = pCtrler;
}
