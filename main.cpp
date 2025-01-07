#include <KamataEngine.h>
using namespace KamataEngine;

#include "../DirectXGame/scene/ScoreScene.h"
#include "../DirectXGame/scene/GameScene.h"
#include "../DirectXGame/scene/RuleScene.h"
#include "../DirectXGame/scene/TitleScene.h"

TitleScene* titleScene = nullptr;
RuleScene* ruleScene = nullptr;
GameScene* gameScene = nullptr;
ScoreScene* scoreScene = nullptr;

// シーン
enum class Scene {
	kUnknown = 0,

	kTitle,
	kRule,
	kGame,
	kScore
};
Scene scene = Scene::kUnknown;

void ChangeScene() {

	switch (scene) {
	case Scene::kTitle:
		if (titleScene->GetIsFinished()) {
			// シーン変更
			scene = Scene::kRule;
			// 旧シーン解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			ruleScene = new RuleScene;
			ruleScene->Initialize();
		}
		break;

	case Scene::kRule:
		if (ruleScene->GetIsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーン解放
			delete ruleScene;
			ruleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;

	case Scene::kGame:
		if (gameScene->GetIsFinished()) {
			// シーン変更
			scene = Scene::kScore;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成と初期化
			scoreScene = new ScoreScene;
			scoreScene->Initialize();
		}
		break;

	case Scene::kScore:
		if (scoreScene->GetIsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete scoreScene;
			scoreScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	}
}

const char* GetSceneName(Scene scenes) {
	switch (scenes) {
	case Scene::kTitle:
		return "Title";
	case Scene::kRule:
		return "Rule";
	case Scene::kGame:
		return "Game";
	case Scene::kScore:
		return "Score";
	default:
		return "Unknown";
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;

	case Scene::kRule:
		ruleScene->Update();
		break;

	case Scene::kGame:
		gameScene->Update();
		break;

	case Scene::kScore:
		scoreScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;

	case Scene::kRule:
		ruleScene->Draw();
		break;

	case Scene::kGame:
		gameScene->Draw();
		break;

	case Scene::kScore:
		scoreScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;

	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// タイトルシーン初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// ルールシーン初期化
	ruleScene = new RuleScene;
	ruleScene->Initialize();

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	// スコアシーン初期化
	scoreScene = new ScoreScene;
	scoreScene->Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// シーン切り替え
		ChangeScene();
		// 現在シーン更新
		UpdateScene();
		// 軸表示の更新
		axisIndicator->Update();

#ifdef _DEBUG
		// imGui
		ImGui::Begin("Scene");
		ImGui::Text("Scene: %s", GetSceneName(scene)); // シーン名を表示
		ImGui::End();
#endif // _DEBUG

		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// 現在シーンの描画
		DrawScene();

		// 軸表示の描画
		//axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	// 3Dモデル解放
	delete titleScene;
	delete ruleScene;
	delete gameScene;
	delete scoreScene;

	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}