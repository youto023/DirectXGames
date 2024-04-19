#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DebugCamera.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

	//IuGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};

	
	

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	//テクスチャ
	Sprite* sprite_ = nullptr;
	
	//３Dモデル
	Model* model_ = nullptr;

	   // デバックカメラ
		DebugCamera* debugCamera_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロテクション
	ViewProjection viewProjection_;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
