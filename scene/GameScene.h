#pragma once
#include"Player.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DebugCamera.h>
#include<vector>
#include<Skydome.h>
#include"MapChipField.h"

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

	
	void GenerateBlocks();
	



	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	
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
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロテクション
	ViewProjection viewProjection_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	// ３Dモデル
	Model* model_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelSkydome_ = nullptr;
	
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 自キャラ
	Player* player_ = nullptr;
	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;
	// デバックカメラ有効
	bool isDebugCameraActive_ = false;
	//スカイドーム

	Skydome*skydome_ = nullptr;
	//マップチップフィールド
	MapChipField* mapChipField_;




	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
