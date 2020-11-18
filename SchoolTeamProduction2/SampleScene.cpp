#include "SampleScene.h"
#include "CollisionMap.h"

SampleScene::SampleScene()
{
	//�V�F�[�_�[�ɓn���萔�o�b�t�@�̐���
	perspective = new Dx12_CBuffer<PerMatrix>(device->GetDevice(), heap, 0);
	orthograph = new Dx12_CBuffer<DirectX::XMMATRIX>(device->GetDevice(), heap, 0);
	//�v���C���[�ɃC���v�b�g�f�o�C�X���Z�b�g
	player.SetInputDevice(keyboard, ctrler);
}

SampleScene::~SampleScene()
{
	delete basicMeshShader;
	delete particleShader;
	delete textureAnimetionShader;
	delete modelShader;
	delete perspective;
	delete orthograph;
	Map::StaticDeleteAsset();
}

void SampleScene::LoadAsset()
{
	//�V�F�[�_�[�̃��[�h
	basicMeshShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"BasicMeshVS.hlsl", L"BasicMeshPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV }), { POSITION,TEXCOORD,NORMAL });
	particleShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"ParticleVS.hlsl", L"ParticlePS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV }), { POSITION,TEXCOORD,NORMAL });
	textureAnimetionShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"TextureAnimetionVS.hlsl", L"TextureAnimetionPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV,SRV }), { POSITION,TEXCOORD });
	modelShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"ModelVS.hlsl", L"ModelPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV,SRV }), { POSITION,TEXCOORD,NORMAL });
	//�v���C���[�̃A�Z�b�g�����[�h
	player.LoadAsset(device->GetDevice(), heap, loader);
	//�v���g�^�C�v�}�b�v�̃A�Z�b�g�Ȃǂ����[�h
	Coin::StaticLoadAsset(device->GetDevice(), heap, loader);
	prototypeMap.LoadMap("Resources/MapChip/map.txt");
	prototypeMap.LoadAsset(device->GetDevice(), heap, loader);
	CollisionMap::SetCurrentMap(&prototypeMap);
	prototypeMap2.LoadMap("Resources/MapChip/map2.txt");
	prototypeMap2.LoadAsset(device->GetDevice(), heap, loader);
	//coinManager.LoadAsset(device->GetDevice(), heap, loader);

	CheckPoint::StaticLoadAsset(device->GetDevice(), heap, loader);
	checkPoint.LoadAsset(device->GetDevice(), heap, loader);
	enemyManager.LoadAsset(device->GetDevice(), heap, loader);
	Map::StaticLoadAsset(device->GetDevice(), heap, loader);

	boss.LoadAsset(device->GetDevice(), heap, loader);
}

void SampleScene::Initialize()
{
	CollisionMap::GetCurrentMap()->Initialize();
	nextSceneFlag = false;
	//�V�F�[�_�[�ɓn���萔�o�b�t�@�̏�����
	eyepos = { 0,0,-100 };
	target = { 0,0,0 };
	up = { 0,1,0 };
	view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
	projection3D = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)gameWnd->GetWndWidth() / gameWnd->GetWndHeight(), 0.1f, 10000);
	projection2D = DirectX::XMMatrixOrthographicOffCenterLH(0, (float)gameWnd->GetWndWidth(), (float)gameWnd->GetWndHeight(), 0, 0, 1);
	orthograph->Map(projection2D);
	perspective->Map({ view,projection3D });

	//�v���C���[��������
	player.Initialize();
	//�v���g�^�C�v�}�b�v��������
	prototypeMap.Initialize();
	sceneTime = 0.25f;
	//coinManager.Initialize();
	const std::vector<Vector2>& checkPointChips = CollisionMap::GetCurrentMap()->GetCheckPointChips();
	checkPoint.SetTransform({ Vector3(checkPointChips[0].x * checkPoint.GetSize().x + 32,-(checkPointChips[0].y* checkPoint.GetSize().y + 32),0),{},{} });
	enemyManager.Initialize();
	boss.Initialize();
}

void SampleScene::Update()
{
	//std::vector<Coin>* coins = coinManager.GetCoins();
	std::vector<Coin>* coins = CollisionMap::GetCurrentMap()->GetCoinManager()->GetCoins();
	for (int i = 0; i < (int)coins->size(); ++i)
	{
		if ((*coins)[i].GetLiveFlag())
		{
			float distance = Vector3::Distance(player.GetTransform().position, (*coins)[i].GetTransform().position);
			if (distance * distance <= 64 * 64)
			{
				(*coins)[i].SetLiveFlag(false);
				score += 100;
			}
		}
	}
	printf("%d\n", score);
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
			//�����蔻����s���}�b�v���Z�b�g
			CollisionMap::SetCurrentMap(&prototypeMap2);
			Initialize();
		}
	}
	if (!player.GetLiveFlag())
	{
		Initialize();
	}

	//�v���C���[�̍X�V
	player.Update();
	//�r���[�s��̍X�V
	Vector3 playerPos = player.GetTransform().position;
	eyepos = { playerPos.x,playerPos.y,-1000 };
	target = { playerPos.x,playerPos.y,0 };
	view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyepos), DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
	perspective->Map({ view,projection3D });
	//coinManager.Update();
	checkPoint.Update();
	//�G�l�~�[�}�l�[�W���[�̍X�V
	enemyManager.Update();
	boss.Update();
	//�G�l�~�[�N���X�Ńv���C���[��Position���擾
	ShootEnemy::SetPlayerPosition(playerPos);
	MoveEnemy::SetPlayerPosition(playerPos);
	Boss::SetPlayerPosition(playerPos);
#pragma region �e�����蔻��

	std::vector<ShootEnemy*>* shootEnemy = enemyManager.GetShootEnemy();
	std::vector<MoveEnemy*>* moveEnemy = enemyManager.GetMoveEnemy();
	std::vector<Bullet>*playerBullets = player.GetBullets();

	for (auto& x : *shootEnemy)
	{
		x->SetObjectTime(sceneTime);
	}
	for (auto& x : *moveEnemy)
	{
		x->SetObjectTime(sceneTime);
	}

	//�C��G�̓����蔻��
	for (int i = 0; i < shootEnemy->size(); i++)
	{
		if ((*shootEnemy)[i]->GetLiveFlag())
		{
			//�C��G�ƃv���C���[�̓����蔻��
			if (Hit(player.GetTransform().position, (*shootEnemy)[i]->GetTransform().position, player.GetSize().x / 2, (*shootEnemy)[i]->GetSize().x / 2))
			{
				//�������̃_���[�W��^����
				(*shootEnemy)[i]->DamageHP(1);
				//�������̃_���[�W��^����
				player.DamageHP(0.5f);
			}
			//�G�̒e�ƃv���C���[�̔���
			if (Hit(player.GetTransform().position, (*shootEnemy)[i]->GetBullet()->GetTransform().position, player.GetSize().x / 2, (*shootEnemy)[i]->GetBullet()->GetSize().x / 2))
			{
				//�������̃_���[�W��^����
				player.DamageHP(0.5f);
				//�G�e�̏�����
				(*shootEnemy)[i]->GetBullet()->SetTransform({ playerPos,{},{} });
				(*shootEnemy)[i]->GetBullet()->SetRigidbody({ Vector3(0,0,0) });
				(*shootEnemy)[i]->GetBullet()->Initialize();
			}
		}

	}

	//�ˌ��G�ƃv���C���[�̓����蔻��
	for (int i = 0; i < moveEnemy->size(); i++)
	{
		if ((*moveEnemy)[i]->GetLiveFlag())
		{
			if (Hit(player.GetTransform().position, (*moveEnemy)[i]->GetTransform().position, player.GetSize().x / 2, (*moveEnemy)[i]->GetSize().x / 2))
			{
				(*moveEnemy)[i]->Bounce();//�Ԃ�������G���������
				//�������̃_���[�W��^����
				player.DamageHP(0.5f);
			}
		}
	}

	//�{�X�ƃv���C���[�̓����蔻��
	if (Hit(boss.GetTransform().position, player.GetTransform().position, boss.GetSize().x / 2, player.GetSize().x / 2))
	{
		//�����Ă���
		if (boss.GetLiveFlag())
		{
			//�������̃_���[�W��^����
			player.DamageHP(0.5f);
			//�v���C���[�𐁂���΂�*******������*******
			//player.Bounce();
		}
	}


	//�v���C���[�̒e�ƓG�̓����蔻��
	for (int j = 0; j < playerBullets->size(); ++j)
	{
		//�ːi�G
		for (int i = 0; i < moveEnemy->size(); ++i)
		{
			//�����Ă���
			if ((*moveEnemy)[i]->GetLiveFlag())
			{
				if (Hit((*moveEnemy)[i]->GetTransform().position, (*playerBullets)[j].GetTransform().position, (*moveEnemy)[i]->GetSize().x / 2, (*playerBullets)[j].GetSize().x / 2))
				{
					//�������̃_���[�W��^����
					(*moveEnemy)[i]->DamageHP(1);
					//���@�e�̏�����		
					(*playerBullets)[j].SetTransform({ playerPos,{},{} });
					(*playerBullets)[j].SetRigidbody({ Vector3(0,0,0) });
					(*playerBullets)[j].Initialize();
				}
			}
		}
		//�C��G
		for (int i = 0; i < shootEnemy->size(); ++i)
		{
			//�����Ă���
			if ((*shootEnemy)[i]->GetLiveFlag())
			{
				if (Hit((*shootEnemy)[i]->GetTransform().position, (*playerBullets)[j].GetTransform().position, (*shootEnemy)[i]->GetSize().x / 2, (*playerBullets)[j].GetSize().x / 2))
				{
					//�������̃_���[�W��^����
					(*shootEnemy)[i]->DamageHP(1);
					//���@�e�̏�����
					(*playerBullets)[j].SetTransform({ playerPos,{},{} });
					(*playerBullets)[j].SetRigidbody({ Vector3(0,0,0) });
					(*playerBullets)[j].Initialize();
				}
			}
		}

		//�{�X
		if (Hit(boss.GetTransform().position, (*playerBullets)[j].GetTransform().position, boss.GetSize().x / 2, (*playerBullets)[j].GetSize().x / 2))
		{
			//�����Ă���
			if (boss.GetLiveFlag())
			{
				//�������̃_���[�W��^����
				boss.DamageHP(1);
				//���@�e�̏�����
				(*playerBullets)[j].SetTransform({ playerPos,{},{} });
				(*playerBullets)[j].SetRigidbody({ Vector3(0,0,0) });
				(*playerBullets)[j].Initialize();
			}
		}
	}



#pragma endregion
}

void SampleScene::DrawSprite()
{
	ID3D12GraphicsCommandList* pCmdList = device->GetCmdList();
	textureAnimetionShader->Set(pCmdList);
	orthograph->Set(pCmdList);
	player.DrawSprite(pCmdList, heap);
	boss.DrawSprite(pCmdList);
}

void SampleScene::Draw()
{
	ID3D12GraphicsCommandList* pCmdList = device->GetCmdList();
	//particleShader->Set(device->GetCmdList());
	////***ParticleManagerDraw
	//particle.Draw(device->GetCmdList());
	////***ParticleManagerDraw
	modelShader->Set(pCmdList);
	perspective->Set(pCmdList);
	//coinManager.Draw(pCmdList);
	player.Draw(pCmdList);
	CollisionMap::GetCurrentMap()->Draw(pCmdList);
	boss.Draw(device->GetCmdList());
	enemyManager.Draw(pCmdList);
	//�V�F�[�_�[�̃Z�b�g
	basicMeshShader->Set(pCmdList);
	//VP�s��̃Z�b�g
	perspective->Set(pCmdList);
	//�v���C���[�̕`��
	checkPoint.Draw(pCmdList);
	//�v���g�^�C�v�}�b�v�̕`��
	//prototypeMap.Draw(pCmdList);
}

SceneName SampleScene::GetNextSceneName()
{
	return SceneName();
}

bool SampleScene::Hit(Vector3 pos1, Vector3 pos2, float r1, float r2)
{
	float sum = r1 + r2;
	float hitX = pos1.x - pos2.x;
	float hitY = pos1.y - pos2.y;
	float a = hitX * hitX + hitY * hitY;
	if (a < sum*sum)
	{
		return true;
	}
	return false;
}
