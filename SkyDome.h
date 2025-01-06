#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

class SkyDome {

public:

	// 初期化
	void Initialize(Model* model, Camera* camera);

	// 更新
	void Update();

	// 描画
	void Draw(Camera& camera);

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Camera* camera_;
};