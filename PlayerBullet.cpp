#include "PlayerBullet.h"
#include "MathUtilityEx.h"
#include <cassert>

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = Model::CreateFromOBJ("playerAttack", true);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::OnCollision() {
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() {

	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::Update() {

	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const Camera& camera) {

	// モデルの描画
	model_->Draw(worldTransform_, camera);
}