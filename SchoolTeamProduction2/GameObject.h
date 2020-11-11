#pragma once
#include "Vector.h"
#include "Dx12_CBVSRVUAVHeap.h"
#include "LoadContents.h"
#include <d3d12.h>
struct Transform
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

struct Rigidbody
{
	Vector3 velocity;
};

//ゲームオブジェクト(基底クラス)
class GameObject
{
protected:
	Transform transform;
	Rigidbody rigidbody;
	bool liveFlag;
	Vector3 size;
	float objectTime;
public:
	GameObject();
	virtual ~GameObject();
	virtual void LoadAsset(ID3D12Device* device,Dx12_CBVSRVUAVHeap* heap,LoadContents* loader) = 0;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
	//Getter,Setter
	Transform GetTransform();
	Rigidbody GetRigidbody();
	bool GetLiveFlag();
	Vector3 GetSize();
	float GetObjectTime();
	void SetTransform(const Transform& t);
	void SetRigidbody(const Rigidbody& r);
	void SetLiveFlag(bool b);
	void SetObjectTime(float time);
};
