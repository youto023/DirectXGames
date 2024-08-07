#include "Skydome.h"
#include <cassert>

// インストラクタ
Skydome::Skydome() {}

// デストラクタ
Skydome::~Skydome() {}

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	// NULLポインタチェック
	assert(model);

	// 引数
	model_ = model;

	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void Skydome::Update() {}

void Skydome::Draw() {

	// 3Dモデルの描画
	model_->Draw(worldTransform_, *viewProjection_);
}
