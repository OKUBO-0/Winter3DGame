#include "Player.h"
#include "MathUtilityEx.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {

	// シングルインスタンスを取得する
	input_ = Input::GetInstance();

	// NULLポインタチェック
	assert(model);

	// メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Rotate() {

	// 回転速度[ラジアン/frame]
	const float kRotateSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {

		worldTransform_.rotation_.y += kRotateSpeed;
	}

	else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y -= kRotateSpeed;
	}
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {

}

void Player::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	// 座標移動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;

	// アフィン変換
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("window");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -20.0f, 20.0f);
	ImGui::End();

	// 移動限界座標
	const float kMoveLimitX = 30.0f;
	const float kMoveLimitY = 15.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 回転
	Rotate();

	// 攻撃
	Attack();

	// 弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(Camera& camera) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, camera, textureHandle_);

	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

Player::~Player() { 
	delete bullet_; 
}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}