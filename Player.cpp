#include "Player.h"
#include "MyMath.h"
#include "cassert"

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera) 
{
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	camera_ = camera;
}

void Player::Update() 
{
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix(); 
}

void Player::Draw() 
{
	model_->Draw(worldTransform_, *camera_);
}

