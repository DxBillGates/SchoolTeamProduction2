#pragma once
#include "GameObject.h"
#include "Keyboard.h"
#include "Xinput.h"
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"
#include "Bullet.h"

class Player : public GameObject
{
private:
	Keyboard* keyboard;
	Xinput* ctrler;
private:
	Dx12_Mesh mesh;
	Dx12_CBuffer<DirectX::XMMATRIX>* cbData;
	bool jumpFlag;
	bool direction;
	Bullet bullet;
	std::vector<Bullet> bullets;
public:
	Player();
	~Player();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetInputDevice(Keyboard* pKeyboard, Xinput* pCtrler);
	GameObject* GetBullet();
};
