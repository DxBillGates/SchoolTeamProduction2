#pragma once
#include "GameObject.h"
#include "Keyboard.h"
#include "Xinput.h"
#include "Dx12_Mesh.h"
#include "Dx12_Sprite.h"
#include "Dx12_Texture2D.h"
#include "Dx12_CBuffer.h"
#include "Bullet.h"
#include "AnimetinStruct.h"

class Player : public GameObject
{
private:
	Keyboard* keyboard;
	Xinput* ctrler;
private:

	Dx12_Mesh mesh;
	MeshData meshData;
	Dx12_CBuffer<DirectX::XMMATRIX>* cbData;
	bool jumpFlag;
	bool direction;
	std::vector<Bullet> bullets;
	float life;
	int frameCount;
	bool invincivleFlag;
	int invincivleCount;
	float angleY;


	Dx12_Sprite hpSprite;
	Dx12_Texture2D* hpTex;
	std::vector<Dx12_CBuffer<AnimetionData>*> hpCBDatas;

	float barriar;
	bool chargeBarriarFlag;
	int chargeBarriarIntervalCount;
	Dx12_Texture2D* bTex;
	std::vector<Dx12_CBuffer<AnimetionData>*> barriarCBDatas;
public:
	Player();
	~Player();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void DrawSprite(ID3D12GraphicsCommandList* cmdList, Dx12_CBVSRVUAVHeap* heap);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetInputDevice(Keyboard* pKeyboard, Xinput* pCtrler);
	std::vector<Bullet>* GetBullets();
	void DamageHP(float a);
};
