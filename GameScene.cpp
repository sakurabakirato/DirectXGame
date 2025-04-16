#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() 
{ 
	delete model_; 
}

void GameScene::Intialize() { 
	textureManager_ = TextureManager::Load("player_4.png"); 

	model_ = Model::Create();

	worldTransform_.Initialize();
	camera_.Initialize();
}

void GameScene::Update() 
{

}

void GameScene::Draw() 
{ 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance(); 

	Model::PreDraw(dxCommon->GetCommandList());

	model_->Draw(worldTransform_, camera_, textureManager_);

	Model::PostDraw();
}

