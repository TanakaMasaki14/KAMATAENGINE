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


	//���L�����̐���
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	//teki�L�����̐���
	Vector3 position = worldTransforms_->translation_;
	enemy_ = new Enemy();
	//�G�L�����Ɏ��L�����̃A�h���X��n��
	enemy_->SetPlayer(player_);

	enemy_->Initialize(model_, position, textureHandle_);


	//Enemy* newEnemy = new Enemy();
	//std::unique_ptr<Enemy>newEnemy = std::make_unique<Enemy>();
	////���L�����̏�����
	//newEnemy->Initialize(model_, position, textureHandle_);
	//enemy_.reset(newEnemy);

	// �r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();

	//�f�o�b�N�J�����̐���
	debugCamera_ = new DebugCamera(1280, 720);

	// �������\����L���ɂ���
	AxisIndicator::GetInstance()->SetVisible(true);
	// �������\�����Q�Ƃ���r���[�v���W�F�N�V�������w�肷�� (�A�h���X�n��)
	AxisIndicator::SetTargetViewProjection(&viewProjection_);
}



void GameScene::Update() {
	debugCamera_->Update();

	//���L�����̍X�V
	player_->Update();
	//�G�̍X�V
	enemy_->Update();
	//#ifdef _DEBUG 
	//	if (input_->TriggerKey(DIK_O))
	//	{
	//		//�f�o�b�O�J�����L���t���O���g�O�� 
	//		isDebugCameraActive_ = true;
	//	}
	//
	//	//�J�����̏���
	//	if (isDebugCameraActive_)
	//	{
	//		
	//		debugCamera_->Update();
	//		viewProjection_.matView = debugCamera_->SetDistance(float distance) { distance_ = distance;};
	//		viewProjection_.matProjection = �f�o�b�O�J�����̃v���W�F�N�V�����s��;
	//		�r���[�v���W�F�N�V�����̓]��
	//	}
	//	else 
	//	{
	//		�r���[�v���W�F�N�V�����s��̍Čv�Z�Ɠ]��;
	//	}
	//#endif

}

void GameScene::Draw() {

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon_->ClearDepthBuffer();

#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(commandList);

	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>
	//3D���f���`��
	//���@�̕`��
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	// 3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}