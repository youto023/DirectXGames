#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"ImGuiManager.h"
#include"PrimitiveDrawer.h"
#include"MathUtilityForText.h"




//コンストラクタ
GameScene::GameScene() {}

//デスクトラクタ
GameScene::~GameScene() {
	delete player_;
	delete model_;
	delete debugCamera_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
		delete worldTransformBlock;
	}
}
	delete modelSkydome_;
	worldTransformBlocks_.clear();
	// マップチップフィールドの解放
	delete mapChipField_;
}
void GameScene::GenerateBlocks() {                                               
	// 要素数
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();     // 縦
	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal(); // 横
	// 要素数を変更する

	worldTransformBlocks_.resize(kNumBlockVirtical);
	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
void GameScene::Initialize() {
	//ワールドトランスフォームの初期化
	worldTransform_ .Initialize();
	// ビュープロテクションの初期化
	viewProjection_.Initialize();
	
	textureHandle_ = TextureManager::Load("koumori.png");
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelBlock_ = Model::CreateFromOBJ("block");
	//自キャラ生成
	player_ = new Player();
	// スカイドーム生成
	skydome_ = new Skydome();
	//3Dモデル生成
	model_ = Model::Create();
	//マップチップ生成
	mapChipField_ = new MapChipField;
	// 自キャラの更新
	player_->Initialize(model_, textureHandle_,&viewProjection_);
	
	// スカイドームの更新
	skydome_->Initialize(modelSkydome_, &viewProjection_);
	
	// マップチップの更新
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(200, 100);

	
	
	

	
}

void GameScene::Update() { 
	
	

	
	//デモウィンドウの表示を有効化
	ImGui::ShowDemoWindow();
	
	//デバックカメラの最新
	debugCamera_->Update();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif 
	//カメラ処理

	if (isDebugCameraActive_) {
		// デバックカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}


	//自キャラの更新
	player_->Update();


	skydome_->Update();


	//ブロックの最新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			
			if (!worldTransformBlock) {
				continue;
			}
		//	Matrix4x4 result{
			 //   1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, worldTransformBlock->translation_.x, worldTransformBlock->translation_.y, worldTransformBlock->translation_.z,
			 //   1.0f};
			// 平行移動代入
			//worldTransformBlock->matWorld_ = result;
			worldTransformBlock->UpdeteMatrix();
			// 定数バッファに転送する
			//worldTransformBlock->TransferMatrix();
		}
	}
	//Matrix4x4 matWorld = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

	//worldTransformBlock->matWorld_ = matWorld;
	


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

	
	//PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	
	//
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// ３Dモデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);



	skydome_->Draw();


	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

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
