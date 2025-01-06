#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

#include "PlayerBullet.h"
#include <list>

class Player {
public:
	// キーボード入力
	Input* input_ = nullptr;

	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);

	// 回転
	void Rotate();

	// 攻撃
	void Attack();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// 更新
	void Update();

	// 描画
	void Draw(Camera& camera);

	// デストラクタ
	~Player();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 弾
	std::list<PlayerBullet*> bullets_;
	PlayerBullet* bullet_ = nullptr;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};