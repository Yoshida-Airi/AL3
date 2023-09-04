#pragma once
#include"Input.h"
#include"SceneState.h"
#include"Sprite.h"
#include"TextureManager.h"
#include"DirectXCommon.h"
#include"Audio.h"

class TitleScene
{
public:


	void Initialize();
	void Update();
	void Draw();


	void Reset();

	Scene GetScene() const { return scene_; };

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Scene scene_;
	uint32_t titleTexture = 0;
	Sprite* title_ = nullptr;

	uint32_t soundData_ = 0;
	Audio* audio_;

	bool sceneState_ = false;

	bool ifAudio;
};
