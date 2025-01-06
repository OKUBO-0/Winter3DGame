#include "GameScene.h"
#include <cassert>
#include "../MathUtilityEx.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	delete playerModel_;
	delete enemy_;
	delete modelSkyDome_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());

	// プレイヤーのテクスチャ
	playerTextureHandle_ = TextureManager::Load("white1x1.png");
	// プレイヤーのモデル
	playerModel_ = Model::Create();
	// プレイヤーのワールドトランスフォームの初期化
	playerWorldTransform_.Initialize();
	// プレイヤーのカメラの初期化
	camera_.Initialize();
	// プレイヤーの生成
	player_ = new Player();
	// プレイヤーの初期化
	player_->Initialize(playerModel_, playerTextureHandle_);

	// 敵のテクスチャ
	enemyTextureHandle_ = TextureManager::Load("red1x1.png");
	// 敵のモデル
	enemyModel_ = Model::Create();
	// 敵のワールドトランスフォームの初期化
	enemyWorldTransform_.Initialize();
	// 敵のカメラの初期化
	enemyCamera_.Initialize();
	// 敵キャラの生成
	enemy_ = new Enemy;
	// 敵キャラの初期化
	enemy_->Initialize(enemyModel_, enemyTextureHandle_);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// 天球
	modelSkyDome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new SkyDome();
	skydome_->Initialize(modelSkyDome_, &camera_);
}

void GameScene::CheckAllCollisions() {
	// 判定AとBの座標
	Vector3 posA, posB;
	// 自弾リストの取得
	const std::list<PlayerBullet*> playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();
#pragma region
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets) {

		posB = bullet->GetWorldPosition();

		Vector3 subtract = posB - posA;
		float a = Length(subtract);
		if (a < 3.0f) {

			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion
#pragma region
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		Vector3 subtract = posB - posA;
		float a = Length(subtract);
		if (a < 3.0f) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion
#pragma region

	for (PlayerBullet* pBullet : playerBullets) {
		for (EnemyBullet* eBullet : enemyBullets) {
			posA = pBullet->GetWorldPosition();
			posB = eBullet->GetWorldPosition();
			Vector3 subtract = posB - posA;
			float a = Length(subtract);
			if (a < 3.0f) {
				pBullet->OnCollision();
				eBullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::Update() {

	// デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

	// プレイヤーの更新
	player_->Update();

	// 敵の更新
	enemy_->Update();

	// 天球の更新
	skydome_->Update();

	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dモデル描画
	// model_->Draw(worldTransform_, camera_, textureHandle_);

	// プレイヤーの描画
	if (isDebugCameraActive_ == false) {
		player_->Draw(camera_);
	}

	if (isDebugCameraActive_) {
		playerModel_->Draw(playerWorldTransform_, debugCamera_->GetCamera(), playerTextureHandle_);
	}

	// 敵の描画
	enemy_->Draw(enemyCamera_);

	// 天球の描画
	skydome_->Draw(camera_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}