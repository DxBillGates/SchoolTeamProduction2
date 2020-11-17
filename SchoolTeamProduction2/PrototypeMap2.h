#pragma once
#include"Map.h"
#include"Dx12_CBuffer.h"
#include"Dx12_Mesh.h"
#include<DirectXMath.h>

class PrototypeMap2 : public Map
{
private:
	Dx12_Mesh upRightMesh;
	Dx12_Mesh upLeftMesh;
	Dx12_Mesh downRightMesh;
	Dx12_Mesh downLeftMesh;
	Dx12_Mesh cubeMesh;

	Dx12_Mesh mapChipMesh;
	std::vector<Dx12_CBuffer<DirectX::XMMATRIX>*> mapCBData;
public:
	PrototypeMap2();
	~PrototypeMap2();
	//ƒˆ‰¼‘zŠÖ”ŒQAâ‘ÎÀ‘•!
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Draw(ID3D12GraphicsCommandList* cmdList);
};
