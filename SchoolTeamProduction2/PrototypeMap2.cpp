#include "PrototypeMap2.h"

PrototypeMap2::PrototypeMap2()
{
	//チップ一つ一つのサイズ設定
	size = Vector3(64, 64, 0);
}

PrototypeMap2::~PrototypeMap2()
{
	//マップチップの定数バッファをすべて破棄
	for (const auto& x : mapCBData)
	{
		delete x;
	}
}

void PrototypeMap2::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	//マップチップの定数バッファの生成と座標更新
	//縦for文
	for (int i = 0, index = 0; i < mapChipData.size(); ++i)
	{
		//横for文
		for (int j = 0; j < mapChipData[0].size(); ++j, ++index)
		{
			float x, y;
			x = size.x * j + size.x / 2.0f;
			y = -(size.y * i + size.y / 2.0f);
			mapCBData.emplace_back(new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 1));
			mapCBData[index]->Map(DirectX::XMMatrixTranslation(x, y, 0));
		}
	}

	//マップチップ用のメッシュを生成
	MeshData mapChipMeshData = {};
	loader->CreateModelData_Plane(size.x, size.y, mapChipMeshData);
	mapChipMesh.Create(device, &mapChipMeshData);

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

}

void PrototypeMap2::Initialize()
{
}

void PrototypeMap2::Draw(ID3D12GraphicsCommandList * cmdList)
{
	//マップチップの定数バッファ分描画
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
				cubeMesh.Draw(cmdList);
				break;
			case MapChipData::GROUND:
				break;
			case MapChipData::UP_LEFT_SLOPE:
				mapCBData[index]->Set(cmdList);
				upRightMesh.Draw(cmdList);
				break;
			case MapChipData::UP_RIGHT_SLOPE:
				mapCBData[index]->Set(cmdList);
				upLeftMesh.Draw(cmdList);
				break;
			case MapChipData::DOWN_LEFT_SLOPE:
				mapCBData[index]->Set(cmdList);
				downRightMesh.Draw(cmdList);
				break;
			case MapChipData::DOWN_RIGHT_SLOPE:
				mapCBData[index]->Set(cmdList);
				downLeftMesh.Draw(cmdList);
				break;
			}
		}
	}
}
