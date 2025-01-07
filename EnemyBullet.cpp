#include "EnemyBullet.h"
#include "MathUtilityEx.h"
#include <cassert>

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = Model::CreateFromOBJ("enemyAttack", true);

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void EnemyBullet::OnCollision() {
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition() {

	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::Update() {

	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const Camera& camera) {

	// モデルの描画
	model_->Draw(worldTransform_, camera);
}