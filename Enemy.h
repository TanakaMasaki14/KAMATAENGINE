#pragma once
#include <cassert>
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
#include "EnemyBullet.h"
#include <memory>
#include<list>
#include "MathUtility.h"

using namespace MathUtility;

/// <summary>
///  ���@�N���X�̑O���錾
/// </summary>
class Player;
///<summary>
///�G
///</summary>

class Enemy {
public:
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Enemy();

	///<summary>///������///</summary>///<paramname="model">���f��</param>///<paramname="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, const Vector3& position, uint32_t textureHandle);
	///<summary>
	///�X�V
	///</summary>
	void Update();
	///<summary>
	///�`��
	///</summary>
	//void Draw(ViewProjection viewProjection_);
	void Draw(ViewProjection& viewProjection);
	void TransferMatrix();
	void Move();
	void Fire();
	//�ڋ߃t�F�[�Y������
	void ApproachInit();

	// ���ˊԊu
	static int const kFireInterval = 60;

	// �Z�b�^�[
	void SetPlayer(Player* player) { player_ = player; }

	// ���[���h���W���擾
	Vector3 GetWorldPosition();
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	DebugText* debugText_ = nullptr;

	//�s���t�F�[�Y
	enum class Phase {
		Approach,//�ڋ߂���
		Leave,//���E����
	};
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�e
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	//���˃^�C�}�[
	int32_t fireTimer = 0;

	//���L����
	Player* player_ = nullptr;
};