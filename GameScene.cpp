#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<random>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include"DebugCamera.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {

	delete model_;
	delete debugCamera_;
	//delete player_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();


	//自キャラの生成
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	//tekiキャラの生成
	Vector3 position = worldTransforms_->translation_;
	enemy_ = new Enemy();
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	enemy_->Initialize(model_, position, textureHandle_);


	//Enemy* newEnemy = new Enemy();
	//std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
	////自キャラの初期化
	//newEnemy->Initialize(model_, position, textureHandle_);
	//enemy_.reset(newEnemy);

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::SetTargetViewProjection(&viewProjection_);
}



void GameScene::Update() {
	debugCamera_->Update();

	//自キャラの更新
	player_->Update();
	//敵の更新
	enemy_->Update();
	//#ifdef _DEBUG 
	//	if (input_->TriggerKey(DIK_O))
	//	{
	//		//デバッグカメラ有効フラグをトグル 
	//		isDebugCameraActive_ = true;
	//	}
	//
	//	//カメラの処理
	//	if (isDebugCameraActive_)
	//	{
	//		
	//		debugCamera_->Update();
	//		viewProjection_.matView = debugCamera_->SetDistance(float distance) { distance_ = distance;};
	//		viewProjection_.matProjection = デバッグカメラのプロジェクション行列;
	//		ビュープロジェクションの転送
	//	}
	//	else 
	//	{
	//		ビュープロジェクション行列の再計算と転送;
	//	}
	//#endif

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
	//3Dモデル描画
	//自機の描画
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}