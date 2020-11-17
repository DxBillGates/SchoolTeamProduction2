#include "SampleScene.h"
#include "CollisionMap.h"

SampleScene::SampleScene()
{
	//シェーダーに渡す定数バッファの生成
	perspective = new Dx12_CBuffer<PerMatrix>(device->GetDevice(), heap, 0);
	orthograph = new Dx12_CBuffer<DirectX::XMMATRIX>(device->GetDevice(), heap, 0);
	//プレイヤーにインプットデバイスをセット
	player.SetInputDevice(keyboard, ctrler);
}

SampleScene::~SampleScene()
{
	delete basicMeshShader;
	delete particleShader;
	delete textureAnimetionShader;
	delete perspective;
	delete orthograph;
}

void SampleScene::LoadAsset()
{
	//シェーダーのロード
	basicMeshShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"BasicMeshVS.hlsl", L"BasicMeshPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV }), { POSITION,TEXCOORD,NORMAL });
	particleShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"ParticleVS.hlsl", L"ParticlePS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV }), { POSITION,TEXCOORD,NORMAL });
	textureAnimetionShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"TextureAnimetionVS.hlsl", L"TextureAnimetionPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV,SRV }), { POSITION,TEXCOORD });
	//プレイヤーのアセットをロード
	player.LoadAsset(device->GetDevice(), heap, loader);
	//プロトタイプマップのアセットなどをロード
	prototypeMap.LoadMap("Resources/MapChip/map.txt");
	prototypeMap.LoadAsset(device->GetDevice(), heap, loader);
	CollisionMap::SetCurrentMap(&prototypeMap);
	prototypeMap2.LoadMap("Resources/MapChip/map2.txt");
	prototypeMap2.LoadAsset(device->GetDevice(), heap, loader);
	coinManager.LoadAsset(device->GetDevice(), heap, loader);

	CheckPoint::StaticLoadAsset(device->GetDevice(), heap, loader);
	checkPoint.LoadAsset(device->GetDevice(), heap, loader);
}

void SampleScene::Initialize()
{
	nextSceneFlag = false;
	//シェーダーに渡す定数バッファの初期化
	eyepos = { 0,0,-100 };
	target = { 0,0,0 };
	up = { 0,1,0 };
	view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
	projection3D = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)gameWnd->GetWndWidth() / gameWnd->GetWndHeight(), 0.1f, 10000);
	projection2D = DirectX::XMMatrixOrthographicOffCenterLH(0, (float)gameWnd->GetWndWidth(), (float)gameWnd->GetWndHeight(), 0, 0, 1);
	orthograph->Map(projection2D);
	perspective->Map({ view,projection3D });

	//プレイヤーを初期化
	player.Initialize();
	//プロトタイプマップを初期化
	prototypeMap.Initialize();
	sceneTime = 0.25f;
	coinManager.Initialize();
	const std::vector<Vector2>& checkPointChips = CollisionMap::GetCurrentMap()->GetCheckPointChips();
	checkPoint.SetTransform({ Vector3(checkPointChips[0].x * checkPoint.GetSize().x + 32,-(checkPointChips[0].y* checkPoint.GetSize().y + 32),0),{},{} });
}

void SampleScene::Update()
{
	std::vector<Coin>* coins = coinManager.GetCoins();
	for (int i = 0; i < (int)coins->size(); ++i)
	{
		if ((*coins)[i].GetLiveFlag())
		{
			float distance = Vector3::Distance(player.GetTransform().position, (*coins)[i].GetTransform().position);
			if (distance * distance <= 64 * 64)
			{
				(*coins)[i].SetLiveFlag(false);
			}
		}
	}
	static bool f = false;
	if (keyboard->KeyPressTrigger(Key::D1))
	{
		if (f)
		{
			f = false;
			CollisionMap::SetCurrentMap(&prototypeMap);
			Initialize();
		}
		else
		{
			f = true;
			//当たり判定を行うマップをセット
			CollisionMap::SetCurrentMap(&prototypeMap2);
			Initialize();
		}
	}
	if (!player.GetLiveFlag())
	{
		Initialize();
	}

	//プレイヤーの更新
	player.Update();
	//ビュー行列の更新
	Vector3 playerPos = player.GetTransform().position;
	eyepos = { playerPos.x,playerPos.y,-1000 };
	target = { playerPos.x,playerPos.y,0 };
	view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
	perspective->Map({ view,projection3D });
	coinManager.Update();
	checkPoint.Update();
	//printf("シーンの固有時間 : %f\n", sceneTime);
	//particle.Update();
}

void SampleScene::DrawSprite()
{
	ID3D12GraphicsCommandList* pCmdList = device->GetCmdList();
	textureAnimetionShader->Set(pCmdList);
	orthograph->Set(pCmdList);
	player.DrawSprite(pCmdList, heap);
}

void SampleScene::Draw()
{
	ID3D12GraphicsCommandList* pCmdList = device->GetCmdList();
	//particleShader->Set(device->GetCmdList());
	////***ParticleManagerDraw
	//particle.Draw(device->GetCmdList());
	////***ParticleManagerDraw

	//シェーダーのセット
	basicMeshShader->Set(pCmdList);
	//VP行列のセット
	perspective->Set(pCmdList);
	//プレイヤーの描画
	player.Draw(pCmdList);
	coinManager.Draw(pCmdList);
	checkPoint.Draw(pCmdList);
	//プロトタイプマップの描画
	//prototypeMap.Draw(pCmdList);
	CollisionMap::GetCurrentMap()->Draw(pCmdList);
}

SceneName SampleScene::GetNextSceneName()
{
	return SceneName();
}
