#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

class EnemyBullet {

public:
	// 初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	Vector3 GetWorldPosition();

	// 更新
	void Update();

	// 描画
	void Draw(const Camera& camera);

	// 消滅
	bool IsDead() const { return isDead_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};