#include "SampleScene.h"
#include "CollisionMap.h"

SampleScene::SampleScene()
{
	//�V�F�[�_�[�ɓn���萔�o�b�t�@�̐���
	perspective = new Dx12_CBuffer<PerMatrix>(device->GetDevice(), heap, 0);
	//�v���C���[�ɃC���v�b�g�f�o�C�X���Z�b�g
	player.SetInputDevice(keyboard, ctrler);
}

SampleScene::~SampleScene()
{
	delete basicMeshShader;
	delete perspective;
}

void SampleScene::LoadAsset()
{
	//�V�F�[�_�[�̃��[�h
	basicMeshShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"BasicMeshVS.hlsl", L"BasicMeshPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV }), { POSITION,TEXCOORD,NORMAL });
	//�v���C���[�̃A�Z�b�g�����[�h
	player.LoadAsset(device->GetDevice(), heap, loader);
	//�v���g�^�C�v�}�b�v�̃A�Z�b�g�Ȃǂ����[�h
	prototypeMap.LoadMap("Resources/MapChip/map.txt");
	prototypeMap.LoadAsset(device->GetDevice(), heap, loader);
}

void SampleScene::Initialize()
{
	nextSceneFlag = false;
	//�V�F�[�_�[�ɓn���萔�o�b�t�@�̏�����
	eyepos = { 0,0,-1000 };
	target = { 0,0,0 };
	up = { 0,1,0 };
	view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
	projection3D = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)gameWnd->GetWndWidth() / gameWnd->GetWndHeight(), 0.1f, 10000);
	projection2D = DirectX::XMMatrixOrthographicOffCenterLH(0, (float)gameWnd->GetWndWidth(), (float)gameWnd->GetWndHeight(), 0, 0, 1);
	perspective->Map({ view,projection3D });

	//�v���C���[��������
	player.Initialize();
	//�v���g�^�C�v�}�b�v��������
	prototypeMap.Initialize();
}

void SampleScene::Update()
{
	//�����蔻����s���}�b�v���Z�b�g
	CollisionMap::SetCurrentMap(&prototypeMap);
	//�v���C���[�̍X�V
	player.Update();
	//�r���[�s��̍X�V
	Vector3 playerPos = player.GetTransform().position;
	eyepos = { playerPos.x,playerPos.y,-1000 };
	target = { playerPos.x,playerPos.y,playerPos.z };
	view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
	perspective->Map({ view,projection3D });
}

void SampleScene::DrawSprite()
{
}

void SampleScene::Draw()
{
	//�V�F�[�_�[�̃Z�b�g
	basicMeshShader->Set(device->GetCmdList());
	//VP�s��̃Z�b�g
	perspective->Set(device->GetCmdList());
	//�v���C���[�̕`��
	player.Draw(device->GetCmdList());
	//�v���g�^�C�v�}�b�v�̕`��
	prototypeMap.Draw(device->GetCmdList());
}

SceneName SampleScene::GetNextSceneName()
{
	return SceneName();
}