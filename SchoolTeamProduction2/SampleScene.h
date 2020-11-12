#pragma once
#include "Scene.h"
#include "Dx12_Pipeline.h"
#include "Player.h"
#include "PrototypeMap.h"
#include "Particle.h"

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
	DirectX::XMFLOAT3 eyepos;
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 up;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection3D;
	DirectX::XMMATRIX projection2D;
	Dx12_CBuffer<PerMatrix>* perspective;
private:
	Player player;
	PrototypeMap prototypeMap;
	Particle particle;
	float sceneTime;
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

