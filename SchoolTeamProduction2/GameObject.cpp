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

//ポジションや回転、スケールを返す
Transform GameObject::GetTransform()
{
	return transform;
}

//速度ベクトルを返す
Rigidbody GameObject::GetRigidbody()
{
	return rigidbody;
}

//生存フラグを返す
bool GameObject::GetLiveFlag()
{
	return liveFlag;
}

//サイズを返す
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
