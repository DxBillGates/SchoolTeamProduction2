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
	//���b�V���f�[�^�̃��[�h
	meshData = {};
	loader->LoadMeshData("Resources/Model/MoveEnemy/", "moveEnemy", meshData);
	//���b�V���̐���
	mesh.Create(device, &meshData);
}
void MoveEnemy::Initialize()
{
	direction = {};
	a = 0;
	hp = 3;
	hitCount = 20;//�_�ł̒����Ɩ��G����

	cbData->Map(DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z));
}
void MoveEnemy::Update()
{
	
#pragma region �}�b�v�Ƃ̓����蔻��


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

#pragma region ����
		Vector3 dir = pPos - transform.position;
		direction = dir.Normalize();

		float range = 1;//�z�o�����O�̐U�ꕝ

		//�͈͊O�̎��̓z�o�����O���đҋ@
		if (!Hit(pPos, transform.position, 150))
		{
			rigidbody.velocity.y = sin(a) * range;

			a += 0.05f;//�z�o�����O�̑���
		}
		else
		{
			//���͈͓̔��ɓ�������ːi
			rigidbody.velocity += direction / 5;
		}
		//�̗͂�0�ɂȂ�����
		if (hp < 1)
		{
			liveFlag = false;
		}

		hitCount++;
#pragma endregion

		//�|�W�V�����̕ύX�𔽉f
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
	if (hitCount > 20)//���G����
	{
		hp -= a;
		hitCount = 0;
	}
}
void MoveEnemy::Bounce()
{
	rigidbody.velocity -= direction * 20;
}

