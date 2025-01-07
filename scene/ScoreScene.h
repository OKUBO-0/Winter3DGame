#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

#include <list>
#include "../SkyDome.h"
#include "../Fade.h"

class ScoreScene {
public:

	ScoreScene();

	~ScoreScene();

	void Initialize();

	void Update();

	void Draw();

	bool GetIsFinished() const { return finished_; }

private:

	// シーンのフェード
	enum class PhaseFade {
		kFadeIn,	// フェードイン
		kMain,	    // メイン部
		kFadoOut,	// フェードアウト
	};

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	int BGMHandle_ = 0;
	int BGMAudio_ = -1;

	// BGMが再生されているかを追跡
	bool isBGMPlaying_ = false;

	bool finished_ = false;

	// モデル
	Model* model_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// カメラ
	Camera camera_;

	// スカイドーム
	SkyDome* skydome_ = nullptr;
	// スカイドームの3Dモデル
	Model* modelSkyDome_ = nullptr;

	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	PhaseFade phaseFade_ = PhaseFade::kFadeIn;
};