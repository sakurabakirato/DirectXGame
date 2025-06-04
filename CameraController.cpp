#include "CameraController.h"
#include "Player.h"

using namespace KamataEngine;
using namespace MathUtility;

void CameraController::Initialize() {
	 
	camera_.Initialize();

}

void CameraController::Update() {

	const Vector3& targetVelocity = target_->GetVelocity();

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからからカメラの座標計算
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;

	camera_.translation_.x = Lerp(camera_.translation_.x, targetPosition_.x, kInterpolationRate);

	camera_.translation_.x = max(camera_.translation_.x, targetPosition_.x + targetMargin.left);
	camera_.translation_.x = min(camera_.translation_.x, targetPosition_.x + targetMargin.right);
	camera_.translation_.y = max(camera_.translation_.y, targetPosition_.y + targetMargin.bottom);
	camera_.translation_.y = min(camera_.translation_.y, targetPosition_.y + targetMargin.top);

	camera_.translation_.x = max(camera_.translation_.x, movebleArea_.left);
	camera_.translation_.x = min(camera_.translation_.x, movebleArea_.right);
	camera_.translation_.y = max(camera_.translation_.y, movebleArea_.bottom);
	camera_.translation_.y = min(camera_.translation_.y, movebleArea_.top);

	// 行列を更新
	camera_.UpdateMatrix();

}

void CameraController::Reset() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからからカメラの座標計算
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
	

}
