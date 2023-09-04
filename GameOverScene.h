#pragma once
#include"Input.h"
#include"SceneState.h"
#include"Sprite.h"
#include"TextureManager.h"
#include"DirectXCommon.h"
#include"Audio.h"

class GameOverScene {
public:
	void Initialize();
	void Update();
	void Draw();

	Scene GetScene() const { return scene_; };

	bool GetState() const { return sceneState_; };
	void Reset();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Scene scene_;
	uint32_t Texture = 0;
	Sprite* GameOver_ = nullptr;

	bool sceneState_ = false;

	uint32_t soundData_ = 0;
	Audio* audio_;


};
