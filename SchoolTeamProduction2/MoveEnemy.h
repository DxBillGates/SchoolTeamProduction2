#pragma once
#include "GameObject.h"
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"
#include"PrototypeMap.h"
#include"Bullet.h"
#include"Player.h"
class MoveEnemy : public GameObject
{
private:
	float hp;
	float a;
	int hitCount;
	Vector3 direction;
	bool Hit(Vector3 pos1, Vector3 pos2, float r);
private:
	MeshData meshData;
	Dx12_Mesh mesh;
	Dx12_CBuffer<DirectX::XMMATRIX>* cbData;
	Bullet bullet;
	static Vector3 pPos;
public:
	MoveEnemy();
	~MoveEnemy();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Bounce();	//è’ìÀÇµÇΩÇ∆Ç´Ç…íµÇÀï‘ÇÈ
	void DamageHP(float a);
	static void SetPlayerPosition(const Vector3& pos)
	{
		pPos = pos;
	}
};
