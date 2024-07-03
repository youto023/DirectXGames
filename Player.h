#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
///	自キャラ
/// </summary>
class Player {

public: // 引数を書くところ
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position); // void Initialize(Model* model, ViewProjection* viewProjection);

	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private: // 関数（メンバ変数）
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// 速度
	Vector3 velocity_ = {};

	// 角度補間

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 接地状態フラグ
	bool onGround_ = true;

	// 慣性移動
	static inline const float kAcceleration = 0.01f;

	// 速度減衰
	static inline const float kAttenuation = 0.01f;

	// 最大速度制限
	static inline const float kLimitRunSpeed = 1;


	// 旋回時間＜秒＞
	static inline const float kTimeTurn = 0.3f;

	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.05f;

	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.5f;

	// ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 0.5f;

	ViewProjection* viewProjection_ = nullptr;
};
