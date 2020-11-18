#pragma once
#include "GameObject.h"
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"
#include"PrototypeMap.h"
#include"Bullet.h"
#include"Player.h"
class Boss : public GameObject
{
private:
	float hp;
	Vector3 direction;
	bool Hit(Vector3 pos1, Vector3 pos2, float r);
private:
	Dx12_Mesh mesh;
	MeshData meshData;
	Dx12_CBuffer<DirectX::XMMATRIX>* cbData;
	Bullet bullet;
	static Vector3 pPos;

	Dx12_Sprite hpSprite;
	Dx12_CBuffer<DirectX::XMMATRIX>* hpCBData;
	Dx12_Texture2D* hpTex;
public:
	Boss();
	~Boss();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void DrawSprite(ID3D12GraphicsCommandList* cmdList);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Bounce();	//è’ìÀÇµÇΩÇ∆Ç´Ç…íµÇÀï‘ÇÈ
	void DamageHP(float a);
	static void SetPlayerPosition(const Vector3& pos)
	{
		pPos = pos;
	}
};
