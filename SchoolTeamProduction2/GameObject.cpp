#include "GameObject.h"

GameObject::GameObject()
	:
	transform({}),
	rigidbody({}),
	liveFlag(true),
	size({}),
	objectTime(1)
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
bool GameObject::GetLiveFlag()
{
	return liveFlag;
}

//�T�C�Y��Ԃ�
Vector3 GameObject::GetSize()
{
	return size;
}

float GameObject::GetObjectTime()
{
	return objectTime;
}

void GameObject::SetTransform(const Transform & t)
{
	transform = t;
}

void GameObject::SetRigidbody(const Rigidbody & r)
{
	rigidbody = r;
}

void GameObject::SetLiveFlag(bool b)
{
	liveFlag = b;
}

void GameObject::SetObjectTime(float time)
{
	objectTime = time;
}
