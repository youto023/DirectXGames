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

	
	
	//３Dモデル
	Model* model_ = nullptr;
	Model* modelBlock_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;
	   // デバックカメラ
		DebugCamera* debugCamera_ = nullptr;
		//デバックカメラ有効
	    bool isDebugCameraActive_ = false;



	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロテクション
	ViewProjection viewProjection_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
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
	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
