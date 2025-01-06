#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

#include "../Player.h"
#include "../Enemy.h"
#include "../EnemyBullet.h"
#include "../SkyDome.h"

class GameScene {

public:

	GameScene();

	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize();

	// 衝突判定と応答
	void CheckAllCollisions();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t playerTextureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	// ワールドトランスフォーム
	WorldTransform playerWorldTransform_;
	WorldTransform enemyWorldTransform_;

	// カメラ
	Camera camera_;
	Camera playerCamera_;
	Camera enemyCamera_;

	// 自キャラ
	Player* player_ = nullptr;
	// 3Dモデルの生成
	Model* playerModel_ = nullptr;

	// 敵
	Enemy* enemy_ = nullptr;
	// 3Dモデルの生成
	Model* enemyModel_ = nullptr;

	// 天球
	SkyDome* skydome_ = nullptr;
	// 3Dモデルの生成
	Model* modelSkyDome_ = nullptr;

	// デバッグカメラ
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
};
