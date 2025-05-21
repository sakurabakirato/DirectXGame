#pragma once
#include <KamataEngine.h>

class Player {
public:
	void Initialize(KamataEngine::Model* model,KamataEngine::Camera* camera);

	void Update();

	void Draw();

private:

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	KamataEngine::Camera* camera_ = nullptr;

};