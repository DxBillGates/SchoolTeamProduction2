#include "SampleScene.h"
#include "CollisionMap.h"

SampleScene::SampleScene()
{
	//シェーダーに渡す定数バッファの生成
	perspective = new Dx12_CBuffer<PerMatrix>(device->GetDevice(), heap, 0);
	//プレイヤーにインプットデバイスをセット
	player.SetInputDevice(keyboard, ctrler);
}

SampleScene::~SampleScene()
{
	delete basicMeshShader;
	delete particleShader;
	delete perspective;
}

void SampleScene::LoadAsset()
{
	//シェーダーのロード
	basicMeshShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"BasicMeshVS.hlsl", L"BasicMeshPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV }), { POSITION,TEXCOORD,NORMAL });
	particleShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"ParticleVS.hlsl", L"ParticlePS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV }), { POSITION,TEXCOORD,NORMAL });
	//プレイヤーのアセットをロード
	player.LoadAsset(device->GetDevice(), heap, loader);
	//プロトタイプマップのアセットなどをロード
	prototypeMap.LoadMap("Resources/MapChip/map.txt");
	prototypeMap.LoadAsset(device->GetDevice(), heap, loader);

	//particle.LoadAsset(device->GetDevice(), heap, loader);
	//particle.LoadAsset_Mesh(device->GetDevice(), heap, loader);
}

void SampleScene::Initialize()
{
	nextSceneFlag = false;
	//シェーダーに渡す定数バッファの初期化
	eyepos = { 0,0,-1000 };
	target = { 0,0,0 };
	up = { 0,1,0 };
	view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
	projection3D = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)gameWnd->GetWndWidth() / gameWnd->GetWndHeight(), 0.1f, 10000);
	projection2D = DirectX::XMMatrixOrthographicOffCenterLH(0, (float)gameWnd->GetWndWidth(), (float)gameWnd->GetWndHeight(), 0, 0, 1);
	perspective->Map({ view,projection3D });

	CollisionMap::SetCurrentMap(&prototypeMap);
	//プレイヤーを初期化
	player.Initialize();
	//プロトタイプマップを初期化
	prototypeMap.Initialize();
	sceneTime = 0.25f;
	//particle.Initialize();
}

void SampleScene::Update()
{
	if (keyboard->CheakHitKey(Key::UP))
	{
		sceneTime += 0.01f;
	}
	if (keyboard->CheakHitKey(Key::DOWN))
	{
		sceneTime -= 0.01f;
	}
	if (sceneTime <= 0)
	{
		sceneTime = 0.01f;
	}
	if (keyboard->CheakHitKey(Key::I))
	{
		Initialize();
	}
	GameObject* p = player.GetBullet();
	p->SetObjectTime(sceneTime);
	//当たり判定を行うマップをセット
	CollisionMap::SetCurrentMap(&prototypeMap);
	//プレイヤーの更新
	player.Update();
	//ビュー行列の更新
	Vector3 playerPos = player.GetTransform().position;
	eyepos = { playerPos.x,playerPos.y,-1000 };
	target = { playerPos.x,playerPos.y,0 };
	view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
	perspective->Map({ view,projection3D });
	//printf("シーンの固有時間 : %f\n", sceneTime);
	//particle.Update();
}

void SampleScene::DrawSprite()
{
}

void SampleScene::Draw()
{
	//particleShader->Set(device->GetCmdList());
	////***ParticleManagerDraw
	//particle.Draw(device->GetCmdList());
	////***ParticleManagerDraw

	//シェーダーのセット
	basicMeshShader->Set(device->GetCmdList());
	//VP行列のセット
	perspective->Set(device->GetCmdList());
	//プレイヤーの描画
	player.Draw(device->GetCmdList());
	//プロトタイプマップの描画
	prototypeMap.Draw(device->GetCmdList());
}

SceneName SampleScene::GetNextSceneName()
{
	return SceneName();
}
