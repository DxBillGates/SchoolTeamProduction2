#pragma once
#include"Dx12_CBVSRVUAVHeap.h"
#include"LoadContents.h"
#include"Vector.h"
#include<d3d12.h>
#include<vector>

enum MapChipData
{
	AIR,
	GLASS,
	GROUND,
	UP_LEFT_SLOPE,
	UP_RIGHT_SLOPE,
	DOWN_LEFT_SLOPE,
	DOWN_RIGHT_SLOPE,
	ENEMY,
	CHECK_POINT,
	MAX_DATA,
};

//���N���X
class Map
{
protected:
	//�}�b�v�`�b�v�f�[�^
	std::vector<std::vector<int>> mapChipData;
	Vector3 size;
public:
	Map();
	virtual ~Map();
	//�}�b�v���[�h�֐�
	void LoadMap(const char* fileName);
	//�������z�֐�
	virtual void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader) = 0;
	virtual void Initialize() = 0;
	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
	//Getter�ASetter
	std::vector<std::vector<int>>* GetMapChipPointer();    //�}�b�v�`�b�v�̃|�C���^��Ԃ�
	std::vector<std::vector<int>> GetMapChipData();    //�}�b�v�`�b�v�̃f�[�^��Ԃ�
	Vector3 GetChipSize();    //�`�b�v��̃T�C�Y��Ԃ�
	Vector2 GetMapSize();    //�}�b�v�̃T�C�Y��Ԃ�
	MapChipData GetMapChipNumber(int x, int y);    //�w��`�b�v�̏���Ԃ�
	void SetMapChip(int x, int y, MapChipData number = AIR);    //�w��`�b�v��ύX
	void SetSize(Vector3 size);    //�`�b�v�̃T�C�Y��ύX
};

