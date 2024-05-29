#include "Skydome.h"
#include <cassert>

void Skydome:: Initialize(Model* model, ViewProjection* viewProjection){
	// NULLポインタチェック
	assert(model);
	model_ = model;
	
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	
}

void Skydome::Update() { worldTransform_.UpdeteMatrix(); }

void Skydome::Draw() { model_->Draw(worldTransform_, *viewProjection_); }




