#include <cassert>
#include "Enemy.h"
#include "MathUtilityEx.h"
#include "Player.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// シングルインスタンスを取得する
	input_ = Input::GetInstance();

	// NULLポインタチェック
	assert(model);

	// メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 弾
	flag = 0;
	timer = 0.0f;

	// 初期座標
	worldTransform_.translation_ = Vector3(20.0f, 0.0f, 20.0f);
}

void Enemy::Approach() {

	// 敵の移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};
	Vector3 speed = {0.0f, 0.0f, 0.0f};

	// 移動
	worldTransform_.translation_ += speed;

	// フェーズ移動
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {

	// 敵の移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};
	Vector3 speed = {0.0f, 0.0f, 0.0f};

	// 移動
	worldTransform_.translation_ += speed;
}

void Enemy::Fire() {

	// 発射タイマー
	timer++;
	if (timer >= 150.0f) {
		flag = 1;
	} else {
		flag = 0;
	}

	if (flag == 1) {

		const float kEnemyBulletSpeed = 1.0f;

		Vector3 targetPos = player_->GetWorldPosition();
		Vector3 basePos = this->GetWorldPosition();
		Vector3 velocity = targetPos - basePos;

		velocity = Normalize(velocity);
		velocity *= kEnemyBulletSpeed;

		if (enemyBullet_) {
			delete enemyBullet_;
			enemyBullet_ = nullptr;
		}

		// 弾を生成し、初期化
		EnemyBullet* newEnemyBullet = new EnemyBullet();
		newEnemyBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録する
		enemyBullets_.push_back(newEnemyBullet);
		timer = 0.0f;
	}
}

void Enemy::OnCollision() {

}

void Enemy::Update() {

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	switch (phase_) {

	case Enemy::Phase::Approach:
	default:
		Approach();
		break;

	case Enemy::Phase::Leave:
		Leave();
		break;
	}

	Fire();

	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(Camera& camera) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera, textureHandle_);

	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(camera);
	}
}

Enemy::~Enemy() { 
	delete enemyBullet_; 
}

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}