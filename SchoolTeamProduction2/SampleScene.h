#pragma once
#include "Scene.h"
#include "Dx12_Pipeline.h"
#include "Player.h"
#include "PrototypeMap.h"
#include "PrototypeMap2.h"
#include "Particle.h"
#include "CoinManager.h"
#include "CheckPoint.h"

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
	CoinManager coinManager;
	CheckPoint checkPoint;
	int score;
public:
	SampleScene();
	~SampleScene();
	void LoadAsset();
	void Initialize();
	void Update();
	void DrawSprite();
	void Draw();
	SceneName GetNextSceneName();
};

