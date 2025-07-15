#pragma once
#include <KamataEngine.h>

class MapChipField;

class Enemy 
{
public:

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Updata();

	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	MapChipField* mapChipField_ = nullptr;

	static inline const float kWalkSpeed = 0.01f;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kWalkMotionAngleStart = 0.0f;

	static inline const float kWalkMotionAngleEnd = 5.0f;

	static inline const float kWalkMotionTime = 1.0f;

	float walkTimer_ = 0.0f;



};
