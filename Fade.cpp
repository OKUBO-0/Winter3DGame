#include "Fade.h"
#include <algorithm>

Fade::~Fade() {
	delete sprite_;
}

void Fade::Initialize() {

	textureHandle_ = TextureManager::Load("white1x1.png");
	sprite_ = Sprite::Create(textureHandle_, { 0, 0 });
	sprite_->SetSize(Vector2(1280, 720));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}


void Fade::Update() {

	// フェード状態による分岐
	switch (status_) {

	case Fade::Status::None:
		// 何もしない
		break;


	case Fade::Status::FadeIn:
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード持続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、経過時間がフェード持続時間に近づくほどアルファ値を小さくする
		sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(1.0f - (counter_ / duration_), 0.0f, 1.0f)));
		break;


	case Fade::Status::FadeOut:
		//1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		//フェード持続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		//0.0fから1.0fの間で、経過時間がフェード持続時間に近づくほどアルファ値を大きくする
		sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	}
}


void Fade::Draw() {

	if (status_ == Status::None) {
		return;
	}
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	Sprite::PostDraw();
}


void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}


void Fade::Stop() {
	status_ = Status::None;
}


bool Fade::IsFinished() const {

	// フェード状態による分岐
	switch (status_) {
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:
		if (counter_ >= duration_) {
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}