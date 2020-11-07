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
bool GameObject::GetliveFlag()
{
	return liveFlag;
}

//サイズを返す
Vector3 GameObject::GetSize()
{
	return size;
}
