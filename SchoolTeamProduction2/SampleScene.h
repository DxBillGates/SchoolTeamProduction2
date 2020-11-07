#pragma once
#include "Scene.h"

class SampleScene : public Scene
{
private:
private:
public:
	SampleScene();
	~SampleScene();
	void LoadAsset();
	void Initialize();
	void Update();
	void DrawSprite();
	void Draw();
	SceneName GetNextSceneName();
};

