#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

#include "EnemyBullet.h"

class Player;
class Enemy {
public:
	// 行動フェーズ
	enum class Phase {
		Approach,
		Leave,
	};
	// フェーズ
	Phase phase_ = Phase::Approach;

	// キーボード入力
	Input* input_ = nullptr;

	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);

	// 接近
	void Approach();

	// 離脱
	void Leave();

	// 弾発射
	void Fire();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	// 更新
	void Update();

	// 描画
	void Draw(Camera& camera);

	// 自キャラ
	Player* player_ = nullptr;

	void SetPlayer(Player* player) { player_ = player; }


	Vector3 GetWorldPosition();

	// デストラクタ
	~Enemy();

	// 弾
	std::list<EnemyBullet*> enemyBullets_;
	EnemyBullet* enemyBullet_ = nullptr;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 弾
	int flag;
	float timer;
};