#include "Boss.h"
#include"CollisionMap.h"

Vector3 Boss::pPos = {};
Boss::Boss()
{
	size = Vector3(128, 128, 0);
}
Boss::~Boss()
{
	delete cbData;
	delete hpTex;
	delete hpCBData;
}
void Boss::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cbData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);
	//メッシュデータのロード
	meshData = {};
	loader->LoadMeshData("Resources/Model/Boss/", "boss", meshData);
	//メッシュの生成
	mesh.Create(device, &meshData);

	SpriteData spriteData = {};
	loader->CreateModelData_Plane(1280, 16, spriteData);
	hpSprite.Create(device, &spriteData);
	hpTex = loader->LoadTexture("Resources/Model/Boss/", "hp.png");
	hpCBData = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1);
}
void Boss::Initialize()
{
	direction = {};
	hp = 30;
	liveFlag = true;
	//Vector2 pos = CollisionMap::GetCurrentMap()->GetBossChip();
	transform.position = Vector3(500, -300, 0);//pos.x * size.x + 32.0f, -(pos.y * size.y + 32.0f), 0);
	cbData->Map(DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z));
}
void Boss::Update()
{

#pragma region マップとの当たり判定

	transform.position.x += rigidbody.velocity.x;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
	{
		transform.position.x -= rigidbody.velocity.x;
		rigidbody.velocity.x = 0;
	}

	transform.position.y += rigidbody.velocity.y;
	if (CollisionMap::CollisionCheckMapChipAndGameObjectFourCorner(*this))
	{
		transform.position.y -= rigidbody.velocity.y;
		rigidbody.velocity.y = 0;
	}

#pragma endregion

#pragma region 動き
	Vector3 dir = pPos - transform.position;
	direction = dir.Normalize();


	rigidbody.velocity.x += direction.x/30;
	rigidbody.velocity.y -= 0.8f;
#pragma endregion

	if (hp < 1)
	{
		liveFlag = false;
	}
	printf("%f\n", hp);
	//ポジションの変更を反映
	cbData->Map(DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z));
	float hpScale = hp / 30.0f;
	hpCBData->Map(DirectX::XMMatrixScaling(hpScale, 1, 1) * DirectX::XMMatrixIdentity());
}

void Boss::DrawSprite(ID3D12GraphicsCommandList * cmdList)
{
	if (liveFlag)
	{
		cmdList->SetGraphicsRootDescriptorTable(2, hpCBData->GetHeap()->GetSRVHandleForGPU(hpTex->GetSRVNumber()));
		hpCBData->Set(cmdList);
		hpSprite.Draw(cmdList);
	}
}

void Boss::Draw(ID3D12GraphicsCommandList * cmdList)
{
	if (liveFlag)
	{
		cmdList->SetGraphicsRootDescriptorTable(2, hpCBData->GetHeap()->GetSRVHandleForGPU(meshData.materialData.texture->GetSRVNumber()));
		cbData->Set(cmdList);
		mesh.Draw(cmdList);
	}
}

bool Boss::Hit(Vector3 pos1, Vector3 pos2, float r)
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
void Boss::DamageHP(float a)
{	
		hp -= a;
}
void Boss::Bounce()
{
	rigidbody.velocity -= direction * 1;
}

