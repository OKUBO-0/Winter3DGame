#include "TitleScene.h"

TitleScene::TitleScene() { finished_ = false; }

TitleScene::~TitleScene() {
	delete model_;
	delete model2_;
	delete fade_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	BGMHandle_ = audio_->LoadWave("./Resources/audio/undersea.wav");

	model_ = Model::CreateFromOBJ("title", true);
	model2_ = Model::CreateFromOBJ("title2", true);

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// カメラの初期化
	camera_.Initialize();

	// スカイドームの生成
	skydome_ = new SkyDome();
	// スカイドームのモデル
	modelSkyDome_ = Model::CreateFromOBJ("skydome", true);
	// 初期化
	skydome_->Initialize(modelSkyDome_, &camera_);

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TitleScene::Update() {

	// BGMが再生されていない場合のみ再生する
	if (!isBGMPlaying_) {
		audio_->PlayAudio(BGMAudio_, BGMHandle_, true, 0.3f);
		isBGMPlaying_ = true; // フラグを立てる
	}

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		fade_->Start(Fade::Status::FadeOut, 1.0f);
		phaseFade_ = PhaseFade::kFadoOut;
	}

	switch (phaseFade_) {

	case TitleScene::PhaseFade::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Stop();
			phaseFade_ = PhaseFade::kMain;
		}
		break;

	case TitleScene::PhaseFade::kFadoOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			//fade_->Stop();
			audio_->StopAudio(BGMAudio_);
			finished_ = true;
		}
		break;
	}
}

void TitleScene::Draw() {
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

	model_->Draw(worldTransform_, camera_);
	model2_->Draw(worldTransform_, camera_);

	// スカイドームの描画
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

	fade_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}