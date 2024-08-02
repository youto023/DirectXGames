#include "CameraController.h"
#include "Player.h"
void CameraController::Initialize() {

	viewProjection_.Initialize();
		
		
}

void CameraController::Update() {

    // 追従対象のワールドトランスフォームを参照

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	const Vector3& targetVelocity = target_->GetVelocity();
	// 追従対象とオフセットと追従対象の速度からカメラの目標座標の計算
	cameraTatget_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;

	//座標補間によりゆったり追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, cameraTatget_, kInterpolationRate);

	//移動範囲制限
	viewProjection_.translation_.x = max(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = min(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = max(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = min(viewProjection_.translation_.y, movableArea_.top);
	//行列の更新する
	viewProjection_.UpdateMatrix();	

}

void CameraController::Reset() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	//追従対象とオフセットからカメラの座標の計算
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;


}
