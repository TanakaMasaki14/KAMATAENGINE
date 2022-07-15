#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene {

public: // �����o�֐�
  /// <summary>
  /// �R���X�g�N���^
  /// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();


	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();



private: // �����o�ϐ�
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	bool isDebugCameraActive_ = false;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	//3D���f��
	Model* model_ = nullptr;

	//�f�o�b�O�J����
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// �Q�[���V�[���p
	//���L����
	Player* player_ = nullptr;
	//std::unique_ptr<Player> player_;
	Enemy* enemy_ = nullptr;
	//teki
	//std::unique_ptr<Enemy> enemy_;
	//�J����������̊p�x
	float viewAngle = 0.0f;
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransforms_[100];
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	/// </summary>

	//Matrix4 GetMatrix(const WorldTransform& worldTransforms_);
public:
	enum PartId {
		kRoot,		// �匳
		kSpine,		// �Ґ�
		kChest,		// ��
		kHead,		// ��
		kArmL,		// ���r
		kArmR,		// �E�r
		kHip,		// �K
		kLegL,		// ����
		kLegR,		// �E��

		kNumPartId
	};
};