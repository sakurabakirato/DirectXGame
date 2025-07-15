#include "Enemy.h"
#include "MyMath.h"
#include "MapChipField.h"
#include "cassert"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;
using namespace MathUtility;


void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	camera_ = camera;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Updata() 
{
	worldTransform_.translation_ += velocity_;

	walkTimer_ += 1.0f / 60.0f;

	worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw() 
{ 
	model_->Draw(worldTransform_, *camera_); 
}
