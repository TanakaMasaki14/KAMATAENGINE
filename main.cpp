#include "Audio.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// �ėp�@�\
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;

	// �Q�[���E�B���h�E�̍쐬
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX����������
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region �ėp�@�\������
	// ���͂̏�����
	input = Input::GetInstance();
	input->Initialize();

	// �I�[�f�B�I�̏�����
	audio = Audio::GetInstance();
	audio->Initialize();

	// �e�N�X�`���}�l�[�W���̏�����
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// �X�v���C�g�ÓI������
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// �f�o�b�O�e�L�X�g������
	debugText = DebugText::GetInstance();
	debugText->Initialize();

	// 3D���f���ÓI������
	Model::StaticInitialize();

	// �������\��������
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// �Q�[���V�[���̏�����
	gameScene = new GameScene();
	gameScene->Initialize();

	// ���C�����[�v
	while (true) {
		// ���b�Z�[�W����
		if (win->ProcessMessage()) {
			break;
		}

		// ���͊֘A�̖��t���[������
		input->Update();
		// �Q�[���V�[���̖��t���[������
		gameScene->Update();
		// ���\���̍X�V
		axisIndicator->Update();

		// �`��J�n
		dxCommon->PreDraw();
		// �Q�[���V�[���̕`��
		gameScene->Draw();
		// ���\���̕`��
		axisIndicator->Draw();
		// �v���~�e�B�u�`��̃��Z�b�g
		primitiveDrawer->Reset();
		// �`��I��
		dxCommon->PostDraw();
	}

	// �e����
	SafeDelete(gameScene);
	audio->Finalize();

	// �Q�[���E�B���h�E�̔j��
	win->TerminateGameWindow();

	return 0;
}