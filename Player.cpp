#include "Player.h"
#include "cassert"


using namespace KamataEngine;

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) 
{
	assert(model);

	textureHandle_ = textureHandle;

	model_ = model;

	worldTransform_.Initialize();

	camera_ = camera;
}

void Player::Update() { 
	worldTransform_.TransferMatrix(); }

void Player::Draw() 
{
	model_->Draw(worldTransform_, *camera_, textureHandle_);
}

