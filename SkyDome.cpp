#include "SkyDome.h"
#include <cassert>
#include "MathUtilityEx.h"

void SkyDome::Initialize(Model* model, Camera* camera) {
	assert(model);
	model_ = model;
	camera_ = camera;
	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void SkyDome::Update() {
	worldTransform_.rotation_.y -= 0.001f;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void SkyDome::Draw(Camera& camera) {
	model_->Draw(worldTransform_, camera);
}