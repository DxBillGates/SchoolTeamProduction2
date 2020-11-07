#include "GameObject.h"

GameObject::GameObject()
	:
	transform({}),
	rigidbody({}),
	liveFlag(true),
	size({})
{
}

GameObject::~GameObject()
{
}

//�|�W�V�������]�A�X�P�[����Ԃ�
Transform GameObject::GetTransform()
{
	return transform;
}

//���x�x�N�g����Ԃ�
Rigidbody GameObject::GetRigidbody()
{
	return rigidbody;
}

//�����t���O��Ԃ�
bool GameObject::GetliveFlag()
{
	return liveFlag;
}

//�T�C�Y��Ԃ�
Vector3 GameObject::GetSize()
{
	return size;
}
