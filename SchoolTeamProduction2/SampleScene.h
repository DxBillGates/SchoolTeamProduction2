#pragma once
#include "Scene.h"
#include "Dx12_Pipeline.h"
#include "Player.h"
#include "PrototypeMap.h"
#include "PrototypeMap2.h"
#include "Particle.h"
#include "CoinManager.h"
#include "CheckPoint.h"
#include "EnemyManager.h"
#include "Boss.h"

struct PerMatrix
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

class SampleScene : public Scene
{
private:
	Dx12_Pipeline* basicMeshShader;
	Dx12_Pipeline* particleShader;
	Dx12_Pipeline* textureAnimetionShader;
	Dx12_Pipeline* modelShader;
	DirectX::XMFLOAT3 eyepos;
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 up;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection3D;
	DirectX::XMMATRIX projection2D;
	Dx12_CBuffer<PerMatrix>* perspective;
	Dx12_CBuffer<DirectX::XMMATRIX>* orthograph;
private:
	Player player;
	PrototypeMap prototypeMap;
	PrototypeMap2 prototypeMap2;
	Particle particle;
	float sceneTime;
	CheckPoint checkPoint;
	int score;
	EnemyManager enemyManager;
	Boss boss;
public:
	SampleScene();
	~SampleScene();
	void LoadAsset();
	void Initialize();
	void Update();
	void DrawSprite();
	void Draw();
	SceneName GetNextSceneName();
	bool Hit(Vector3 pos1, Vector3 pos2, float r1, float r2);
};

