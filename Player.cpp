#define NOMINMAX
#include "Player.h"
#include "MyMath.h"
#include "MapChipField.h"
#include "cassert"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;
using namespace MathUtility;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
	assert(model);

	model_ = model;


	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	camera_ = camera;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::InputMove() 
{
	if (onGround_) 
	{
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) 
		{
			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) 
			{
				if (lrDirection_ != LRDirection::kRight) 
				{
					lrDirection_ = LRDirection::kRight;

					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

				// 左移動中の右入力
				if (velocity_.x < 0.0f) 
				{
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x += (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
			}
			else if (Input::GetInstance()->PushKey(DIK_LEFT)) 
			{
				if (lrDirection_ != LRDirection::kLeft) 
				{
					lrDirection_ = LRDirection::kLeft;

					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

				// 右移動中の左入力
				if (velocity_.x > 0.0f) 
				{
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
			}
			// 加速/減速
			velocity_ += acceleration;
			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} 
		else 
		{
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) 
		{
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAccleration, 0);
		}
	} 
	else 
	{
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::CheckMapCollision(CollisionMapInfo& info) 
{
	CheckMapCollisionUp(info);
	/*CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);*/
}

void Player::CheckMapMove(const CollisionMapInfo& info) 
{
	//移動
	worldTransform_.translation_ += info.move;
}

void Player::CheckMapCeiling(const CollisionMapInfo& info) 
{
	if (info.ceiling) 
	{
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
	}
}

void Player::AnimateTurn() 
{
	if (turnTimer_ > 0.0f) 
	{
		turnTimer_ = 1.0f / 60.0f;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状況に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) 
{ 
	Vector3 offsetTable[kNumCorner] = 
	{
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) 
{

	if (info.move.y <= 0) 
	{
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, 5> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) 
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	//真上の当たり判定を行う
	bool hit = false;
	//左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yindex);
	if (mapChipType == MapChipType::kBlock) 
	{
		hit = true;
	}
	//右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);

	if (hit) 
	{
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0,kHeight / 2.0f,0));
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yindex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		//天井に当たったことを記録する
		info.ceiling = true;
	}



}

void Player::Update() {

	//1、移動入力
	InputMove();


	//衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	//移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	//2、マップ衝突チェック
	CheckMapCollision(collisionMapInfo);

	//3、判定結果を反映して移動させる
	CheckMapMove(collisionMapInfo);

	//4、天井に接触している場合の処理
	CheckMapCeiling(collisionMapInfo);


	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 下降中？
	if (velocity_.y < 0) 
	{
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) 
		{
			landing = true;
		}
	}
	// 接地判定
	if (onGround_) 
	{
		// ジャンプ開始
		onGround_ = false;
	} 
	else 
	{
		// 着地
		if (landing) 
		{
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}


	// 7、旋回制御
	AnimateTurn();

	// 8、行列計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }
