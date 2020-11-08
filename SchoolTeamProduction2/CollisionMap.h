#pragma once
#include"GameObject.h"
#include"Map.h"

class CollisionMap
{
	static Map* currentMap;
public:
	static void SetCurrentMap(Map* pMap);
	static bool CollisionCheckMapChipAndGameObject(GameObject& gameObject);    //�Q�[���I�u�W�F�N�g������}�b�v�`�b�v�̃^�C���𔻒肵�P���ȓ����蔻����s��
	static bool CollisionCheckMapChipAndGameObjectFourCorner(GameObject& gameObject, MapChipData chip = GLASS);    //�Q�[���I�u�W�F�N�g�̎l���ƃ`�b�v�𔻒肵�ǂ����������Ă��邩��Ԃ�
	static bool CollisionCheckMapChipAndGameObjectFourCorner(GameObject& gameObject, MapChipData chip,std::vector<int>& m);
	static bool CollisionCheckTriangleAndGameObject(GameObject& gameObject, Vector3 a, Vector3 b, Vector3 c);
};

