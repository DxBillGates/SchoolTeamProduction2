#include"PrototypeMap.h"

PrototypeMap::PrototypeMap()
{
	//�`�b�v���̃T�C�Y�ݒ�
	size = Vector3(64, 64, 0);
}

PrototypeMap::~PrototypeMap()
{
	//�}�b�v�`�b�v�̒萔�o�b�t�@�����ׂĔj��
	for (const auto& x : mapCBData)
	{
		delete x;
	}
}

void PrototypeMap::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
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
	MeshData mapChipMeshData = {};
	loader->CreateModelData_Plane(size.x, size.y, mapChipMeshData);
	mapChipMesh.Create(device, &mapChipMeshData);
}

void PrototypeMap::Initialize()
{
}

void PrototypeMap::Draw(ID3D12GraphicsCommandList * cmdList)
{
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
				mapChipMesh.Draw(cmdList);
				break;
			case MapChipData::GROUND:
				break;
			case MapChipData::UP_LEFT_SLOPE:
				mapCBData[index]->Set(cmdList);
				mapChipMesh.Draw(cmdList);
				break;
			case MapChipData::DOWN_LEFT_SLOPE:
				mapCBData[index]->Set(cmdList);
				mapChipMesh.Draw(cmdList);
				break;
			case MapChipData::DOWN_RIGHT_SLOPE:
				mapCBData[index]->Set(cmdList);
				mapChipMesh.Draw(cmdList);
				break;
			}
		}
	}
}
