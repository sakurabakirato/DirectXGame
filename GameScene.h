#pragma once
#include"kamataEngine.h"


class GameScene 
{
public:
	~GameScene();
	//初期化
	void Intialize();
	//更新
	void Update();
	//描画
	void Draw();

private:
	uint32_t textureManager_ = 0;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;

};
