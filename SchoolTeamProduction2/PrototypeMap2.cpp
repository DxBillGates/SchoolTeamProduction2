#include "PrototypeMap2.h"

PrototypeMap2::PrototypeMap2()
{
	//�`�b�v���̃T�C�Y�ݒ�
	size = Vector3(64, 64, 0);
}

PrototypeMap2::~PrototypeMap2()
{
	//�}�b�v�`�b�v�̒萔�o�b�t�@�����ׂĔj��
	for (const auto& x : mapCBData)
	{
		delete x;
	}
}

void PrototypeMap2::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	//�}�b�v�`�b�v�̒萔�o�b�t�@�̐����ƍ��W�X�V
	//�cfor��
	for (int i = 0, index = 0; i < mapChipData.size(); ++i)
	{
		//��for��
		for (int j = 0; j < mapChipData[0].size(); ++j, ++index)
		{
			float x, y;
			x = size.x * j + size.x / 2.0f;
			y = -(size.y * i + size.y / 2.0f);
			mapCBData.emplace_back(new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1));
			mapCBData[index]->Map(DirectX::XMMatrixTranslation(x, y, 0));
		}
	}

	//�}�b�v�`�b�v�p�̃��b�V���𐶐�
	MeshData urData = {};
	loader->LoadMeshData("Resources/Model/", "UP_RIGHT_BLOCK", urData);
	upRightMesh.Create(device, &urData);
	MeshData ulData = {};
	loader->LoadMeshData("Resources/Model/", "UP_LEFT_BLOCK", ulData);
	upLeftMesh.Create(device, &ulData);
	MeshData drData = {};
	loader->LoadMeshData("Resources/Model/", "DOWN_RIGHT_BLOCK", drData);
	downRightMesh.Create(device, &drData);
	MeshData dlData = {};
	loader->LoadMeshData("Resources/Model/", "DOWN_LEFT_BLOCK", dlData);
	downLeftMesh.Create(device, &dlData);
	MeshData cubeData = {};
	loader->LoadMeshData("Resources/Model/", "CUBE", cubeData);
	cubeMesh.Create(device, &cubeData);

	coinManager.LoadAsset(device, heap, loader);
}

void PrototypeMap2::Initialize()
{
	std::vector<Coin>* pCoins = coinManager.GetCoins();
	Vector3 coinSize = (*pCoins)[0].GetSize();

	for (int i = 0; i < (int)coinChip.size(); ++i)
	{
		if ((*pCoins)[i].GetLiveFlag())
		{
			(*pCoins)[i].SetTransform({ {coinChip[i].x * coinSize.x + coinSize.x / 2,-(coinChip[i].y* coinSize.y + coinSize.y / 2),0} , {},{} });
		}
	}

	for (int i = (int)coinChip.size(); i < (int)pCoins->size(); ++i)
	{
		(*pCoins)[i].SetLiveFlag(false);
	}
}

void PrototypeMap2::Draw(ID3D12GraphicsCommandList * cmdList)
{
	coinManager.Update();
	coinManager.Draw(cmdList);
	//�}�b�v�`�b�v�̒萔�o�b�t�@���`��
	for (int i = 0, index = 0; i < (int)mapChipData.size(); ++i)
	{
		for (int j = 0; j < (int)mapChipData[0].size(); ++j, ++index)
		{
			switch (mapChipData[i][j])
			{
			case MapChipData::AIR:
				break;
			case MapChipData::GLASS:
				mapCBData[index]->Set(cmdList);
				cmdList->SetGraphicsRootDescriptorTable(2, mapCBData[0]->GetHeap()->GetSRVHandleForGPU(cGrassTex->GetSRVNumber()));
				cubeMesh.Draw(cmdList);
				break;
			case MapChipData::GROUND:
				mapCBData[index]->Set(cmdList);
				cmdList->SetGraphicsRootDescriptorTable(2, mapCBData[0]->GetHeap()->GetSRVHandleForGPU(cGroundTex->GetSRVNumber()));
				cubeMesh.Draw(cmdList);
				break;
			case MapChipData::UP_LEFT_SLOPE:
				mapCBData[index]->Set(cmdList);
				cmdList->SetGraphicsRootDescriptorTable(2, mapCBData[0]->GetHeap()->GetSRVHandleForGPU(hGrassTex->GetSRVNumber()));
				upRightMesh.Draw(cmdList);
				break;
			case MapChipData::UP_RIGHT_SLOPE:
				mapCBData[index]->Set(cmdList);
				cmdList->SetGraphicsRootDescriptorTable(2, mapCBData[0]->GetHeap()->GetSRVHandleForGPU(hGrassTex->GetSRVNumber()));
				upLeftMesh.Draw(cmdList);
				break;
			case MapChipData::DOWN_LEFT_SLOPE:
				mapCBData[index]->Set(cmdList);
				cmdList->SetGraphicsRootDescriptorTable(2, mapCBData[0]->GetHeap()->GetSRVHandleForGPU(hGrassTex->GetSRVNumber()));
				downRightMesh.Draw(cmdList);
				break;
			case MapChipData::DOWN_RIGHT_SLOPE:
				mapCBData[index]->Set(cmdList);
				cmdList->SetGraphicsRootDescriptorTable(2, mapCBData[0]->GetHeap()->GetSRVHandleForGPU(hGrassTex->GetSRVNumber()));
				downLeftMesh.Draw(cmdList);
				break;
			case MapChipData::NOT_REFLECT_BLOCK:
				mapCBData[index]->Set(cmdList);
				cmdList->SetGraphicsRootDescriptorTable(2, mapCBData[0]->GetHeap()->GetSRVHandleForGPU(cStoneTex->GetSRVNumber()));
				cubeMesh.Draw(cmdList);
				break;
			}
		}
	}
}
