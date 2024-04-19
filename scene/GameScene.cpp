#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"ImGuiManager.h"
#include"PrimitiveDrawer.h"

//コンストラクタ
GameScene::GameScene() {}
//デスクトラクタ
GameScene::~GameScene() { 
	delete sprite_;
	delete model_;
	delete debugCamera_;

}

void GameScene::Initialize() {
	//ワールドトランスフォームの初期化
	worldTransform_ .Initialize();
	// ビュープロテクションの初期化
	viewProjection_.Initialize();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("koumori.png");
	//スプライト生成
	sprite_ = Sprite ::Create(textureHandle_, {100, 50});
	//3Dモデル生成
	model_ = Model::Create();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(200, 100);
}

void GameScene::Update() { 
	//スプライトの今の座標取得
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);
	
	//デバックテキストの表示
	ImGui::Begin("Debug1");
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::Text("Kamata Tarou %d %d %d", 2050, 12, 31);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();
	
	//デモウィンドウの表示を有効化
	ImGui::ShowDemoWindow();
	
	//デバックカメラの最新
	debugCamera_->Update();

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
	// ３Dモデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
