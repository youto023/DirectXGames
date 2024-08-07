#include "GameScene.h"
#include "CameraController.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>

// コンストラクタ
GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {

	delete model_;

	delete player_;

	delete skyDome_;

	delete modelSkydome_;

	delete modelBlock_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	// worldTransformBlocks_.clear();

	delete debugCamera_;

	// マップチップフィールドの開放
	delete mapChipFiled_;

	// カメラ初期化
	delete CameraController_;

	// 敵
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクション生成
	viewProjection_.Initialize();
	viewProjection_.farZ = 600; // これを５００とかにすると後ろの方ででっかい弾が出る。

	mapChipFiled_ = new MapChipField;

	// マップチップフィールドの初期化
	mapChipFiled_->Initialize();

	mapChipFiled_->LoadMapChipCsv("Resources/blocks.csv");

	// 表示ブロックの生成
	GenerateBlocks();

	// 3Dモデルデータの生成
	model_ = Model::Create();

	// 画像の読み込み
	textureHandle_ = TextureManager::Load("player.png");

	// ３Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// マップチップフィールドの生成
	// mapChipFiled_ = new MapChipField();

	// プレイヤーモデルの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	// プレイヤーの生成
	player_ = new Player();

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipFiled_->GetMapChipPositionByIndex(7, 7);

	// プレイヤーの初期化
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition); // 元player_->Initialize(modelPlayer_, &viewProjection_);

	player_->SetMapChipField(mapChipFiled_);

	// 天球の生成
	skyDome_ = new Skydome();
	// 天球の初期化
	skyDome_->Initialize(modelSkydome_, &viewProjection_);

	// ブロックのモデルを読み込む
	modelBlock_ = Model::CreateFromOBJ("block", true);

	// 要素数
	// const uint32_t kNumBlockVirtical = 10;//10
	// const uint32_t kNumBlockHorizontal = 100;//20
	// ブロック1個分の横幅
	// const float kBlockWidth = 1.0f;//2.0f
	// const float kBlockHeight = 1.0f;//2.0f
	// 要素数を変更する
	// 配列を設定
	// worldTransformBlocks_.resize(kNumBlockVirtical);
	// for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
	//	// １列の要素数を設定
	//	worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	//}
	//
	//// キューブの生成
	// for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
	//	for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
	//
	//		// if文＋continueで表示しないブロックを指定できる
	//		if ((i + j) % 2 == 0)
	//			continue;
	//
	//		worldTransformBlocks_[i][j] = new WorldTransform();
	//		worldTransformBlocks_[i][j]->Initialize();
	//		worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
	//		worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
	// }
	//}

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// カメラコントローラーの生成
	CameraController_ = new CameraController;

	// カメラ初期化
	CameraController_->Initialize();

	// 追従対象をセット
	CameraController_->SetTarget(player_);

	// リセット
	CameraController_->Reset();

	// カメラ移動の範囲指定
	Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	CameraController_->SetMovableArea(cameraArea);

	// 敵初期化
	enemyModel_ = Model::CreateFromOBJ("enemy", true);

	Vector3 enemyPosition = mapChipFiled_->GetMapChipPositionByIndex(20, 18);

	enemy_ = new Enemy();

	enemy_->Initialize(enemyModel_, &viewProjection_, enemyPosition);
}

void GameScene::Update() {

	// プレイヤーの更新
	player_->Update();

	// 天球の更新
	skyDome_->Update();

	// 敵
	enemy_->Update();

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock)
				continue;

			//// 平行移動行列
			// Matrix4x4 result = {
			//     1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, worldTransformBlock->translation_.x, worldTransformBlock->translation_.y,
			//     worldTransformBlock->translation_.z, 1.0f};

			// Matrix4x4 matWorld = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			//// 平行移動だけ代入
			// worldTransformBlock->matWorld_ = matWorld;

			//// 定数バッファに転送する
			// worldTransformBlock->TransferMatrix();

			worldTransformBlock->UpdateMatrix();
		}
	}

	// デバッグカメラの更新
	debugCamera_->Update();

	// カメラコントローラー
	CameraController_->Update();

	// 全ての当たり判定を行う
	CheckAllCollisions();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}

#endif // _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matView = CameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = CameraController_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// プレイヤーの描画処理
	player_->Draw();

	// 天球の描画処理
	skyDome_->Draw();

	// 敵
	enemy_->Draw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {

	// 要素数
	const uint32_t kNumBlockVirtical = mapChipFiled_->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizontal = mapChipFiled_->GetNumBlockHorizontal();
	// ブロック1個分の横幅
	// const float kBlockWidth = 2.0f;
	// const float kBlockHeight = 2.0f;
	// 要素数を変更する
	// 配列を設定
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		// １列の要素数を設定
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			if (mapChipFiled_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipFiled_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
#pragma region 自キャラと敵の当たり判定
	{
		// 判定対象1と2の座標
		AABB aabb1, aabb2;

		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {
			// 敵弾の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを出す
				player_->OnCollision(enemy);

				// 敵弾の衝突時コールバックを出す
				enemy->OnCollision(player_);
			}
		}
	}

#pragma endregion

#pragma region 自キャラとアイテムの当たり判定
#pragma endregion

#pragma region 自弾と敵の当たり判定
#pragma endregion
}
